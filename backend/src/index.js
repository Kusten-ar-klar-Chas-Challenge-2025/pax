import express from "express";
import dotenv from "dotenv";
import pool from "./db.js";
import swaggerUi from "swagger-ui-express";
import swaggerSpec from "./swagger.js";
import cors from "cors";
import rateLimit from "express-rate-limit";
import limiter from "./middlewares/rateLimiter.js";

console.log("index.js is running");

dotenv.config();

const app = express();

const port = process.env.PORT || 13000;

app.use(express.json()); // for parsing application/json

// allow requests from frontend (localhost:5173)
app.use(
  cors({
    origin: "http://localhost:5173",
    credentials: true,
  })
);

// routes
import userRoutes from "./routes/users.js";
import roomRoutes from "./routes/rooms.js";
app.use("/users", userRoutes);
app.use("/rooms", roomRoutes);

// Swagger route
app.use("/api-docs", swaggerUi.serve, swaggerUi.setup(swaggerSpec));

app.get("/setup", async (req, res) => {
  console.log("setup starting");
  try {
    await new Promise((resolve) => setTimeout(resolve, 5000));

    try {
      console.log("Creating rooms table...");
      await pool.query(`
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
      console.log("Rooms table created.");
    } catch (err) {
      console.error("Error creating rooms table:", err);
      throw err;
    }

    try {
      console.log("Creating users table...");
      await pool.query(`
        CREATE TABLE IF NOT EXISTS users (
          id SERIAL PRIMARY KEY,
          name VARCHAR(100) NOT NULL,
          surname VARCHAR(100) NOT NULL,
          email VARCHAR(150) UNIQUE NOT NULL,
          password TEXT NOT NULL,
          role VARCHAR(50) DEFAULT 'user'
        )
      `);
      console.log("Users table created.");
    } catch (err) {
      console.error("Error creating users table:", err);
      throw err;
    }

    console.log("Table setup completed.");
    res.status(200).send("Setup completed");
  } catch (err) {
    console.error("Error during setup:", err);
    res.status(500).send("Error setting up database");
  }
});

app.get("/ping", (req, res) => {
  res.send("pong!");
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

export default app; // Export for testing
