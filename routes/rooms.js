const express = require('express');
const app = express();
const port = 3000;

// Middleware to parse JSON
app.use(express.json());

// Mock data for rooms
let rooms = [
  {
    id: 1,
    seats: 20,
    availability: true,
    name: 'Conference Room A',
    projector: true,
    screen: true,
    'air-quality': 85
  }
];

// GET all rooms
app.get('/rooms', (req, res) => {
  res.json(rooms);
});

// GET a room by ID
app.get('/rooms/:id', (req, res) => {
  const room = rooms.find(r => r.id === parseInt(req.params.id));
  if (!room) return res.status(404).send('Room not found');
  res.json(room);
});

// POST create a new room
app.post('/rooms', (req, res) => {
  const newRoom = {
    id: rooms.length + 1,
    ...req.body
  };
  rooms.push(newRoom);
  res.status(201).json(newRoom);
});

// PUT update a room
app.put('/rooms/:id', (req, res) => {
  const room = rooms.find(r => r.id === parseInt(req.params.id));
  if (!room) return res.status(404).send('Room not found');

  Object.assign(room, req.body);
  res.json(room);
});

// DELETE a room
app.delete('/rooms/:id', (req, res) => {
  rooms = rooms.filter(r => r.id !== parseInt(req.params.id));
  res.send('Room deleted');
});

// Start the server
app.listen(port, () => {
  console.log(`Server running at http://localhost:${port}`);
});

