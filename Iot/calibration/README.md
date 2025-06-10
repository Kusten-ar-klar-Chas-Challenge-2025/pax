## Kalibrering av temperatur-sensor

PAX Temperatur-sensor kan ibland behöva kalibreras  

Enheten lyssnar på meddelanden över Serial, Baud Rate: 115200

- `TEMP0`         Nollställer offset  
- `TEMP+<value>`  Sätter positiv offset, exempel: `TEMP+2`  
- `TEMP-<value>`  Sätter negativ offset, exempel: `TEMP-3.5`  

För att underlätta för administratörer att kalibrera sensorn följer det med en självdokumenterande terminal-app `send_temperature_offset` som automatiskt kopplar upp sig till PAX-enheten. Denna hjälp-app finns som källkod (i Go) i `/src` och färdigkompilerad körbar fil i `/bin` för windows och osx (Silicon/ARM).

### Windows 
Navigera till `calibration/bin/windows` och kör `send_temperature_offset.exe` för instruktioner  

### OSX 
Navigera till `calibration/bin/osx` och kör `./send_temperature_offset` för instruktioner  
