# PAX-API
[Pax-api](https://paxdb.vercel.app/docs)
nb: Backend har bytt /config/ till /backend-config/
## Vår användning av PAX-API
Enligt programmets normala flöde:
Setup:
- 25: GET till /device-config/:serial-number för att hämta rumsnummer. 
- 5: POST till /auth/login/ med uppgifter för att hämta JWT
- 12: GET till /rooms/ för att testa giltighet av JWT-token
Loop:
- 15: PATCH till /rooms/:id för att skicka uppdaterad rumsdata
- 25: GET till /device-config/:serial-number för att hämta uppdaterad konfiguration med långt mellanrum