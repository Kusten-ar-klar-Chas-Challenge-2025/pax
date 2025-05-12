// controllers/usersController.js
import pool from "../db.js";

// Create a new user
export const createUser = async (req, res) => {
    const { firstname, lastname, email, password, role } = req.body;

    try {
        const result = await pool.query(
            `INSERT INTO users (firstname, lastname, email, password, role)
       VALUES ($1, $2, $3, $4, $5)
       RETURNING id, firstname, lastname, email, role`,
            [firstname, lastname, email, password, role || "user"]
        );
        res.status(201).json(result.rows[0]);
    } catch (err) {
        console.error("Error creating user:", err);
        res.status(500).json({ error: "Error creating user" });
    }
};

// GET all users
export const getAllUsers = async (req, res) => {
    try {
        const result = await pool.query(
            "SELECT id, firstname, lastname, email, role FROM users"
        );
        res.json(result.rows);
    } catch (err) {
        console.error("Error fetching users:", err);
        res.status(500).json({ error: "Error fetching users" });
    }
};

// GET a specific user
export const getUserById = async (req, res) => {
    const { id } = req.params;

    try {
        const result = await pool.query(
            "SELECT id, firstname, lastname, email, role FROM users WHERE id = $1",
            [id]
        );

        if (result.rows.length === 0) {
            return res.status(404).json({ error: "User not found" });
        }

        res.json(result.rows[0]);
    } catch (err) {
        console.error("Error fetching user:", err);
        res.status(500).json({ error: "Error fetching user" });
    }
};

// Update a user
export const updateUser = async (req, res) => {
    const { id } = req.params;
    const { firstname, lastname, email, role } = req.body;

    try {
        const result = await pool.query(
            `UPDATE users 
       SET firstname = $1, lastname = $2, email = $3, role = $4
       WHERE id = $5 
       RETURNING id, firstname, lastname, email, role`,
            [firstname, lastname, email, role, id]
        );

        if (result.rows.length === 0) {
            return res.status(404).json({ error: "User not found" });
        }

        res.json(result.rows[0]);
    } catch (err) {
        console.error("Error updating user:", err);
        res.status(500).json({ error: "Error updating user" });
    }
};

// Delete a user
export const deleteUser = async (req, res) => {
    const { id } = req.params;

    try {
        const result = await pool.query(
            "DELETE FROM users WHERE id = $1 RETURNING id, firstname, lastname, email, role",
            [id]
        );

        if (result.rows.length === 0) {
            return res.status(404).json({ error: "User not found" });
        }

        res.json({ message: "User deleted", user: result.rows[0] });
    } catch (err) {
        console.error("Error deleting user:", err);
        res.status(500).json({ error: "Error deleting user" });
    }
};
