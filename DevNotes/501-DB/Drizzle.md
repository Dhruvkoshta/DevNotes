
> [!NOTE] DEF
> Drizzle is a **headless TypeScript ORM** (Object-Relational Mapper) that prioritizes performance, type safety, and a familiar SQL-like syntax. Unlike traditional ORMs that might hide SQL from you, Drizzle empowers you to write queries that closely mirror raw SQL, but with the full safety of TypeScript.

**Key Features:**

- **Type Safety:** If your query is valid in TypeScript, it's a valid SQL query. This catches errors at compile-time, not runtime.
    
- **Performance:** Drizzle is extremely lightweight and boasts near-native SQL driver performance.
    
- **SQL-like Syntax:** You don't need to learn a complex new query language. If you know SQL, you'll feel right at home.
    
- **Schema as Code:** Your database schema is defined directly in your TypeScript files, enabling version control and a single source of truth.
    

---

### 💻 Approach 1: Codebase-First (The Standard Workflow)

In this approach, your **TypeScript schema files are the single source of truth**. You define your database structure in code, and Drizzle's tooling generates the necessary SQL to make your database match that code.

#### Workflow Steps

**1. Define or Modify Your Schema**

You start by defining your tables in a `.ts` file (e.g., `src/db/schema.ts`). Drizzle provides functions like `pgTable`, `mysqlTable`, or `sqliteTable` to define tables and various data types for columns.


```ts
// src/db/schema.ts
import { relations } from 'drizzle-orm';
import { pgTable, serial, text, varchar } from 'drizzle-orm/pg-core';

export const users = pgTable('users', {
  id: serial('id').primaryKey(),
  fullName: text('full_name'),
  email: varchar('email', { length: 256 }).notNull().unique(),
});

export const posts = pgTable('posts', {
  id: serial('id').primaryKey(),
  content: text('content').notNull(),
  authorId: serial('author_id').references(() => users.id),
});

// Optional: Define relationships for type-safe joins
export const usersRelations = relations(users, ({ many }) => ({
  posts: many(posts),
}));
```

**2. Generate Migration Files ⚙️**

After you create or change your schema, you use the **Drizzle Kit** command-line tool to generate an SQL migration file.

Bash

```
npx drizzle-kit generate:pg
```

This command inspects your TypeScript schema, compares it to its last known state, and generates a new SQL file in your migrations folder. For example, if you just added the `posts` table, the generated SQL file might look like this:

SQL

```sql
-- migrations/0001_initial_posts.sql
CREATE TABLE IF NOT EXISTS "posts" (
	"id" serial PRIMARY KEY NOT NULL,
	"content" text NOT NULL,
	"author_id" serial
);

DO $$ BEGIN
 ALTER TABLE "posts" ADD CONSTRAINT "posts_author_id_users_id_fk" FOREIGN KEY ("author_id") REFERENCES "public"."users"("id") ON DELETE no action ON UPDATE no action;
EXCEPTION
 WHEN duplicate_object THEN null;
END $$;
```

**3. Apply the Migration**

Finally, you run the migration against your database. Drizzle Kit provides a command for this as well.

Bash

```
npx drizzle-kit migrate:pg
```

This command connects to your database, checks which migrations haven't been applied yet, and executes them in order.

#### ✅ Pros & ❌ Cons

- **✅ Pros:**
    
    - **Single Source of Truth:** Your Git repository contains the entire history and definition of your database.
        
    - **Full Type Safety:** Your application code is always in sync with your schema.
        
    - **Collaboration:** Easy for teams to collaborate on schema changes through pull requests.
        
- **❌ Cons:**
    
    - Requires a disciplined workflow; manual changes to the database can cause drift.
        

---

### 📦 Approach 2: Database-First (Introspection)

In this approach, your **existing database is the single source of truth**. This is perfect for projects with a pre-existing database or where a Database Administrator (DBA) manages the schema directly.

#### Workflow Steps

**1. Point Drizzle Kit to Your Database**

You need an existing database with tables and columns already defined.

**2. Introspect the Database 🔍**

You run the Drizzle Kit `introspect` command:

Bash

```
npx drizzle-kit introspect:pg
```

Drizzle Kit connects to your database, reads its entire schema (tables, columns, types, relations, etc.), and **generates the corresponding TypeScript schema files for you**.

The output will be a complete `schema.ts` file that perfectly mirrors your database's current state, ready for you to use in your application for type-safe querying.

#### ✅ Pros & ❌ Cons

- **✅ Pros:**
    
    - **Perfect for Existing Projects:** The easiest way to adopt Drizzle without rebuilding your schema from scratch.
        
    - **DBA-Friendly:** Allows database schemas to be managed by dedicated tools or teams while developers still get type-safe clients.
        
- **❌ Cons:**
    
    - You lose the "single source of truth" in your codebase. Schema changes must be made in the database first, then re-introspected.
        

---

### In-Depth: Migration & Generation Techniques

Drizzle Kit offers two primary ways to sync your schema with your database, which are crucial to understand.

#### `drizzle-kit push` (For Prototyping)

The `push` command is a fast, simple way to make your database schema match your TypeScript schema.

Bash

```
npx drizzle-kit push:pg
```

- **How it Works:** It directly compares your code schema with the database and generates and applies the necessary SQL `ALTER` statements to sync them. It **does not create migration files**.
    
- **When to Use It:** **Only for local development and prototyping.** It's a destructive tool, meaning it can drop columns or data if you're not careful. It's great for quickly iterating on your schema during early development.
    
- **⚠️ Warning:** **Never use `push` in a production environment.**
    

#### `drizzle-kit migrate` (For Production)

The `migrate` command is the production-safe method for applying schema changes.

Bash

```
npx drizzle-kit migrate:pg
```

- **How it Works:** It relies on the migration files you create with `drizzle-kit generate`. It keeps track of which migrations have already been applied (in a special `__drizzle_migrations` table) and only runs the new ones.
    
- **When to Use It:** For all environments other than initial local development (Staging, Production). It provides a reliable, repeatable, and historical log of every schema change.
    

#### Drizzle Studio: Visual Generation

Drizzle Kit also includes a powerful utility called **Drizzle Studio**, a GUI for your database.

Bash

```
npx drizzle-kit studio
```

This command opens a browser window with a clean interface to view, add, and edit data in your database. It's an excellent tool for debugging and visualizing your schema without needing a heavy, external database client.