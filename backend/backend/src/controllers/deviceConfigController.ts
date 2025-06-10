import { Request, Response, } from "express";
import pool from "../db";

export const registerDevice = async (req: Request, res: Response) => {
    const { serialNumber } = req.params;
    try {
        const result = await pool.query(
            `INSERT INTO device_configs (serial_number)
       VALUES ($1)
       ON CONFLICT DO NOTHING
       RETURNING *`,
            [serialNumber]
        );
        res.status(201).json({ message: "Device registered", data: result.rows[0] || {} });
    } catch (err) {
        console.error(err);
        res.status(500).json({ message: "Device registration failed" });
    }
};

export const assignRoomToDevice = async (req: Request, res: Response) => {
    const { serialNumber } = req.params;
    const { roomId } = req.body;
    try {
        await pool.query(
            `UPDATE device_configs
       SET room_id = $1
       WHERE serial_number = $2`,
            [roomId, serialNumber]
        );
        res.json({ message: "Room assigned" });
    } catch (err) {
        console.error(err);
        res.status(500).json({ message: "Room assignment failed" });
    }
};

export const getDeviceConfig = async (
    req: Request,
    res: Response
): Promise<void> => {
    const { serialNumber } = req.params;
    try {
        const result = await pool.query(
            `SELECT room_id FROM device_configs WHERE serial_number = $1`,
            [serialNumber]
        );

        const row = result.rows[0];
        if (!row || !row.room_id) {
            res.json({});
            return;
        }

        res.json({ roomId: row.room_id });
    } catch (err) {
        console.error(err);
        res.status(500).json({ message: "Failed to fetch config" });
    }
};
