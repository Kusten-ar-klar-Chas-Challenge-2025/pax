import * as fs from "fs";
import * as path from "path";
import yaml from "yaml";

// Ladda samtliga yaml-filer
const swaggerDir = path.join(__dirname, "../swagger");
const files = fs.readdirSync(swaggerDir).filter((f) => f.endsWith(".yaml"));

const combined = {
  openapi: "3.0.0",
  info: {
    title: "Pax API",
    version: "1.0.0",
    description:
      "Documentation for the Pax API, powering automated room bookings through real-time sensor data. This API handles users, rooms, and sensors to streamline presence-based reservation management.",
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
  const parsed = yaml.parse(content);

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
