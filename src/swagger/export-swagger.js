// export-swagger.js
import fs from "fs";
import swaggerSpec from "./swagger.js";

fs.writeFileSync("swagger-export.json", JSON.stringify(swaggerSpec, null, 2));
console.log("Swagger JSON exported to swagger-export.json");
