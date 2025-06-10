# SUVX-specifik retrospektiv
Se [retrospektiv](../../RETROSPECTIVE.md) för hela gruppens tankar om arbetet i stort.
## Vidareutveckling
Om vi hade fortsatt att utveckla produkten så finns det några identifierade nästa steg.
- Byta inloggningsmetod.
Just nu lagrar vi inlogg till JWT i progmem, men vi borde egentligen kräva inloggning (sätta Arduinon i AP-läge och visa captive-portal) vid setup, så att vi kan lagra vår JWT-token i EEPROM och en recovery-token i RAM. Detta följer branschens riktlinjer.
Det skulle kräva omarbetning av vårt main-flöde men borde inte ta mycket tid att fixa.
- mm-vågsensor för rumsaktivitet.
En känsligare sensor för att upptäcka även stillasittande personer. Den kan kombineras med PIR för olika tillstånd. Exempelvis PIR efter arbetstid eller obokat rum, mm-våg när rummet är bokat eller aktivt.
mm-våg drar mer ström så vi vill bara använda den när den behövs.
- Strukturera projektet för enhetstestning.
I produktionsscenario så vill vi ha en CI/CD-pipeline. Detta kräver att vi har ett projekt som går att enhetstesta. Se [PAX Advanced](https://github.com/simbachu/pax-advanced) för en möjlig väg att gå.
- Fixa kalibreringen av temperatursensorn
Just nu fungerar inte EEPROM-lagringen riktigt. Det kan fixas. Dessutom borde kalibreringen göras relativ så att man kan finjustera utifrån uppmätt temperatur.
- Ett tillstånd för inmatning
När vi har identiferat att vi har inkommande kalibreringar över Serial borde vi sätta ett tillstånd så att vi inte kör innan kalibreringen är genomförd. Detta för att säkerställa korrekt feedback över Serial. Just nu funkar det 9/10 gånger då ett annat meddelande kan ligga i buffer.