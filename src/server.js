const express = require('express');
const app = express();
const PORT = 3000;

app.use(express.json());

// Dummy-databas
let rooms = [
  { id: 1, name: "St Knut", status: "available" },
  { id: 2, name: "Möllevång", status: "booked" },
  { id: 3, name: "Hansa", status: "available" }
];

// GET - Hämta alla rum
app.get('/Rooms', (req, res) => {
  res.json(rooms);
});

// DELETE - Ta bort rum med specifikt ID
app.delete('/Rooms/delete/:id', (req, res) => {
  const id = parseInt(req.params.id);
  const index = rooms.findIndex(room => room.id === id);

  if (index === -1) {
    return res.status(404).json({ error: "Rum ej hittat" });
  }

  const deletedRoom = rooms.splice(index, 1);
  res.json({ message: "Rummet har tagits bort", deletedRoom });
});

app.listen(PORT, () => {
  console.log(`Servern körs på http://localhost:${PORT}`);
});