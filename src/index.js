const express = require("express");
const dotenv = require("dotenv");
const pool = require("./db");
console.log("index.js is running");
dotenv.config();
const app = express();
const port = process.env.PORT || 13000;
app.use(express.json()); // for parsing application/json
app.get("/setup", async (req, res) => {
  console.log("setup starting");
  try {
    // Vänta 5 sekunder för att säkerställa att DB är redo
    await new Promise((resolve) => setTimeout(resolve, 5000));
    // SQL-query för att skapa tabellen
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
    console.log("Table setup completed.");
    res.status(200).send("Setup completed");
  } catch (err) {
    console.error("Error during setup:", err);
    res.status(500).send("Error setting up database");
  }
});
app.post("/rooms", async (req, res) => {
  console.log("request", req.body);
  const {
    name,
    description,
    available,
    air_quality,
    screen,
    floor,
    chairs,
    whiteboard,
    projector,
  } = req.body;
  try {
    const result = await pool.query(
      "INSERT INTO rooms (name, description, available, air_quality, screen, floor, chairs, whiteboard, projector) VALUES ($1, $2, $3, $4, $5, $6, $7, $8, $9) RETURNING *",
      [
        name,
        description,
        available,
        air_quality,
        screen,
        floor,
        chairs,
        whiteboard,
        projector,
      ]
    );
    res.status(201).json(result.rows[0]);
  } catch (err) {
    console.error("Error creating room:", err); // Log error specifically here
    res.status(500).send("Error creating room");
  }
});

app.put("/rooms/:id", async (req, res) => {
  const id = req.params.id;
  const {
    name,
    description,
    available,
    air_quality,
    screen,
    floor,
    chairs,
    whiteboard,
    projector,
  } = req.body;

  try {
    // Update the room in the database
    const result = await pool.query(
      `UPDATE rooms SET 
        name = $1,
        description = $2,
        available = $3,
        air_quality = $4,
        screen = $5,
        floor = $6,
        chairs = $7,
        whiteboard = $8,
        projector = $9
      WHERE id = ${id}
      RETURNING *`,
      [
        name,
        description,
        available,
        air_quality,
        screen,
        floor,
        chairs,
        whiteboard,
        projector,
        id,
      ]
    );

    if (result.rows.length === 0) {
      return res.status(404).json({ message: "Room not found" });
    }

    res.status(200).json(result.rows[0]);
  } catch (err) {
    console.error(err);
    res.status(500).json({ message: "Internal server error" });
  }
});

app.get("/rooms", async (req, res) => {
  try {
    const data = await pool.query("SELECT * FROM rooms");
    res.status(200).json(data.rows);
  } catch (error) {
    console.error(error);
    res.status(500).send("Internal Server Error");
  }
});

// DELETE ROOMS
app.delete("/rooms/:id", async (req, res) => {
  const id = req.params.id;

  try {
    const result = await pool.query("DELETE FROM rooms WHERE id = $1 RETURNING *", [id]);

    if (result.rows.length === 0) {
      return res.status(404).json({
        message: "Room not found."
      });
    }

    res.status(200).json({
      message: "Room deleted successfully", 
      deletedRoom: result.rows[0]
    });
    
  } catch (err) {
    console.error("Error deleting room:", err);
    res.status(500).json({
      message: "Internal server error"
    });
  }

});

app.listen(port, () => {
  console.log(`Server listening on port ${port}`);
});
console.log("DB_USER:", process.env.DB_USER);
console.log("DB_HOST:", process.env.DB_HOST);
console.log("DB_NAME:", process.env.DB_NAME);
console.log("DB_PASSWORD:", process.env.DB_PASSWORD);
console.log("DB_PORT:", process.env.DB_PORT);
module.exports = app; // Export for testing
