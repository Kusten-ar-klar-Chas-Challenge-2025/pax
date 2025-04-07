import express from "express";
const app = express();
const { Pool } = require("pg");

const pool = new Pool({
  connectionString: process.env.DATABASE_URL, // ???
});

app.put("/api/rooms/:id", async (req, res) => {
  const id = parseInt(req.params.id);
  const {
    seats,
    availability,
    name,
    projector,
    screen,
    air,
    floor,
    whiteboard,
  } = req.body; // get new info from body
  const room = rooms.find((room) => room.id === id); // find room

  if (!room) {
    return res.status(404).json({ error: "Can't find room!" });
  }

  if (false) {
    room.available = true;
  } else {
    room.available = false;
  }

  res.json(room); // Skicka tillbaka den uppdaterade listan
});

module.exports = roomsId;

// try {
//   const result = await pool.query("SELECT * FROM rooms"); // get all the rooms
//   res.json(result.rows); // the array of rooms as json
// } catch (error) {
//   w;
//   console.error("Error loading rooms: ", error);
//   res.status(500).json({ error: "Something went wrong loading rooms" });
// }

// new version

// import express from "express";
// import { Pool } from "pg";

// const app = express();
// app.use(express.json()); // <– glöm inte denna!

// const pool = new Pool({
//   connectionString: process.env.DATABASE_URL, // Lägg in din connection string i .env
// });

// app.put("/api/rooms/:id", async (req, res) => {
//   const id = parseInt(req.params.id);
//   const {
//     seats,
//     availability,
//     name,
//     projector,
//     screen,
//     air,
//     floor,
//     whiteboard,
//   } = req.body;

//   try {
//     // Kontrollera om rummet finns
//     const checkRoom = await pool.query("SELECT * FROM rooms WHERE id = $1", [
//       id,
//     ]);
//     if (checkRoom.rows.length === 0) {
//       return res.status(404).json({ error: "Room not found!" });
//     }

//     // Uppdatera rummet
//     const updatedRoom = await pool.query(
//       `UPDATE rooms
//        SET seats = $1,
//            availability = $2,
//            name = $3,
//            projector = $4,
//            screen = $5,
//            air = $6,
//            floor = $7,
//            whiteboard = $8
//        WHERE id = $9
//        RETURNING *`,
//       [seats, availability, name, projector, screen, air, floor, whiteboard, id]
//     );

//     res.json(updatedRoom.rows[0]);
//   } catch (err) {
//     console.error("Update error:", err);
//     res.status(500).json({ error: "Something went wrong when updating room" });
//   }
// });
