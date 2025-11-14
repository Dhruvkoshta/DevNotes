
# 🧠 What Are Controllers, Services, Repositories, Middlewares, and Request Context?


---

## 🏗️ 1. Controller–Service–Repository Pattern

A foundational pattern that **separates concerns** in backend design, improving scalability, maintainability, and readability.

### ⚙️ Request Flow

**Request → Controller → Service → Repository → Database → Response**

---

### 🧩 Controllers (a.k.a. Handlers)

The **entry point** of your application logic.
It directly interacts with HTTP requests and responses.

**Responsibilities:**

1. **Binding / Deserialization:**
   Convert raw request data (e.g., JSON) into usable objects or classes.
   → Invalid input → send `400 Bad Request`.

2. **Validation:**
   Check that data meets all constraints — required fields, formats, types.

3. **Transformation:**
   Apply defaults or transformations before processing (e.g., add default pagination or sort order).

4. **Delegation:**
   Pass cleaned and validated data to the service layer.

5. **Response Formation:**
   Send back the appropriate HTTP response with status codes (200, 201, 404, 500, etc.).

**In short:**
🧾 *Controller = “Interface between client and server logic.”*

---

### 🧠 Services

Contain the **business logic** — the “***brains***” of your application.

**Characteristics:**

* Independent of HTTP or network layers.
* Can be reused by multiple controllers.

**Responsibilities:**

1. **Business Processing:** Perform computations, decisions, or actions (e.g., send notifications, process payments).
2. **Orchestration:** Combine multiple repository results to form complex outputs.
3. **Database Interaction:** Call repository methods for data fetching or updates.

**In short:**
⚙️ *Service = “**Performs business logic and coordinates data sources**.”*

---

### 🗃️ Repositories

The **data access layer** — ***interacts directly with the database***.

**Responsibilities:**

* Execute database queries or ORM operations.
* Perform single-purpose actions (like `findById`, `createUser`, `getAllBooks`).
* Return raw or structured data to the service layer.

**In short:**
🧱 Repository = “***Talks to the database; one function, one purpose***.”*

---

## 🧩 2. Middlewares

Middlewares are ***functions that process requests before reaching the controller (or after responses).***

They form a **chain** through which every request passes.

### 🧭 How Middlewares Work

Each middleware:

```js
(req, res, next) => { ... }
```

* Performs a specific task.
* Calls `next()` to continue the chain.
* Or sends a response early (e.g., error or access denied).

**Order matters!**
They are executed in the exact order they are registered.

---

### ⚒️ Common Middlewares and Their Uses

| Middleware                | Purpose                                                            |
| ------------------------- | ------------------------------------------------------------------ |
| **Logging**               | Log each request’s method, path, and timestamp.                    |
| **CORS**                  | Add `Access-Control-Allow-Origin` headers for cross-origin access. |
| **Authentication**        | Verify tokens and block unauthorized requests (`401`).             |
| **Rate Limiting**         | Prevent abuse by limiting requests per IP (`429`).                 |
| **Global Error Handling** | Catch and standardize all errors (`try/catch` alternative).        |
| **Body Parsing**          | Automatically parse JSON or form data from requests.               |

**Main Goal:**
🧩 *Avoid duplication by centralizing shared logic.*

---

## 🌐 3. Request Context

The **Request Context** is like a private, ***temporary storage attached to each request***.
It exists **only for the duration of that request**.

### 🎯 Purpose

* Pass data *downstream* through middlewares, services, and handlers.
* Avoid “tight coupling” and long parameter chains.

---

### 🧱 Common Use Cases

#### 1. Authentication Data

* Middleware authenticates a token → extracts `user_id`, `role`.
* Stores these in the request context.
* Handlers later read them to know *who* is making the request — securely and consistently.

#### 2. Distributed Tracing (Request ID)

* Middleware generates a unique `request_id` (UUID).
* Stored in context and included in every log.
* Enables tracking a single request across multiple services (useful in microservices).

#### 3. Cancellation Signals

* Context can signal when a request is cancelled or times out.
* Useful for graceful shutdowns or aborting DB/network calls if a client disconnects.

---

## 🧭 Summary Overview

| Layer               | Role                            | Should Know About HTTP? | Example Task                  |
| ------------------- | ------------------------------- | ----------------------- | ----------------------------- |
| **Controller**      | Entry point, validates & routes | ✅ Yes                   | Validate user input           |
| **Service**         | Business logic                  | ❌ No                    | Calculate invoice total       |
| **Repository**      | Data access                     | ❌ No                    | Fetch users from DB           |
| **Middleware**      | Pre-/Post-processing            | ✅ Yes                   | Check authentication token    |
| **Request Context** | Request-specific data           | ✅ Scoped                | Store `user_id`, `request_id` |

---

## 🧩 Key Takeaways

* **Controllers:** Handle requests & responses.
* **Services:** Implement the core logic.
* **Repositories:** Manage database access.
* **Middlewares:** Handle reusable request logic.
* **Request Context:** Pass request-scoped data without coupling.

This architecture ensures:
✅ High cohesion
✅ Low coupling
✅ Better testability
✅ Cleaner codebase

---
