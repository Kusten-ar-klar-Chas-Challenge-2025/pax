import swaggerJSDoc from "swagger-jsdoc";
import path from "path";
import { fileURLToPath } from "url";
import { dirname } from "path";

const __filename = fileURLToPath(import.meta.url);
const __dirname = dirname(__filename);

const swaggerDefinition = {
  openapi: "3.0.0",
  info: {
    title: "Pax API",
    version: "1.0.0",
    description:
      "Documentation for the Pax API, powering automated room bookings through real-time sensor data. This API handles users, rooms, and sensors to streamline presence-based reservation management.",
  },
  servers: [
    {
      url: "http://localhost:13000",
    },
  ],
  components: {
    schemas: {
      User: {
        type: "object",
        properties: {
          id: { type: "integer" },
          firstname: { type: "string" },
          lastname: { type: "string" },
          email: { type: "string" },
          role: { type: "string" },
        },
      },
      CreateUserInput: {
        type: "object",
        required: ["firstname", "lastname", "email", "password"],
        properties: {
          firstname: { type: "string" },
          lastname: { type: "string" },
          email: { type: "string" },
          password: { type: "string" },
          role: { type: "string" },
        },
      },
      Room: {
        type: "object",
        properties: {
          id: { type: "integer" },
          name: { type: "string" },
          description: { type: "string" },
          available: { type: "boolean" },
          air_quality: { type: "string" },
          screen: { type: "boolean" },
          floor: { type: "integer" },
          chairs: { type: "integer" },
          whiteboard: { type: "boolean" },
          projector: { type: "boolean" },
        },
      },
      CreateRoomInput: {
        type: "object",
        required: [
          "name",
          "description",
          "available",
          "air_quality",
          "screen",
          "floor",
          "chairs",
          "whiteboard",
          "projector",
        ],
        properties: {
          name: { type: "string" },
          description: { type: "string" },
          available: { type: "boolean" },
          air_quality: { type: "string" },
          screen: { type: "boolean" },
          floor: { type: "integer" },
          chairs: { type: "integer" },
          whiteboard: { type: "boolean" },
          projector: { type: "boolean" },
        },
      },
      Error: {
        type: "object",
        properties: {
          error: { type: "string" },
        },
      },
    },
  },
};

const options = {
  swaggerDefinition,
  apis: [path.join(__dirname, "../routes/*.js")],
};

console.log(
  "Looking for Swagger comments in:",
  path.join(__dirname, "../routes/*.js")
);

const swaggerSpec = swaggerJSDoc(options);

export default swaggerSpec;
