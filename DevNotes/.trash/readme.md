# My C-Powered Web Server: A Fun Dive into Proxies & Caching!

Hey there! So, this project is basically my take on building a web server in C, with a special focus on making it work as a proxy. I even borrowed some cool HTTP parsing ideas from this awesome Proxy Server repo.
Let's Break It Down: What's This Project All About?

### How Our Proxy Server Works (in a Nutshell)

**Think of our proxy as a friendly middleman. When you ask for a webpage, it steps in, grabs your request, sends it off to the real server, and then brings the server's reply right back to you. Simple, right?**

(Imagine a cool diagram here showing the flow, like the one in the original docs!)

### Why Multi-threading? And Why Semaphores?

To handle all your requests without breaking a sweat, we've made our server multi-threaded. Now, you might think of "condition variables" for this, but we went with Semaphores. **Why**?

    pthread_join() can be a bit finicky, needing to know exactly which thread to wait for.

    Semaphores (sem_wait() and sem_post()) are way more chill – they don't need any specific thread info, making our concurrency setup smoother and more flexible.

### Why Did I Even Make This? (And What's So Cool About Proxies?)

**This project isn't just code; it's a learning adventure! Here's what you'll get out of it, and why proxies are actually pretty neat:**

    Get the "Aha!" Moment:

        Finally understand the secret life of your web requests.

        See how a server handles a crowd of clients all at once.

        Figure out why "locking" is so important when things are happening concurrently.

        Dive into the magic of caching – how browsers use it to make your life easier.

    Proxy Power-Ups:

        Speed Demon: Proxies can seriously speed up how fast you get content, and they ease the load on busy servers, especially with smart caching.

        Gatekeeper: Want to block certain websites? A proxy can totally do that!

        Secret Agent: A well-set-up proxy can hide your real IP address, giving you more privacy. Plus, they can even encrypt your requests to keep snoopers away!

### The Techy Bits: OS Goodies We Used

We tapped into some core operating system concepts to make this happen:

    Threading: For handling multiple users at the same time.

    Locks: To keep things orderly and prevent chaos when threads share stuff.

    Semaphores: Our go-to for keeping threads in sync.

    Cache: We built a smart cache using the LRU (Least Recently Used) algorithm – it's like a memory for frequently visited sites, making them load faster next time!

What's Not So Perfect (Yet!)

Every project has its quirks, and mine's no different:

    Tricky URLs: If a single website address secretly triggers a bunch of other requests (like for images or scripts), our cache might save each piece separately. This can sometimes mean a website doesn't load fully from the cache, which is a bummer.

    Fixed Cache Size: Our cache elements have a set size. So, if you try to cache a super-duper-huge website, it might not all fit.

How We Can Make It Even Better!

This project is just the beginning! Here are some cool ways we could expand it:

    Go Parallel with Multiprocessing: Right now, we're multi-threaded. Switching to multiprocessing could make things even faster by truly running tasks in parallel.

    Smarter Content Blocking: We could add more advanced rules to decide which websites are allowed and which aren't.

    Handle POST Requests: Currently, it mostly deals with simple "GET" requests. Adding support for "POST" (like when you submit a form) would make it way more versatile.

Ready to Play? Here's How to Run It!

Getting this proxy server running is a breeze. Just follow these steps:

    Grab the code:

    git clone https://github.com/Dhruvkoshta/Web-Server-in-C.git


    Jump into the folder:

    cd Web-Server-in-C.


    Compile everything:

    make all


    Fire up the server!

    ./proxy <port no.>


    (Pick any port you like, e.g., ./proxy 8080)

Once it's running, open your favorite browser and head to:

http://localhost:port/https://www.cs.princeton.edu/

A Couple of Quick Pointers:

    This code is built for Linux machines, so keep that in mind!

    To really see the cache in action, make sure you turn off your browser's own cache first.

    If you want to run the proxy without caching, just tweak the Makefile to use proxy_server_without_cache.c instead of proxy_server_with_cache.c.

See It in Action: The Cache Demo!

Watch the magic happen with this quick demo:

(Imagine a cool screenshot here showing the cache messages!)

    First Time's the Charm (or Miss!): When you visit a site for the very first time, you'll see a message like url not found – that means it's a cache miss.

    Second Time's the Win!: But if you visit that same site again, you'll see Data is retrieved from the cache. Boom! Instant gratification, thanks to caching.

Happy coding