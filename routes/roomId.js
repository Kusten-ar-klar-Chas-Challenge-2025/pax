import express from "express";
const app = express();
const PORT = 3000;

app.use(express.json()); // Så vi kan använda req.body

// Mocked list with rooms
let rooms = [
  {
    id: 1,
    name: "Konferens A",
    seats: 6,
    availability: true,
    projector: true,
    screen: false,
    air: true,
    floor: 1,
    whiteboard: true,
  },
  {
    id: 2,
    name: "Mötesrum B",
    seats: 4,
    availability: false,
    projector: false,
    screen: false,
    air: true,
    floor: 2,
    whiteboard: true,
  },
];

// Get all rooms
app.get("/api/rooms", (req, res) => {
  res.json(rooms);
});

// Update a room
app.put("/api/rooms/:id", (req, res) => {
  const id = parseInt(req.params.id);
  const index = rooms.findIndex((room) => room.id === id);

  if (index === -1) {
    return res.status(404).json({ error: "Room not found!" });
  }

  const {
    seats,
    availability,
    name,
    projector,
    screen,
    air,
    floor,
    whiteboard,
  } = req.body;

  // Update room
  rooms[index] = {
    ...rooms[index],
    seats,
    availability,
    name,
    projector,
    screen,
    air,
    floor,
    whiteboard,
  };

  res.json(rooms[index]);
});

// Add new room
app.post("/api/rooms", (req, res) => {
  const newRoom = { id: Date.now(), ...req.body };
  rooms.push(newRoom);
  res.status(201).json(newRoom);
});

app.listen(PORT, () => {
  console.log(`✅ Mock-server körs på http://localhost:${PORT}`);
});

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
