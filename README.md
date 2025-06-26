# Proiect Smart Home cu ESP32

Biblioteca WebServer permite crearea unui server web local pe ESP32 pentru afișarea datelor și controlul componentelor din browser.  
Biblioteca BH1750 permite citirea luminozității ambientale în lux de la senzorul BH1750 prin comunicație I2C.  
Biblioteca Wire este utilizată pentru comunicarea I2C între ESP32 și module precum senzori sau afișaje.  
Biblioteca Adafruit_BMP280 permite citirea temperaturii și presiunii de la senzorul BMP280 prin I2C.  
Biblioteca LiquidCrystal_I2C permite afișarea de date pe un ecran LCD cu interfață I2C, economisind pini.  
Biblioteca ESP32Servo controlează servo-motoare prin semnal PWM generat de ESP32.  

## Instalare

1. Se descarcă și se instalează Visual Studio Code de pe site-ul oficial: https://code.visualstudio.com/

2. După instalare, se deschide VS Code și se accesează secțiunea Extensions.

3. Din această secțiune, se caută extensia "PlatformIO IDE" și se instalează.

4. Se creează un proiect nou în PlatformIO, selectând placa "ESP32 Dev Module" și framework-ul "Arduino".

5. În fișierul generat automat platformio.ini, se adaugă următoarea configurație:

```
[env:upesy_wroom]
platform = espressif32	
board = esp32dev
framework = arduino
monitor_speed = 115200
lib_deps = 
	esphome/ESPAsyncWebServer-esphome@^3.3.0
	claws/BH1750@^1.3.0
	adafruit/Adafruit BMP280 Library@^2.6.8
	marcoschwartz/LiquidCrystal_I2C@^1.1.4
	madhephaestus/ESP32Servo@^3.0.6
```

Această configurație setează placa utilizată, viteza de comunicare pe portul serial și include automat bibliotecile necesare pentru funcționarea componentelor precum: senzorul de lumină BH1750, senzorul de temperatură și presiune BMP280, afișajul LCD I2C, servo motorul și serverul web asincron.

## Pașii de compilare

După adăugarea codului în PlatformIO:

1. Trebuie să se apese pe butonul de **Upload** din bara PlatformIO sau să se folosească comanda rapidă: `Ctrl + Alt + U`.

2. Ulterior, pentru a intra în website, se intră în **Serial Monitor**, unde se va afișa link-ul de conectare la rețeaua locală a ESP32.
