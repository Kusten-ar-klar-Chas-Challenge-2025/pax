"use strict";
var __createBinding = (this && this.__createBinding) || (Object.create ? (function(o, m, k, k2) {
    if (k2 === undefined) k2 = k;
    var desc = Object.getOwnPropertyDescriptor(m, k);
    if (!desc || ("get" in desc ? !m.__esModule : desc.writable || desc.configurable)) {
      desc = { enumerable: true, get: function() { return m[k]; } };
    }
    Object.defineProperty(o, k2, desc);
}) : (function(o, m, k, k2) {
    if (k2 === undefined) k2 = k;
    o[k2] = m[k];
}));
var __setModuleDefault = (this && this.__setModuleDefault) || (Object.create ? (function(o, v) {
    Object.defineProperty(o, "default", { enumerable: true, value: v });
}) : function(o, v) {
    o["default"] = v;
});
var __importStar = (this && this.__importStar) || (function () {
    var ownKeys = function(o) {
        ownKeys = Object.getOwnPropertyNames || function (o) {
            var ar = [];
            for (var k in o) if (Object.prototype.hasOwnProperty.call(o, k)) ar[ar.length] = k;
            return ar;
        };
        return ownKeys(o);
    };
    return function (mod) {
        if (mod && mod.__esModule) return mod;
        var result = {};
        if (mod != null) for (var k = ownKeys(mod), i = 0; i < k.length; i++) if (k[i] !== "default") __createBinding(result, mod, k[i]);
        __setModuleDefault(result, mod);
        return result;
    };
})();
var __importDefault = (this && this.__importDefault) || function (mod) {
    return (mod && mod.__esModule) ? mod : { "default": mod };
};
Object.defineProperty(exports, "__esModule", { value: true });
const fs = __importStar(require("fs"));
const path = __importStar(require("path"));
const yaml_1 = __importDefault(require("yaml"));
// Ladda samtliga yaml-filer
const swaggerDir = path.join(__dirname, "../swagger");
const files = fs.readdirSync(swaggerDir).filter((f) => f.endsWith(".yaml"));
const combined = {
    openapi: "3.0.0",
    info: {
        title: "Pax API",
        version: "1.0.0",
        description: "Documentation for the Pax API, powering automated room bookings through real-time sensor data. This API handles users, rooms, and sensors to streamline presence-based reservation management.",
    },
    servers: [
        { url: "http://localhost:13000" },
        {
            description: "Mock server",
            url: "https://virtserver.swaggerhub.com/alicegmn/pax-api/dev-oas3",
        },
    ],
    paths: {},
    components: { schemas: {} },
};
for (const file of files) {
    const content = fs.readFileSync(path.join(swaggerDir, file), "utf8");
    const parsed = yaml_1.default.parse(content);
    if (parsed.paths) {
        Object.assign(combined.paths, parsed.paths);
    }
    if (parsed.components?.schemas) {
        Object.assign(combined.components.schemas, parsed.components.schemas);
    }
}
// Exportera till JSON
const outputPath = path.join(__dirname, "../../swagger-export.json");
fs.writeFileSync(outputPath, JSON.stringify(combined, null, 2));
console.log("✅ Exported Swagger JSON to", outputPath);
