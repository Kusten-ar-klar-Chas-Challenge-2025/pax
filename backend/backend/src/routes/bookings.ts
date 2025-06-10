import express, { Request, Response } from "express";
import pool from "../db";
import asyncHandler from "../middlewares/asyncHandler";

const router = express.Router();

interface Booking {
  id: number;
  room_id: number;
  user_id: number;
  start_time: string;
  end_time: string;
}

type CreateBookingInput = Omit<Booking, "id">;

// GET all bookings with room name and user email
router.get(
  "/",
  asyncHandler(async (_req: Request, res: Response) => {
    const result = await pool.query(`
      SELECT 
        b.id, b.start_time, b.end_time,
        r.name AS room_name,
        u.email AS user_email
      FROM bookings b
      JOIN rooms r ON b.room_id = r.id
      JOIN users u ON b.user_id = u.id
    `);

    res.status(200).json(result.rows);
  })
);

// GET a single booking by ID
router.get(
  "/:id",
  asyncHandler(async (req: Request, res: Response) => {
    const { id } = req.params;
    const result = await pool.query("SELECT * FROM bookings WHERE id = $1", [
      id,
    ]);
    if (result.rows.length === 0) {
      return res.status(404).json({ error: "Booking not found" });
    }
    res.status(200).json(result.rows[0]);
  })
);

// POST a new booking with foreign key validation
router.post(
  "/",
  asyncHandler(async (req: Request, res: Response) => {
    const { room_id, user_id, start_time, end_time }: CreateBookingInput =
      req.body;

    const roomCheck = await pool.query("SELECT id FROM rooms WHERE id = $1", [
      room_id,
    ]);
    if (roomCheck.rows.length === 0) {
      return res.status(400).json({ error: "Invalid room_id" });
    }

    const userCheck = await pool.query("SELECT id FROM users WHERE id = $1", [
      user_id,
    ]);
    if (userCheck.rows.length === 0) {
      return res.status(400).json({ error: "Invalid user_id" });
    }

    const result = await pool.query(
      "INSERT INTO bookings (room_id, user_id, start_time, end_time) VALUES ($1, $2, $3, $4) RETURNING *",
      [room_id, user_id, start_time, end_time]
    );

    res.status(201).json(result.rows[0]);
  })
);

// PUT update booking
router.put(
  "/:id",
  asyncHandler(async (req: Request, res: Response) => {
    const { id } = req.params;
    const { room_id, user_id, start_time, end_time }: CreateBookingInput =
      req.body;

    const roomCheck = await pool.query("SELECT id FROM rooms WHERE id = $1", [
      room_id,
    ]);
    if (roomCheck.rows.length === 0) {
      return res.status(400).json({ error: "Invalid room_id" });
    }

    const userCheck = await pool.query("SELECT id FROM users WHERE id = $1", [
      user_id,
    ]);
    if (userCheck.rows.length === 0) {
      return res.status(400).json({ error: "Invalid user_id" });
    }

    const result = await pool.query(
      "UPDATE bookings SET room_id = $1, user_id = $2, start_time = $3, end_time = $4 WHERE id = $5 RETURNING *",
      [room_id, user_id, start_time, end_time, id]
    );

    if (result.rows.length === 0) {
      return res.status(404).json({ error: "Booking not found" });
    }

    res.status(200).json(result.rows[0]);
  })
);

// DELETE a booking
router.delete(
  "/:id",
  asyncHandler(async (req: Request, res: Response) => {
    const { id } = req.params;
    const result = await pool.query(
      "DELETE FROM bookings WHERE id = $1 RETURNING *",
      [id]
    );
    if (result.rows.length === 0) {
      return res.status(404).json({ error: "Booking not found" });
    }
    res.status(204).send();
  })
);

export default router;
