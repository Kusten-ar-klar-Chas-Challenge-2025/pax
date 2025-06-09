# Pax-dosan
## Inledning
PAX-dosan installeras i konferensrum som en del i ett konferensbokningssystem.
PAX-dosan samlar information om närvaro i rummet via rörelsesensor och eCO2-nivå, samt information om rumstemperatur och luftkvalitet. Detta skickas sedan till backend för bearbetning och lagring.
## Hårdvara
PAX-dosan kontrolleras av en Arduino R4 Wifi. På den är det kopplat en temperatursensor, en gassensor och en PIR-sensor. Vi har också en lysdiod för handhavande i rummet.
### Koppling
![Kopplingsschema](./docs/SCH_PAX-Schematic_1-P1_2025-05-26.png)
### PIR-sensor
Monteras så att den ser hela rummet i brösthöjd. Skall sättas i HOLD-läge och trimmas till lämplig håll-tid (vi föreslår 30 sekunder).
### Temperatur-sensor
Vi använder för närvarande en DHT11. Det var inte vårt förstaval för produktion då precisionen är på +-1 grad. Kalibreras vid "fabrik": Använd TEMP+x-kommando för att kalibrera offset till uppmätt inomhustemperatur.
### Gassensor
Vi använder SGP30,inkopplad på I2C-bus. Vi skickar ett värde till backend mellan 0.0 till 100.0 där 100.0 är bästa luftkvalitet.
### EEPROM
Vi har 2kb lagring via en 24LC64-EEPROM på I2C-bus för lagring av exempelvis kalibrering och liknande.
## Mjukvara
### Main-program
I mainprogrammet kliver enheten genom olika tillstånd för att starta  till driftläge. Om ett lyckat tillstånd inte nås går enheten in i ett lågintensivt läge där den då och då kollar om den kan komma vidare. När den försökt några gånger går den i passivt läge och behöver startas om för att försöka igen.
#### Huvudlägen
- Koppla upp till WiFi. Vid fel sätts enheten i försök igen-läge.
- Försök logga in på servern för att hämta JWT-uppgift. Vid fel försöker den igen några gånger innan den sätter passivt, då vi inte kan återhämta oss på ett rimligt sätt utan handhavande.
- Försök komma åt JWT-skyddad resurs. GET från /rooms/ är JWT-skyddad, så vi kollar efter ett 200-svar därifrån. Om vi inte får 200 så går vi passiv, då vår token inte är giltig.
- Om vi har Wifi, giltig JWT-token, och kan göra http-kommandon är vi i driftläge.
### Backend-koppling
Uppkoppling till server görs med det i WiFiS3.h-inbyggda klassen WiFiSSLClient i en Backend-klass. Http-kommandon konstrueras med de headers som behövs för Vercel-kopplingen. Olika http-funktioner som GET, PUT, PATCH och POST stödjs.
### Mättillstånd
Sensorernas utläsning samlas i en klass som hanterar utläsning. Utläsningen skickas direkt vidare till backend-klassen.
