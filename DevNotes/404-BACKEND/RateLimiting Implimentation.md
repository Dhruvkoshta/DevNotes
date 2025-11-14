
```js
/*
  nodejs-rate-limiter.js
  A flexible, production-ready rate limiting middleware for Express (Node.js).

  Features:
    - Pluggable stores: Redis (distributed) and in-memory (single-process fallback)
    - Sliding window implementation in Redis using sorted sets for accurate limits
    - Configurable window size and max requests, per-key identification (IP, user id)
    - Response headers: RateLimit-Limit, RateLimit-Remaining, RateLimit-Reset
    - Customizable callbacks and behavior when limit exceeded
    - Easy Express integration

  Usage example:
    const express = require('express');
    const createRateLimiter = require('./nodejs-rate-limiter');

    // create Redis client (ioredis or node-redis compatible)
    const Redis = require('ioredis');
    const redis = new Redis();

    const limiter = createRateLimiter({
      windowMs: 60_000,          // 1 minute window
      max: 100,                  // max 100 requests per window per key
      redisClient: redis,        // optional, for distributed limiting
      keyGenerator: (req) => req.user?.id || req.ip,
      trustProxy: true,
    });

    const app = express();
    app.use(limiter.middleware());

    app.get('/', (req, res) => res.send('ok'));

    app.listen(3000);
*/

const DEFAULTS = {
  windowMs: 60_000,
  max: 60,
  message: 'Too many requests, please try again later.',
  headers: true,
  keyPrefix: 'rl:',
  trustProxy: false,
  store: 'redis', // 'redis' | 'memory'
};

function createRateLimiter(options = {}) {
  const cfg = Object.assign({}, DEFAULTS, options);

  if (cfg.store === 'redis' && !cfg.redisClient) {
    console.warn('[rate-limiter] store=redis but no redisClient provided — falling back to memory store');
    cfg.store = 'memory';
  }

  const store = cfg.store === 'redis' ? createRedisStore(cfg) : createMemoryStore(cfg);

  function middleware() {
    return async function rateLimitMiddleware(req, res, next) {
      try {
        const key = (cfg.keyPrefix || '') + (await (cfg.keyGenerator ? cfg.keyGenerator(req) : defaultKeyGenerator(req, cfg)));

        const now = Date.now();
        const result = await store.increment(key, now);
        // result: { current, remaining, reset }

        if (cfg.headers) {
          res.setHeader('RateLimit-Limit', String(cfg.max));
          res.setHeader('RateLimit-Remaining', String(Math.max(0, result.remaining)));
          res.setHeader('RateLimit-Reset', String(Math.ceil(result.reset / 1000))); // seconds until reset
        }

        if (result.current > cfg.max) {
          // Exceeded
          if (cfg.onLimitReached) {
            // allow custom handling (logging, metrics)
            try { cfg.onLimitReached(req, { key, result, cfg }); } catch (e) { /* ignore */ }
          }

          res.status(cfg.statusCode || 429);
          // Recommended to include Retry-After header in seconds
          const retryAfterSec = Math.ceil(result.reset / 1000);
          res.setHeader('Retry-After', String(retryAfterSec));

          if (typeof cfg.handler === 'function') return cfg.handler(req, res, next);

          return res.json({ error: cfg.message, retry_after_seconds: retryAfterSec });
        }

        // allowed
        return next();
      } catch (err) {
        // If the store fails, decide whether to allow or block. Default: allow (fail-open)
        console.error('[rate-limiter] error', err);
        if (cfg.failClosed) {
          return res.status(500).json({ error: 'rate limiter error' });
        }
        return next();
      }
    };
  }

  return { middleware };
}

function defaultKeyGenerator(req, cfg) {
  // Use IP by default. If trustProxy is true, use req.ip (express handles X-Forwarded-For if trust proxy configured on app)
  if (cfg.trustProxy && req.ip) return req.ip;
  // fallback to remote address
  return (req.ip || req.connection?.remoteAddress || 'unknown');
}

// --- Memory store (single-process) ---
function createMemoryStore(cfg) {
  // Token-bucket inspired sliding window using timestamps array per key
  const clients = new Map();
  const windowMs = cfg.windowMs;
  const max = cfg.max;

  // periodic cleanup to avoid memory growth
  const CLEANUP_INTERVAL = Math.max(60_000, Math.floor(windowMs * 2));
  const cleanupHandle = setInterval(() => {
    const now = Date.now();
    for (const [key, item] of clients.entries()) {
      if (item.lastSeen + windowMs * 2 < now) clients.delete(key);
    }
  }, CLEANUP_INTERVAL).unref();

  async function increment(key, now) {
    let item = clients.get(key);
    if (!item) {
      item = { timestamps: [], lastSeen: now };
      clients.set(key, item);
    }
    item.lastSeen = now;
    const cut = now - windowMs;
    // keep only timestamps within window
    item.timestamps = item.timestamps.filter(ts => ts > cut);
    item.timestamps.push(now);

    const current = item.timestamps.length;
    const remaining = Math.max(0, max - current);
    // compute reset time: earliest timestamp in window + windowMs - now
    const reset = item.timestamps[0] ? (item.timestamps[0] + windowMs - now) : windowMs;

    return { current, remaining, reset };
  }

  // expose store API
  return { increment };
}

// --- Redis store (distributed, sliding window using sorted set) ---
function createRedisStore(cfg) {
  const redis = cfg.redisClient;
  const windowMs = cfg.windowMs;
  const max = cfg.max;
  const prefix = cfg.keyPrefix || 'rl:';

  // Lua script could be used for atomicity; but we'll use sorted-set commands which are atomic per command sequence
  async function increment(key, now) {
    const redisKey = key;
    const min = now - windowMs;

    // Use pipeline for efficiency
    const pipeline = redis.multi();
    pipeline.zadd(redisKey, 'NX', now, String(now));
    pipeline.zremrangebyscore(redisKey, 0, min);
    pipeline.zcard(redisKey);
    pipeline.pexpire(redisKey, windowMs + 1000);

    const execRes = await pipeline.exec();
    // execRes is array of results for each command
    // zadd -> [null, 1]
    // zremrangebyscore -> [null, count]
    // zcard -> [null, card]
    // pexpire -> [null, 1]

    // When using node-redis or ioredis the shape differs; normalize:
    const zcardRes = execRes && execRes[2];
    let current;
    if (Array.isArray(zcardRes)) {
      // ioredis returns [null, value]
      current = Number(zcardRes[1]);
    } else if (typeof zcardRes === 'number') {
      current = zcardRes;
    } else if (zcardRes && zcardRes[1] !== undefined) {
      current = Number(zcardRes[1]);
    } else {
      // fallback: fetch explicitly
      current = Number(await redis.zcard(redisKey));
    }

    const remaining = Math.max(0, max - current);

    // Compute reset time: find the earliest timestamp in the sorted set
    let reset = windowMs;
    try {
      const earliest = await redis.zrange(redisKey, 0, 0, 'WITHSCORES');
      // earliest could be [member, score] or ['member1','member2'] depending on client
      let score = null;
      if (Array.isArray(earliest) && earliest.length >= 2) {
        score = Number(earliest[1]);
      } else if (Array.isArray(earliest) && earliest.length === 1) {
        score = Number(earliest[0]);
      }
      if (!Number.isNaN(score)) {
        reset = Math.max(0, Math.ceil((score + windowMs - now)));
      }
    } catch (e) {
      // ignore, fallback to window
    }

    return { current, remaining, reset };
  }

  return { increment };
}

module.exports = createRateLimiter;

```
