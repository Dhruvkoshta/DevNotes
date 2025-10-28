-- Expand exams schema and add related tables; extend logs

-- Drop deprecated column and add new columns to exams
ALTER TABLE "exams" DROP COLUMN IF EXISTS "exam_link";
ALTER TABLE "exams" ADD COLUMN IF NOT EXISTS "status" text DEFAULT 'draft' NOT NULL;
ALTER TABLE "exams" ADD COLUMN IF NOT EXISTS "course" text;
ALTER TABLE "exams" ADD COLUMN IF NOT EXISTS "meta" jsonb;
ALTER TABLE "exams" ADD COLUMN IF NOT EXISTS "proctoring" jsonb;
ALTER TABLE "exams" ADD COLUMN IF NOT EXISTS "updated_at" timestamp DEFAULT now() NOT NULL;

-- Extend logs with attempt reference
ALTER TABLE "logs" ADD COLUMN IF NOT EXISTS "attempt_id" integer;

-- Questions and options
CREATE TABLE IF NOT EXISTS "exam_questions" (
	"id" serial PRIMARY KEY NOT NULL,
	"exam_id" integer NOT NULL,
	"order" integer NOT NULL,
	"type" text NOT NULL,
	"title" text NOT NULL,
	"prompt" text NOT NULL,
	"points" integer DEFAULT 0 NOT NULL,
	"created_at" timestamp DEFAULT now() NOT NULL
);

CREATE TABLE IF NOT EXISTS "exam_mcq_options" (
	"id" serial PRIMARY KEY NOT NULL,
	"question_id" integer NOT NULL,
	"text" text NOT NULL,
	"is_correct" boolean DEFAULT false NOT NULL
);

-- Coding metadata
CREATE TABLE IF NOT EXISTS "exam_coding_meta" (
	"question_id" integer PRIMARY KEY NOT NULL,
	"language" text NOT NULL,
	"starter_code" text
);

CREATE TABLE IF NOT EXISTS "exam_coding_tests" (
	"id" serial PRIMARY KEY NOT NULL,
	"question_id" integer NOT NULL,
	"input" text NOT NULL,
	"expected_output" text NOT NULL
);

-- Attempts and answers
CREATE TABLE IF NOT EXISTS "exam_attempts" (
	"id" serial PRIMARY KEY NOT NULL,
	"exam_id" integer NOT NULL,
	"student_email" text NOT NULL,
	"student_name" text,
	"status" text DEFAULT 'active' NOT NULL,
	"attempt_no" integer DEFAULT 1 NOT NULL,
	"started_at" timestamp DEFAULT now() NOT NULL,
	"submitted_at" timestamp
);

CREATE TABLE IF NOT EXISTS "exam_answers" (
	"id" serial PRIMARY KEY NOT NULL,
	"attempt_id" integer NOT NULL,
	"question_id" integer NOT NULL,
	"payload" jsonb,
	"auto_score" integer,
	"manual_score" integer,
	"total_score" integer
);

-- Verification artifacts per attempt
CREATE TABLE IF NOT EXISTS "exam_verification" (
	"id" serial PRIMARY KEY NOT NULL,
	"attempt_id" integer NOT NULL,
	"rules_agreed" boolean DEFAULT false NOT NULL,
	"webcam_ok" boolean DEFAULT false,
	"mic_ok" boolean DEFAULT false,
	"face_photo_url" text,
	"room_scan_url" text,
	"completed_at" timestamp DEFAULT now()
);
