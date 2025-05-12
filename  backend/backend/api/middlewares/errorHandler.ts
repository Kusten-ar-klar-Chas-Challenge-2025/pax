import { Request, Response, NextFunction } from "express";

/**
 * Global felhanterare som fångar fel från async routes och middleware.
 */
const errorHandler = (
  err: any,
  _req: Request,
  res: Response,
  _next: NextFunction
) => {
  console.error("Unhandled error:", err);
  res.status(500).json({ message: "Internal Server Error" });
};

export default errorHandler;
