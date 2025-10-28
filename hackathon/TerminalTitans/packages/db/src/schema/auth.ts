import { pgTable, text, timestamp, boolean, serial, integer, jsonb } from "drizzle-orm/pg-core";


export const user = pgTable("user", {
	id: text("id").primaryKey(),
	name: text("name").notNull(),
	email: text("email").notNull().unique(),
	emailVerified: boolean("email_verified").notNull(),
	image: text("image"),
	userType: text("user_type").notNull().default("student"),
	createdAt: timestamp("created_at").notNull(),
	updatedAt: timestamp("updated_at").notNull(),
});

export const session = pgTable("session", {
	id: text("id").primaryKey(),
	expiresAt: timestamp("expires_at").notNull(),
	token: text("token").notNull().unique(),
	createdAt: timestamp("created_at").notNull(),
	updatedAt: timestamp("updated_at").notNull(),
	ipAddress: text("ip_address"),
	userAgent: text("user_agent"),
	userId: text("user_id")
		.notNull()
		.references(() => user.id, { onDelete: "cascade" }),
});

export const account = pgTable("account", {
	id: text("id").primaryKey(),
	accountId: text("account_id").notNull(),
	providerId: text("provider_id").notNull(),
	userId: text("user_id")
		.notNull()
		.references(() => user.id, { onDelete: "cascade" }),
	accessToken: text("access_token"),
	refreshToken: text("refresh_token"),
	idToken: text("id_token"),
	accessTokenExpiresAt: timestamp("access_token_expires_at"),
	refreshTokenExpiresAt: timestamp("refresh_token_expires_at"),
	scope: text("scope"),
	password: text("password"),
	createdAt: timestamp("created_at").notNull(),
	updatedAt: timestamp("updated_at").notNull(),
});

export const verification = pgTable("verification", {
	id: text("id").primaryKey(),
	identifier: text("identifier").notNull(),
	value: text("value").notNull(),
	expiresAt: timestamp("expires_at").notNull(),
	createdAt: timestamp("created_at"),
	updatedAt: timestamp("updated_at"),
});

export const logs = pgTable("logs", {
	id: serial("id").primaryKey(),
	examCode: text("exam_code").notNull(),
	studentName: text("student_name").notNull(),
	studentEmail: text("student_email").notNull(),
	attemptId: integer("attempt_id"),

	tabChangeCount: integer("tab_change_count").notNull().default(0),
	keyPressCount: integer("key_press_count").notNull().default(0),
	mobileFound: boolean("mobile_found").notNull().default(false),
	prohibitedObjectFound: boolean("prohibited_object_found").default(false),
	faceNotVisible: boolean("face_not_visible").notNull().default(false),
	multipleFacesFound: boolean("multiple_faces_found").notNull().default(false),
	eyesOffScreen: boolean("eyes_off_screen").notNull().default(false),
	focusScore: integer("focus_score").default(100),
	focusStatus: text("focus_status").default("focused"),
	createdAt: timestamp("created_at").notNull().defaultNow(),
});


export const exams = pgTable("exams", {
	id: serial("id").primaryKey(),
	name: text("name").notNull(),
	profEmail: text("prof_email").notNull(),
	dateTimeStart: timestamp("date_time_start").notNull(),
	duration: integer("duration").notNull(),
	examCode: text("exam_code").notNull(),
	status: text("status").notNull().default("draft"),
	course: text("course"),
	meta: jsonb("meta"),
	proctoring: jsonb("proctoring"),
	createdAt: timestamp("created_at").notNull().defaultNow(),
	updatedAt: timestamp("updated_at").notNull().defaultNow(),
});

export const examQuestions = pgTable("exam_questions", {
	id: serial("id").primaryKey(),
	examId: integer("exam_id").notNull(),
	order: integer("order").notNull(),
	type: text("type").notNull(),
	title: text("title").notNull(),
	prompt: text("prompt").notNull(),
	points: integer("points").notNull().default(0),
	createdAt: timestamp("created_at").notNull().defaultNow(),
});

export const examMcqOptions = pgTable("exam_mcq_options", {
	id: serial("id").primaryKey(),
	questionId: integer("question_id").notNull(),
	text: text("text").notNull(),
	isCorrect: boolean("is_correct").notNull().default(false),
});

export const examCodingMeta = pgTable("exam_coding_meta", {
	questionId: integer("question_id").primaryKey(),
	language: text("language").notNull(),
	starterCode: text("starter_code"),
});

export const examCodingTests = pgTable("exam_coding_tests", {
	id: serial("id").primaryKey(),
	questionId: integer("question_id").notNull(),
	input: text("input").notNull(),
	expectedOutput: text("expected_output").notNull(),
});

export const examAttempts = pgTable("exam_attempts", {
	id: serial("id").primaryKey(),
	examId: integer("exam_id").notNull(),
	studentEmail: text("student_email").notNull(),
	studentName: text("student_name"),
	status: text("status").notNull().default("active"),
	attemptNo: integer("attempt_no").notNull().default(1),
	startedAt: timestamp("started_at").notNull().defaultNow(),
	submittedAt: timestamp("submitted_at"),
});

export const examAnswers = pgTable("exam_answers", {
	id: serial("id").primaryKey(),
	attemptId: integer("attempt_id").notNull(),
	questionId: integer("question_id").notNull(),
	payload: jsonb("payload"),
	autoScore: integer("auto_score"),
	manualScore: integer("manual_score"),
	totalScore: integer("total_score"),
});

export const examVerification = pgTable("exam_verification", {
	id: serial("id").primaryKey(),
	attemptId: integer("attempt_id").notNull(),
	rulesAgreed: boolean("rules_agreed").notNull().default(false),
	webcamOk: boolean("webcam_ok").default(false),
	micOk: boolean("mic_ok").default(false),
	facePhotoUrl: text("face_photo_url"),
	roomScanUrl: text("room_scan_url"),
	completedAt: timestamp("completed_at").defaultNow(),
});

// extend existing logs with attemptId

