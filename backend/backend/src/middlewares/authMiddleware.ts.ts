import { Request, Response, NextFunction } from 'express';
import jwt, { JwtPayload } from 'jsonwebtoken';

interface AuthenticatedRequest extends Request {
    user?: JwtPayload & { role?: string }; // Lägg till role direkt
}

const authenticateToken = (
    req: AuthenticatedRequest,
    res: Response,
    next: NextFunction
): void => { // explicit return type
    const token = req.header('Authorization')?.replace('Bearer ', '');
    console.log('Token från header:', token);

    if (!token) {
        res.status(401).json({ message: 'No token provided' });
        return; // return void
    }

    try {
        const decoded = jwt.verify(token, process.env.JWT_SECRET || 'fallback_secret');

        // Kontrollera att det är ett objekt och har id/username/role
        if (typeof decoded !== 'object' || decoded === null || !('id' in decoded)) {
            res.status(403).json({ message: 'Invalid token structure' });
            return;
        }

        req.user = decoded as JwtPayload & { role?: string };
        console.log('Token valid, user:', req.user);

        next();
    } catch (err) {
        console.error('Token verification error:', err);
        res.status(403).json({ message: 'Invalid token' });
        return;
    }
};

export default authenticateToken;
export type { AuthenticatedRequest };
