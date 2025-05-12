# <p align = "center"> PAX 
<p align = "center"> Automatiskt bokningssystem för företagslokaler - Chas Challenge 2025  </p> 
      
## Introduktion och bakgrund  
### Projektets syfte och mål:
Syftet med PAX är att effektivisera användningen av mötesrum genom ett automatiserat bokningssystem. Sensorer som känner av aktivitet installeras i rummen och kopplas till en app-tjänst. När du är i rummet bokas det automatiskt. Genom appen får användarna en bra överblick över tillgängliga rum. Målet är smidig bokning för användarna, optimering av beläggningen samt möjlighet för datainsamling för statistik och vidare analys. 

### Problemet som lösningen ska adressera:
Uppbokade mötesrum på större företag och företagshotell, som står tomma. Medarbetare behöver leta efter tomma rum, då rummen står uppbokade på en viss tid men inte används hela tiden. Lokalkostnader är bland det dyraste för företag. Att kunna växa större i samma lokaler har potential att spara miljonbelopp per år. 

### Intressenter och målgrupper:  
Större bolag med många anställda som vill kunna boka rum för möte och konferens (stor efterfrågan, lågt utbud). Som nämnts ovan finns stor möjlighet till besparingar genom effektivisering av rumsbeläggningen. Det kan även vara möjligt att modifiera produkten för andra typer av anläggningar med rum som används av flera personer, till exempel kontorshotell eller lokaler i bostadsrättsföreningar.



### Teknologier för frontend, backend och IoT.  
**Frontend:** React Native, Figma, Tailwind CSS  
**Backend:** PostgreSQL, Node.js och Express.js   
**IoT:** Arduino, C++, Sensorer 
___

