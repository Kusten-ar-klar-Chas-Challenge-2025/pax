const express = require("express");
const app = express();
const { Pool } = require("pg");

const pool = new Pool({
  connectionString: process.env.DATABASE_URL, // ???
});

app.get("/api/rooms", async (req, res) => {
  try {
    const result = await pool.query("SELECT * FROM rooms"); // get all the rooms
    res.json(result.rows); // the array of rooms as json
  } catch (error) {
    console.error("Error loading rooms: ", error);
    res.status(500).json({ error: "Something went wrong loading rooms" });
  }
});

module.exports = rooms;
