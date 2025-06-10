import { Response, NextFunction } from 'express';
import { AuthenticatedRequest } from './authMiddleware.ts';

const requireRole = (role: string) => {
    return (req: AuthenticatedRequest, res: Response, next: NextFunction): void => {
        if (!req.user || req.user.role !== role) {
            res.status(403).json({ message: 'Access denied: insufficient role' });
            return;
        }
        next();
    };
};

export default requireRole;
