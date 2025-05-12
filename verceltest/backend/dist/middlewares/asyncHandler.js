"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
/**
 * Wrapper för asynkrona route-handlers så att fel skickas vidare till Express felhantering.
 */
const asyncHandler = (fn) => {
    return (req, res, next) => {
        Promise.resolve(fn(req, res, next)).catch(next);
    };
};
exports.default = asyncHandler;
