# 1. Basbild
FROM node:18

# 2. Arbetskatalog
WORKDIR /usr/src/app

# 3. Kopiera package-filer och installera beroenden
COPY package*.json ./
RUN npm install

# 4. Kopiera resten av koden
COPY . .

# 5. Exponera porten
EXPOSE 13000

# 6. Välj startkommando baserat på miljö
# DEV = ts-node-dev
# PROD = kör kompilerad JS från dist
CMD [ "npm", "run", "dev" ]
