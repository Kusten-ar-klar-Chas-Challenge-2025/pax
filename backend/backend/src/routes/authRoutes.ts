import express, { Request, Response } from "express";
import bcrypt from "bcryptjs";
import jwt from "jsonwebtoken";
import pool from "../db";
import { JwtPayload } from "jsonwebtoken";
import asyncHandler from "../middlewares/asyncHandler";

const router = express.Router();

interface User {
  id: number;
  name: string;
  surname: string;
  email: string;
  role: string;
}

interface DbUser extends User {
  password: string;
}

const allowedRoles = ["admin", "user", "moderator"];

// // POST /register
// router.post(
//   "/register",
//   asyncHandler(async (req: Request, res: Response) => {
//     const {
//       name,
//       surname,
//       email,
//       password,
//       role,
//     }: { name: string; surname: string; email: string; password: string; role?: string } = req.body;

//     // Validate role
//     if (role && !allowedRoles.includes(role)) {
//       return res.status(400).json({
//         message: "Invalid role. Allowed roles are: admin, user, moderator.",
//       });
//     }

//     try {
//       const existingUser = await pool.query(
//         "SELECT * FROM users WHERE email = $1",
//         [email]
//       );
//       if (existingUser.rows.length > 0) {
//         return res.status(409).json({ message: "Username is already taken" });
//       }

//       const hashedPassword = await bcrypt.hash(password, 10);

//       const result = await pool.query(
//         "INSERT INTO users (name, surname, email, password, role) VALUES ($1, $2, $3) RETURNING id, name, surname, email, role",
//         [name, surname, email, hashedPassword, role || "user"]
//       );

//       const newUser: User = result.rows[0];

//       const token = jwt.sign(
//         {
//           id: newUser.id,
//           email: newUser.email,
//           role: newUser.role,
//         } as JwtPayload,
//         process.env.JWT_SECRET as string,
//         { expiresIn: "1h" }
//       );

//       res.status(201).json({
//         message: "User registered successfully",
//         token,
//         user: newUser,
//       });
//     } catch (err: any) {
//       console.error("Registration error:", err);
//       res
//         .status(500)
//         .json({ message: "Registration failed", error: err.message });
//     }
//   })
// );

router.post(
  "/register",
  asyncHandler(async (req: Request, res: Response) => {
    const {
      email,
      password,
      name,
      surname,
      role,
    }: { email: string; password: string; name: string; surname: string; role?: string } = req.body;
    if (role && !allowedRoles.includes(role)) {
      return res.status(400).json({
        message: "Invalid role. Allowed roles are: admin, user, moderator.",
      });
    }
    const existingUser = await pool.query("SELECT * FROM users WHERE email = $1", [email]);
    if (existingUser.rows.length > 0) {
      return res.status(409).json({ message: "Email is already registered" });
    }
    const hashedPassword = await bcrypt.hash(password, 10);
    const result = await pool.query(
      "INSERT INTO users (email, password, name, surname, role) VALUES ($1, $2, $3, $4, $5) RETURNING id, email, name, surname, role",
      [email, hashedPassword, name, surname, role || "user"]
    );
    const newUser = result.rows[0];
    const token = jwt.sign(
      {
        id: newUser.id,
        email: newUser.email,
        role: newUser.role,
      },
      process.env.JWT_SECRET as string,
      { expiresIn: "1h" }
    );
    res.status(201).json({
      message: "User registered successfully",
      token,
      user: newUser,
    });
  })
);

// POST /login
// router.post(
//   "/login",
//   asyncHandler(async (req: Request, res: Response) => {
//     const { email, password }: { email: string; password: string } =
//       req.body;

//     try {
//       const result = await pool.query(
//         "SELECT * FROM users WHERE email = $1",
//         [email]
//       );
//       const dbUser: DbUser | undefined = result.rows[0];

//       if (!dbUser) {
//         return res.status(401).json({ message: "Invalid login credentials" });
//       }

//       const isPasswordValid = await bcrypt.compare(password, dbUser.password);
//       if (!isPasswordValid) {
//         return res.status(401).json({ message: "Invalid login credentials" });
//       }

//       const user: User = {
//         id: dbUser.id,
//         email: dbUser.email,
//         role: dbUser.role,
//       };

//       const token = jwt.sign(
//         { id: user.id, email: user.email, role: user.role } as JwtPayload,
//         process.env.JWT_SECRET as string,
//         { expiresIn: "1h" }
//       );

//       res.json({
//         message: "Login successful",
//         token,
//         role: user.role,
//       });
//     } catch (err: any) {
//       console.error("Login error:", err);
//       res.status(500).json({ message: "Login failed", error: err.message });
//     }
//   })
// );

router.post(
  "/login",
  asyncHandler(async (req: Request, res: Response) => {
    const { email, password }: { email: string; password: string } = req.body;
    const result = await pool.query("SELECT * FROM users WHERE email = $1", [email]);
    const dbUser = result.rows[0];
    if (!dbUser) {
      return res.status(401).json({ message: "Invalid login credentials" });
    }
    const isPasswordValid = await bcrypt.compare(password, dbUser.password);
    if (!isPasswordValid) {
      return res.status(401).json({ message: "Invalid login credentials" });
    }
    const token = jwt.sign(
      {
        id: dbUser.id,
        email: dbUser.email,
        role: dbUser.role,
      },
      process.env.JWT_SECRET as string,
      { expiresIn: "1h" }
    );
    res.json({
      message: "Login successful",
      token,
      role: dbUser.role,
    });
  })
);

export default router;
