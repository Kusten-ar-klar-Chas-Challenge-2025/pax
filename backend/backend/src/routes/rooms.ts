import express, { Request, Response } from "express";
import pool from "../db";
import asyncHandler from "../middlewares/asyncHandler";

const router = express.Router();

interface Room {
  id: number;
  name: string;
  description: string;
  available: boolean;
  air_quality: number;
  screen: boolean;
  floor: number;
  chairs: number;
  whiteboard: boolean;
  projector: boolean;
  temperature: number;
  activity: boolean;
  time: string;
  img: string;
}

type CreateRoomInput = Omit<Room, "id">;

// GET all rooms
router.get(
  "/",
  asyncHandler(async (_req: Request, res: Response) => {
    const result = await pool.query("SELECT * FROM rooms");
    res.status(200).json(result.rows);
  })
);

// GET room by ID
router.get(
  "/:id",
  asyncHandler(async (req, res) => {
    const id = parseInt(req.params.id, 10);
    if (isNaN(id)) {
      return res.status(400).json({ message: "Invalid room ID" });
    }

    const result = await pool.query("SELECT * FROM rooms WHERE id = $1", [id]);
    if (result.rows.length === 0) {
      return res.status(404).json({ message: "Room not found" });
    }

    res.status(200).json(result.rows[0]);
  })
);

// POST create new room
router.post(
  "/",
  asyncHandler(async (req: Request, res: Response) => {
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
      temperature,
      activity,
      time,
      img,
    } = req.body as CreateRoomInput;

    const result = await pool.query(
      `INSERT INTO rooms 
      (name, description, available, air_quality, screen, floor, chairs, whiteboard, projector, temperature, activity, time, img)
      VALUES ($1, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11, $12, $13)
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
        temperature,
        activity,
        time,
        img,
      ]
    );

    res.status(201).json(result.rows[0]);
  })
);

// PUT update room
router.put(
  "/:id",
  asyncHandler(async (req, res) => {
    const id = parseInt(req.params.id, 10);
    if (isNaN(id)) {
      return res.status(400).json({ message: "Invalid room ID" });
    }

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
      temperature,
      activity,
      time,
      img,
    } = req.body as CreateRoomInput;

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
        projector = $9,
        temperature = $10,
        activity = $11,
        time = $12,
        img = $13
        WHERE id = $14
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
        temperature,
        activity,
        time,
        img,
        id,
      ]
    );

    if (result.rows.length === 0) {
      return res.status(404).json({ message: "Room not found" });
    }

    res.status(200).json(result.rows[0]);
  })
);

// DELETE room
router.delete(
  "/:id",
  asyncHandler(async (req, res) => {
    const id = parseInt(req.params.id, 10);
    if (isNaN(id)) {
      return res.status(400).json({ message: "Invalid room ID" });
    }

    const result = await pool.query(
      "DELETE FROM rooms WHERE id = $1 RETURNING *",
      [id]
    );

    if (result.rows.length === 0) {
      return res.status(404).json({ message: "Room not found" });
    }

    res.status(200).json({ message: "Room deleted", room: result.rows[0] });
  })
);

export default router;
