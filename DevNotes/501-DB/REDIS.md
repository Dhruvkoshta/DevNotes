## What is Redis?

Redis is an in-memory database that is significantly faster than traditional disk-based databases. It can be used for various purposes, including caching, message queues, and coordination.

---

## Why Redis?

Imagine a traditional database being pinged for every query, leading to high latency, increased load on the database, and financial costs. Redis helps solve this problem by creating a database in memory to handle repeated calls for the same data, offloading the traditional database.

---

## Where and for What Should I Use Redis?

You can determine if Redis is the right tool for a scenario by asking a few key questions:

- **Am I fetching the same data repeatedly?** Redis can cache this data.
    
- **Do I need very fast read/write speeds, in microseconds?** Redis operations are much faster than traditional databases.
    
- **Do I need a queue or a pub/sub system for coordination?** Redis Lists can be used for this.
    
- **Is it okay if this data is temporary or can be reconstructed?** 8
    

### Why Do I Need a Queue?

A queue is useful when tasks arrive faster than you can process them, or when you need controlled, background processing. For example, if 1000 customers place a food order at once, you can't process them all simultaneously. Instead, you can add them to a queue to be processed on a "first come, first serve" basis. This allows customers to wait while their orders are processed.

### What is Coordination?

Coordination ensures that systems or workers don't clash with each other and instead work together peacefully. For example, if you have 10 chefs, you want to make sure only one chef takes a specific pizza order.

---

## How to Use Redis

### Caching Repeated Data

A common use case for Redis is caching. For instance, in a news app that shows the top 10 trending stories, many users may open the app at once, and a database call for each user could overload the database. A better solution is to cache the trending news in Redis so that subsequent requests hit the cache instead of the database16. The cache can be refreshed when new news becomes available.

A common workflow is to first check if the data exists in Redis. If it does, return the cached data19. If not, fetch the data from the source database, store it in Redis with an expiration time, and then return the data.

### Redis Methods

- **get**: Retrieves the value of a key.
    
- **setex**: Sets a key to a value and specifies a time-to-live (expiration time)22.
    
- **set**: Sets a key to a value with no expiration.
    
- **getset**: Sets a key to a new value and returns the old value.
    
- **del**: Deletes one or more keys.
    
- **exists**: Checks if a key exists.
    
- **expire**: Sets a timeout on an existing key in seconds.
    
- **ttl**: Returns the remaining time-to-live for a key in seconds.
    
- **persist**: Removes the expiration from a key.
    
- **incr**: Increments the integer value of a key by .
    
- **decr**: Decrements the integer value of a key by .
    
- **incrby**: Increments the integer value of a key by a specified amount.
    

### Using Redis for Queues

When orders arrive faster than you can process them, you can use a queue. You can push tasks into a queue using

`lpush` and have a background worker pull them using `brpop`. This approach frees up your web app to handle other tasks while the worker processes the queue in the background. This is good for apps that perform background tasks like image processing or other operations that shouldn't block the user36.

---

## Redis Data Types

Redis is a key-value store where every piece of data requires a key. You can store anything in Redis as long as you model it using one of its data types.

| Redis Data Type       | Stored As (Key → Value)             | What It Looks Like                                      | Common Use Cases                                 |
| --------------------- | ----------------------------------- | ------------------------------------------------------- | ------------------------------------------------ |
| **String**            | key → string (text/number)          | `SET user:1 "Ajeet"`                                    | Caching values, counters, feature flags, tokens  |
| **List**              | key → ordered list                  | `LPUSH queue "task1"` → `["task1", "task2"]`            | Message queues, task lists, chat history         |
| **Set**               | key → unordered unique items        | `SADD online_users 101 102` → `{101, 102}`              | Unique visitors, tags, "likes"                   |
| **Sorted Set (ZSET)** | key → set + scores (sorted)         | `ZADD leaderboard 500 "player1"` → `[("player1", 500)]` | Leaderboards, rankings, priority queues          |
| **Hash**              | key → field-value pairs (mini JSON) | `HSET user:1 name "Ajeet" age 25`                       | User profiles, product info, config maps         |
| **Stream**            | key → append-only log of events     | `XADD mystream * message "Hi"`                          | Event sourcing, chat feeds, logs                 |
| **Bitmap**            | key → bit array                     | `SETBIT logins 51`                                      | Track daily activity (yes/no), flags             |
| **HyperLogLog**       | key → probabilistic counter         | `PFADD unique_users user123`                            | Count unique items (e.g., unique visitors)       |
| **Geospatial**        | key → geo-coordinates (lat, lon)    | `GEOADD locations 77.1 28.6 "Delhi"`                    | Find nearby places, "who's closest" lookups      |
