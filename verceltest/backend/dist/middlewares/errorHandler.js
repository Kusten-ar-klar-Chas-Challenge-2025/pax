"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
/**
 * Global felhanterare som fångar fel från async routes och middleware.
 */
const errorHandler = (err, _req, res, _next) => {
    console.error("Unhandled error:", err);
    res.status(500).json({ message: "Internal Server Error" });
};
exports.default = errorHandler;
