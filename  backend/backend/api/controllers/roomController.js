import pool from "../db.js";

// Get all rooms
export const getAllRooms = async (req, res) => {
    try {
        const data = await pool.query("SELECT * FROM rooms");
        res.status(200).json(data.rows);
    } catch (error) {
        console.error("Error fetching rooms:", error);
        res.status(500).send("Internal Server Error");
    }
};

// Get a specific rooom 
export const getRoomById = async (req, res) => {
    const id = parseInt(req.params.id);
    try {
        const result = await pool.query("SELECT * FROM rooms WHERE id = $1", [id]);
        if (result.rows.length === 0) {
            return res.status(404).json({ message: "Room not found" });
        }
        res.status(200).json(result.rows[0]);
    } catch (err) {
        console.error("Error fetching room:", err);
        res.status(500).json({ message: "Internal server error" });
    }
};

// Create a room
export const createRoom = async (req, res) => {
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
            `INSERT INTO rooms 
        (name, description, available, air_quality, screen, floor, chairs, whiteboard, projector)
       VALUES ($1, $2, $3, $4, $5, $6, $7, $8, $9)
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
            ]
        );

        res.status(201).json(result.rows[0]);
    } catch (err) {
        console.error("Error creating room:", err);
        res.status(500).send("Error creating room");
    }
};

// Updates a room
export const updateRoom = async (req, res) => {
    const id = parseInt(req.params.id);
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
      WHERE id = $10
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
        console.error("Error updating room:", err);
        res.status(500).json({ message: "Internal server error" });
    }
};

// Deletes a room
export const deleteRoom = async (req, res) => {
    const id = parseInt(req.params.id);

    try {
        const result = await pool.query(
            "DELETE FROM rooms WHERE id = $1 RETURNING *",
            [id]
        );

        if (result.rows.length === 0) {
            return res.status(404).json({ message: "Room not found" });
        }

        res.status(200).json({ message: "Room deleted", room: result.rows[0] });
    } catch (err) {
        console.error("Error deleting room:", err);
        res.status(500).json({ message: "Internal server error" });
    }
};
