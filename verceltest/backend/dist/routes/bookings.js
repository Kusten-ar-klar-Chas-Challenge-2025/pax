"use strict";
var __importDefault = (this && this.__importDefault) || function (mod) {
    return (mod && mod.__esModule) ? mod : { "default": mod };
};
Object.defineProperty(exports, "__esModule", { value: true });
const express_1 = __importDefault(require("express"));
const db_1 = __importDefault(require("../db"));
const asyncHandler_1 = __importDefault(require("../middlewares/asyncHandler"));
const router = express_1.default.Router();
// GET all bookings with room name and user email
router.get("/", (0, asyncHandler_1.default)(async (_req, res) => {
    const result = await db_1.default.query(`
      SELECT 
        b.id, b.start_time, b.end_time,
        r.name AS room_name,
        u.email AS user_email
      FROM bookings b
      JOIN rooms r ON b.room_id = r.id
      JOIN users u ON b.user_id = u.id
    `);
    res.status(200).json(result.rows);
}));
// GET a single booking by ID
router.get("/:id", (0, asyncHandler_1.default)(async (req, res) => {
    const { id } = req.params;
    const result = await db_1.default.query("SELECT * FROM bookings WHERE id = $1", [
        id,
    ]);
    if (result.rows.length === 0) {
        return res.status(404).json({ error: "Booking not found" });
    }
    res.status(200).json(result.rows[0]);
}));
// POST a new booking with foreign key validation
router.post("/", (0, asyncHandler_1.default)(async (req, res) => {
    const { room_id, user_id, start_time, end_time } = req.body;
    const roomCheck = await db_1.default.query("SELECT id FROM rooms WHERE id = $1", [
        room_id,
    ]);
    if (roomCheck.rows.length === 0) {
        return res.status(400).json({ error: "Invalid room_id" });
    }
    const userCheck = await db_1.default.query("SELECT id FROM users WHERE id = $1", [
        user_id,
    ]);
    if (userCheck.rows.length === 0) {
        return res.status(400).json({ error: "Invalid user_id" });
    }
    const result = await db_1.default.query("INSERT INTO bookings (room_id, user_id, start_time, end_time) VALUES ($1, $2, $3, $4) RETURNING *", [room_id, user_id, start_time, end_time]);
    res.status(201).json(result.rows[0]);
}));
// PUT update booking
router.put("/:id", (0, asyncHandler_1.default)(async (req, res) => {
    const { id } = req.params;
    const { room_id, user_id, start_time, end_time } = req.body;
    const roomCheck = await db_1.default.query("SELECT id FROM rooms WHERE id = $1", [
        room_id,
    ]);
    if (roomCheck.rows.length === 0) {
        return res.status(400).json({ error: "Invalid room_id" });
    }
    const userCheck = await db_1.default.query("SELECT id FROM users WHERE id = $1", [
        user_id,
    ]);
    if (userCheck.rows.length === 0) {
        return res.status(400).json({ error: "Invalid user_id" });
    }
    const result = await db_1.default.query("UPDATE bookings SET room_id = $1, user_id = $2, start_time = $3, end_time = $4 WHERE id = $5 RETURNING *", [room_id, user_id, start_time, end_time, id]);
    if (result.rows.length === 0) {
        return res.status(404).json({ error: "Booking not found" });
    }
    res.status(200).json(result.rows[0]);
}));
// DELETE a booking
router.delete("/:id", (0, asyncHandler_1.default)(async (req, res) => {
    const { id } = req.params;
    const result = await db_1.default.query("DELETE FROM bookings WHERE id = $1 RETURNING *", [id]);
    if (result.rows.length === 0) {
        return res.status(404).json({ error: "Booking not found" });
    }
    res.status(204).send();
}));
exports.default = router;
