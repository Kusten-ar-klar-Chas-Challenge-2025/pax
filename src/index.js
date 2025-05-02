import express from "express";
import dotenv from "dotenv";
import pool from "./db.js";
import swaggerUi from "swagger-ui-express";
import swaggerSpec from "./swagger/swagger.js";

console.log("index.js is running");

dotenv.config();

const app = express();

const port = process.env.PORT || 13000;

app.use(express.json()); // for parsing application/json

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
    // Vänta 5 sekunder för att säkerställa att DB är redo
    await new Promise((resolve) => setTimeout(resolve, 5000));
    // SQL-query för att skapa tabellen för rooms
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

    // SQL-query för att skapa tabellen för users
    await pool.query(`
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
  } catch (err) {
    console.error("Error during setup:", err);
    console.error("Full error:", err);
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
