import rateLimit from "express-rate-limit";
import { Request, Response } from "express";

const limiter = rateLimit({
  windowMs: 10 * 60 * 1000, // 10 minuter
  max: 100, // Tillåt 100 förfrågningar per IP inom fönstret
  message: "Too many requests, please try again later.",
  handler: (req: Request, res: Response) => {
    console.warn(
      `[RATE LIMIT] ${req.ip} exceeded the limit of 100 requests in 10 minutes.`
    );
    res.status(429).send("Too many requests, please try again later.");
  },
});

export default limiter;
