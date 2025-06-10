import express from "express";
import { registerDevice, assignRoomToDevice, getDeviceConfig } from "../controllers/deviceConfigController";

const router = express.Router();

router.post("/:serialNumber", registerDevice);
router.put("/:serialNumber", assignRoomToDevice);
router.get("/:serialNumber", getDeviceConfig);

export default router;