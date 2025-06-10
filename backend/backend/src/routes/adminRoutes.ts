import express from 'express';
import authenticateToken, { AuthenticatedRequest } from '../middlewares/authMiddleware.ts';
import requireRole from '../middlewares/requireRole';

const router = express.Router();

router.get('/dashboard', authenticateToken, requireRole('admin'), (req: AuthenticatedRequest, res) => {
    res.json({ message: 'Welcome to the admin dashboard', user: req.user });
});

export default router;