## Teamstruktur och ansvarsområden  
### FMWX24:      
**Medlemmar:** [Tova](https://github.com/tovaalicia), [Hanna K](https://github.com/HannaKindholm), [Hannah B](https://github.com/HannahVrou)            
#### Huvudansvar och arbetsuppgifter:  
Med React Native och Tailwind ta fram en intuitiv och lättanvänd plattform för admin, implementera datavisualisering, bygga upp en mobil applikation för användare.   
Påbörjar layout, form, färg redan innan - fria händer för Frontend i färgen och formen.   
Ta reda på användarbehovet, hur kommer man vilja använda appen - gemensamma beslut samt avcheckning med de andra grupperna inom detta.
Ipad-layout för bokningsskärm utanför konferansrummet.

#### Teamsamarbete och kunskapsöverföring:  
Ta del av den data som kommer behövas i form av API:er från backend. Lämna rapport efter boiler room vad vi gjort och vilka utmaningar vi mött. Stand up-möte på Tisdagar där minst en Frontend:are representerar. Skapa en read.me fil för dokumentation. 
___
### SUVX24:      
**Medlemmar:** [Jennifer](https://github.com/simbachu), [Erik](https://github.com/erikdsp), [Oscar](https://github.com/NewNamesAreHard), [Sabina](https://github.com/binasime), [Johan](https://github.com/bubba-94)            
#### Huvudansvar och arbetsuppgifter:  
Bygga och utveckla hårdvara som med hjälp av sensorer kommunicerar med en API för datahantering.  
- Kontrollenhet för rummet
- Dataöverföring/kommunikation med backend via USB/WiFi
- Sensorer för att känna av rummets status:
  - IR-Laser
  - Mikrofon
  - Luftkvalitetssensor
#### Teamsamarbete och kunskapöverföring:  
Gemensam planering och arbete under både projektdagar och Boiler Rooms.  
___   

### FJSX24:      
**Medlemmar:** [Dennis](https://github.com/TheUnseenBug), [Alice](https://github.com/alicegmn), [Rhiannon](https://github.com/Rhibro), [Phithai]()       
#### Huvudansvar och arbetsuppgifter:   
Bygga databas och API för att hantera data från sensorer och skicka vidare den i en användbar form till frontend.  
Databas för att spara vilka konferensrum som är bokade.

#### **Teamsamarbete och kunskapöverföring:**
API-dokumentation. Gemensam planering och arbete under både projektdagar och BR.  

___

### Teamwork och säker kunskapsöverföring:  
- Varje team ska efter sin gemensamma arbetstid (Boiler Room) lämna en rapport till de andra teamen så att alla hålls uppdaterade.
- Varje team (frontend, backend & IoT) har en teamleader, dessa tre har det yttersta ansvaret att synka mellan teamen.  
- Alla måste kommunicera inom gruppen och vara med på tisdagsmöten enligt gruppkontraktet.
___  

## Arbetsmetodik och verktyg:  
- Vilka verktyg som ska användas för:
  - Kodhantering: Github
  - Kommunikation: Slack
  - Sprint-planering: GitHub projects
- Standups/ceremonier/möten:
  - Arbetsmöte i stor grupp tisdagar kl 9.00.
  - Intern sprintplanering i Boiler Room-grupperna inklusive standups och retros.   
  - Vid behov ytterligare synkroniseringsmöten som delegeras av gruppledarna.
  

___  

## Tidsplan och milstolpar:  
- V. 10–11: Research, val av projekt (inkl.hårdvara)
- V. 12: Skapande av projektplan.Fredag 21/3: 15.00 Projektplanen skickas in för godkännande.
- V. 13: Paus!
- V. 14: Arbetsmöte. Arbetet i Boiler Room-grupperna börjar
- V. 15: Fullstack: Få upp endpoints till ett API med mockad data.
- V. 16: Implementering av kärnfunktioner.
- V. 17: MVP.
- V. 18: Fullstack: Integrerad databas och ersatt mockad data.
- V. 19:
- V. 20:
- V. 21:
- V. 22: Testning, förbättring och optimering. Fullstack: Implementerat säkerhet med JWT.
- V. 23: Projektavslut (demo, uppvisning, retro).

#### Hur hanterar teamet förändringar i tidsplanen om någor tar längre tid än förväntat?
Genom regelbundna möten, tydlig kommunikation och användande av agila metoder. Större förändringar stäms alltid av med beställaren.
______

## Riskanalys och plan för problemhantering: 
- **Vilka risker kan uppstå? (ex. tekniska utmaningar, brist på kommunikation, tidsbrist).**
  Bristande kommunikation om vi inte håller varandra uppdaterade, brist på förståelse vid olika termer.   
  Frontend kan behöva vänta på att backend ska bli klar med API:er.   
  Versionshantering där konflikter kan uppstå. Orealistiska deadlines samt olika arbetstempo kan också bli hinder.
- **Hur ska ni hantera problem som blockerar utvecklingen?**  
  Regelbundna standups och tydliga arbetsfördelningar. 
  Meddela i första hand ditt egna “team” om man stöter på problem, blir det ett större problem så meddela även de andra teamen.  
- **Vilka steg tar ni om en teammedlem inte levererar enligt plan?**  
  I första hand prata med den berörda, finns det någon bakomliggande anledning? Går det att lösa? 
____

## Leveranser och dokumenation:  
Slutproduktens minimikrav: Vad varje team (frontend, backend, IoT) måste leverera vid projektavslut
  - Fullstack:
    - API för datainsamling och hantering av händelser
    - Generera notifieringar till appen baserade på sensordata
    - Databas för lagring och analys
    - Autentisering och rollhantering 
  - Frontend:
    - Ta emot API från backend
    - WCAG Layout
    - Funktionella komponenter som pratar med backend och sensorerna
  - IoT:
    - Fungerande prototyp med mikrokontroller och sensorer som genererar och hanterar data
    - Program för mikrokontrollern som är skalbart och kan konvertera och hantera data som skickas till och från backend API
    - Dokumentation inklusive installation, användning och felsökning av IoT-delen av projektet.
    - Förslag på utformning av en slutlig produkt
    
- Vilken dokumentation som ska finnas och vem som ansvarar för den?
  - Varje team ansvarar för sin egen dokumentation.

- Plan för att hålla README och API-dokumentation uppdaterad
  - Varje vecka utse en person inom respektive team som ansvarar för dokumentation.


