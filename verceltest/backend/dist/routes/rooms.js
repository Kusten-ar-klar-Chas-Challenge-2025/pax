"use strict";
var __importDefault = (this && this.__importDefault) || function (mod) {
    return (mod && mod.__esModule) ? mod : { "default": mod };
};
Object.defineProperty(exports, "__esModule", { value: true });
const express_1 = __importDefault(require("express"));
const db_1 = __importDefault(require("../db"));
const asyncHandler_1 = __importDefault(require("../middlewares/asyncHandler"));
const router = express_1.default.Router();
/**
 * @swagger
 * /rooms:
 *   get:
 *     summary: Get all rooms
 *     tags: [Rooms]
 *     responses:
 *       200:
 *         description: A list of rooms
 *         content:
 *           application/json:
 *             schema:
 *               type: array
 *               items:
 *                 $ref: '#/components/schemas/Room'
 *       500:
 *         description: Internal server error
 */
router.get("/", (0, asyncHandler_1.default)(async (_req, res) => {
    const result = await db_1.default.query("SELECT * FROM rooms");
    res.status(200).json(result.rows);
}));
/**
 * @swagger
 * /rooms/{id}:
 *   get:
 *     summary: Get a room by ID
 *     tags: [Rooms]
 *     parameters:
 *       - in: path
 *         name: id
 *         required: true
 *         schema:
 *           type: integer
 *         description: Room ID
 *     responses:
 *       200:
 *         description: Room found
 *         content:
 *           application/json:
 *             schema:
 *               $ref: '#/components/schemas/Room'
 *       404:
 *         description: Room not found
 *       500:
 *         description: Internal server error
 */
router.get("/:id", (0, asyncHandler_1.default)(async (req, res) => {
    const id = parseInt(req.params.id, 10);
    if (isNaN(id)) {
        return res.status(400).json({ message: "Invalid room ID" });
    }
    const result = await db_1.default.query("SELECT * FROM rooms WHERE id = $1", [id]);
    if (result.rows.length === 0) {
        return res.status(404).json({ message: "Room not found" });
    }
    res.status(200).json(result.rows[0]);
}));
/**
 * @swagger
 * /rooms:
 *   post:
 *     summary: Create a new room
 *     tags: [Rooms]
 *     requestBody:
 *       required: true
 *       content:
 *         application/json:
 *           schema:
 *             $ref: '#/components/schemas/CreateRoomInput'
 *     responses:
 *       201:
 *         description: Room created successfully
 *         content:
 *           application/json:
 *             schema:
 *               $ref: '#/components/schemas/Room'
 *       500:
 *         description: Error creating room
 */
router.post("/", (0, asyncHandler_1.default)(async (req, res) => {
    const { name, description, available, air_quality, screen, floor, chairs, whiteboard, projector, } = req.body;
    const result = await db_1.default.query(`INSERT INTO rooms 
    (name, description, available, air_quality, screen, floor, chairs, whiteboard, projector)
    VALUES ($1, $2, $3, $4, $5, $6, $7, $8, $9)
    RETURNING *`, [
        name,
        description,
        available,
        air_quality,
        screen,
        floor,
        chairs,
        whiteboard,
        projector,
    ]);
    res.status(201).json(result.rows[0]);
}));
/**
 * @swagger
 * /rooms/{id}:
 *   put:
 *     summary: Update a room
 *     tags: [Rooms]
 *     parameters:
 *       - in: path
 *         name: id
 *         required: true
 *         schema:
 *           type: integer
 *         description: Room ID
 *     requestBody:
 *       required: true
 *       content:
 *         application/json:
 *           schema:
 *             $ref: '#/components/schemas/CreateRoomInput'
 *     responses:
 *       200:
 *         description: Room updated successfully
 *         content:
 *           application/json:
 *             schema:
 *               $ref: '#/components/schemas/Room'
 *       404:
 *         description: Room not found
 *       500:
 *         description: Internal server error
 */
router.put("/:id", (0, asyncHandler_1.default)(async (req, res) => {
    const id = parseInt(req.params.id, 10);
    const { name, description, available, air_quality, screen, floor, chairs, whiteboard, projector, } = req.body;
    const result = await db_1.default.query(`UPDATE rooms SET 
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
    RETURNING *`, [
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
    ]);
    if (result.rows.length === 0) {
        return res.status(404).json({ message: "Room not found" });
    }
    res.status(200).json(result.rows[0]);
}));
/**
 * @swagger
 * /rooms/{id}:
 *   delete:
 *     summary: Delete a room
 *     tags: [Rooms]
 *     parameters:
 *       - in: path
 *         name: id
 *         required: true
 *         schema:
 *           type: integer
 *         description: Room ID
 *     responses:
 *       200:
 *         description: Room deleted successfully
 *         content:
 *           application/json:
 *             schema:
 *               type: object
 *               properties:
 *                 message:
 *                   type: string
 *                 room:
 *                   $ref: '#/components/schemas/Room'
 *       404:
 *         description: Room not found
 *       500:
 *         description: Internal server error
 */
router.delete("/:id", (0, asyncHandler_1.default)(async (req, res) => {
    const id = parseInt(req.params.id, 10);
    const result = await db_1.default.query("DELETE FROM rooms WHERE id = $1 RETURNING *", [id]);
    if (result.rows.length === 0) {
        return res.status(404).json({ message: "Room not found" });
    }
    res.status(200).json({ message: "Room deleted", room: result.rows[0] });
}));
exports.default = router;
