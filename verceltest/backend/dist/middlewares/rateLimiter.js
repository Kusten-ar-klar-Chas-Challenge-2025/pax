"use strict";
var __importDefault = (this && this.__importDefault) || function (mod) {
    return (mod && mod.__esModule) ? mod : { "default": mod };
};
Object.defineProperty(exports, "__esModule", { value: true });
const express_rate_limit_1 = __importDefault(require("express-rate-limit"));
const limiter = (0, express_rate_limit_1.default)({
    windowMs: 10 * 60 * 1000, // 10 minuter
    max: 100, // Tillåt 100 förfrågningar per IP inom fönstret
    message: "Too many requests, please try again later.",
    handler: (req, res) => {
        console.warn(`[RATE LIMIT] ${req.ip} exceeded the limit of 100 requests in 10 minutes.`);
        res.status(429).send("Too many requests, please try again later.");
    },
});
exports.default = limiter;
