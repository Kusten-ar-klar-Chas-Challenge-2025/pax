# Chas Challenge 2025 "Kusten är Klar"

**SUVX24 Authors:** [Jennifer Gott](https://github.com/simbachu), [Sabina Stawbrink](https://github.com/binasime), [Oscar Asserlund](https://github.com/NewNamesAreHard), [Erik Dahl](https://github.com/erikdsp), [Johan Modin](https://github.com/bubba-94)  

## Beskrivning

PAX är ett bokningssystem bestående av olika sensorer och teknologier i en hel produkt som mäter aktivitet och olika miljödata i konferensrum, kopplade till en enhet.  
Den insamlade datan hanteras av en IoT-komponent som kommunicerar med en backend via ett API, där data lagras i en databas.  
Med PAX kan du på ett effektivt och automatiserat sätt, övervaka statusen på era olika lokaler.  

### Komponenter

| **Komponent**                     | **I/O Pin**           | **Beskrivning**                                     |
| --------------------------------- | --------------------- | --------------------------------------------------- |
| **Arduino Uno Rev4 Mini**         |                       | Microkontroller för inkoppling.                     |
| **PIR-sensor HC-SR501**           | **2**                 | Används för att detektera rörelse i rummet.         |
| **LED**                           | **3**                 | Används för att driva en LED.                       |
| **Temp-sensor DS18B20**           | **6**                 | Används för temperaturmätning i rummet.             |
| **Luftkvalitets-sensor SPG30**    | **SDA(A4) / SCL(A5)** | Används för att mäta luftkvaliteten i rummet.       |
| **Internet Interface Config Pin** | **7**                 | Används för att indikera val av nätverks interface. |

[Projektplan](https://github.com/Kusten-ar-klar-Chas-Challenge-2025/pax/blob/main/PROJEKTPLAN.md)  
[SRS](https://github.com/Kusten-ar-klar-Chas-Challenge-2025/pax/blob/docs/documentation_updates/Iot/docs/SRS.md)  

### Möjlig industrialiserad design
![alt text](pax-dosa.png)

#### Platform IO

Installera PlatformIO och öppna undermappen pax/Iot med PlatformIO.  
PlatformIO kommer automatiskt installera de beroenden som är definierade i Platformio.ini.  
Skapa en .h fil kallad **`arduino_secrets.h`** i **`/include`** mappen.  
Kopiera mallen under för att konfigurera din egna uppkoppling (SSID, Password, URL) mellan sensor och wifi.  

``` cpp

#ifndef ARDUINO_SECRETS_H // Header guard 
#define ARDUINO_SECRETS_H 

#define SECRET_SSID "SSID" // Your WiFi SSID
#define SECRET_PASS "PASSWORD" // Your WiFi password 

// Server configuration
#define SERVER_URL "database.com"
#define SERVER_PORT 443  // Default HTTPS port
#define API_PATH "" 

// Auth configuration
#define JWT_USER "email"
#define JWT_PASS "password"

// Device configuration
#define DEVICE_UUID "UUID2"  // UUID for initial registration
#define ROOM_OVERRIDE 30     // Optional room ID override for testing


#endif
```
