# Testning
## Teststrategi on device
Eftersom den enhetstestingsstrategi vi påbörjade med interfaces och hårdvaruabstrahering var lite för avancerad fokuserade vi på funktionalitetstestning.
Mainprogrammet utformades för att uppnå det önskade beteendet. Om vi kunde observera beteendet klassades det som fungerande. 
### Observerande av beteende
Beteendet kan observeras på olika sätt beroende på vad som ska undersökas. Exempelvis vill vi undersöka att strängar konstrueras korrekt och att http-meddelanden returnerar korrekta värden och kroppar. Dessa skriva ut via Serial.
### Olika utskriftsnivåer
För att inte skriva ut för mycket information läggs debugutskrifer inom olika define-makron. Dessa kan sen defineras som byggmål.
Vi gjorde en debug-profil som skriver ut all information. Vid behov kan olika profiler skapas för specifika scenarion.