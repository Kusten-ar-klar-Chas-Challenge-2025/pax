"use strict";
var __importDefault = (this && this.__importDefault) || function (mod) {
    return (mod && mod.__esModule) ? mod : { "default": mod };
};
Object.defineProperty(exports, "__esModule", { value: true });
const express_1 = __importDefault(require("express"));
const dotenv_1 = __importDefault(require("dotenv"));
const db_1 = __importDefault(require("./db"));
const swagger_ui_express_1 = __importDefault(require("swagger-ui-express"));
const swagger_export_json_1 = __importDefault(require("./swagger/swagger-export.json"));
const cors_1 = __importDefault(require("cors"));
const rateLimiter_1 = __importDefault(require("./middlewares/rateLimiter"));
const errorHandler_1 = __importDefault(require("./middlewares/errorHandler"));
console.log("Index.ts is running");
dotenv_1.default.config();
const app = (0, express_1.default)();
const port = process.env.PORT || 3000;
app.use(express_1.default.json()); // for parsing application/json
app.use(rateLimiter_1.default); // allows limiter on all routes
// allow requests from frontend (localhost:5173)
app.use((0, cors_1.default)({
    origin: "http://localhost:5173",
    credentials: true,
}));
// routes
const users_1 = __importDefault(require("./routes/users"));
const rooms_1 = __importDefault(require("./routes/rooms"));
const bookings_1 = __importDefault(require("./routes/bookings"));
app.use("/users", users_1.default);
app.use("/rooms", rooms_1.default);
app.use("/bookings", bookings_1.default);
app.use(errorHandler_1.default);
// Swagger route
app.use("/api-docs", swagger_ui_express_1.default.serve, swagger_ui_express_1.default.setup(swagger_export_json_1.default));
app.get("/", (req, res) => res.send("Express on Vercel"));
app.get("/setup", async (_req, res) => {
    console.log("setup starting");
    try {
        // Vänta 5 sekunder för att säkerställa att DB är redo
        await new Promise((resolve) => setTimeout(resolve, 5000));
        // SQL-query för att skapa tabellen för rooms
        await db_1.default.query(`
      CREATE TABLE IF NOT EXISTS rooms (
        id SERIAL PRIMARY KEY,
        name VARCHAR(100) NOT NULL,
        description TEXT,
        available BOOLEAN DEFAULT TRUE,
        air_quality INT DEFAULT 0,
        screen BOOLEAN DEFAULT FALSE,
        floor INT DEFAULT 0,
        chairs INT DEFAULT 0,
        whiteboard BOOLEAN DEFAULT FALSE,
        projector BOOLEAN DEFAULT FALSE
      )
    `);
        // SQL-query för att skapa tabellen för users
        await db_1.default.query(`
      CREATE TABLE IF NOT EXISTS users (
        id SERIAL PRIMARY KEY,
        firstname VARCHAR(100) NOT NULL,
        lastname VARCHAR(100) NOT NULL,
        email VARCHAR(150) UNIQUE NOT NULL,
        password TEXT NOT NULL,
        role VARCHAR(50) DEFAULT 'user'
      )
    `);
        console.log("Table setup completed.");
        res.status(200).send("Setup completed");
    }
    catch (err) {
        console.error("Error during setup:", err);
        console.error("Full error:", err);
        res.status(500).send("Error setting up database");
    }
});
app.listen(port, () => {
    console.log(`Server listening on port ${port}`);
    console.log(`Swagger docs: http://localhost:${port}/api-docs`);
});
//only in dev mode
if (process.env.NODE_ENV !== "production") {
    console.log("DB_USER:", process.env.DB_USER);
    console.log("DB_HOST:", process.env.DB_HOST);
    console.log("DB_NAME:", process.env.DB_NAME);
    console.log("DB_PASSWORD:", process.env.DB_PASSWORD);
    console.log("DB_PORT:", process.env.DB_PORT);
}
exports.default = app;
