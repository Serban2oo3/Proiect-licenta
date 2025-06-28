https://github.com/Serban2oo3/Proiect-licenta.git
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


## Codul de hardware
```
#include <WiFi.h>       
#include <WebServer.h> 
#include <BH1750.h>
#include <Wire.h>
#include <Adafruit_BMP280.h>
#include<LiquidCrystal_I2C.h>
#include <ESP32Servo.h>
#include "site.h"



#define pin_led_camera 2
#define pin_led_sufragerie 4
#define pin_led_fata 27
#define pin_led_ac 14
#define pin_led_ca 17

#define pin_pir_fata 26
#define pin_pir_alarma 34


#define pin_alarama 32

#define pin_buton2 18
#define pin_buton 19

#define pin_servo_usa 33
#define pin_servo_graraj 13

#define timp 5000

Adafruit_BMP280 bmp;
BH1750 lumina;
LiquidCrystal_I2C lcd(0x27, 16, 3);



unsigned long ultima_apasare_buton_timp = 0 , ultima_citire=0 ,alarma_pornire=0, lumina_fata_stare=0 , usa_fata;
bool LED1 = false , LED2 = false ,LED3 = false, buzzON =false , AC=false, CA=false;
uint32_t SensorUpdate = 0;


float LUX=0 ,TEMPERATURA=0, PRESIUNE=0, ALTITUDINE=0;
float temperatura_falsa = 0 , temperatura_adevarata=0;

int valoare_pir;
int ultima_valoare_pir = LOW;

Servo myServo;
bool servoAt120 = false;     
bool buttonHandled2 = false; 

Servo myServo1;
bool servoAt90 = false;     
bool buttonHandled = false; 








char fisier_transmitere_date[4096];
IPAddress ip_casei;
WebServer server(80);

void Internet_Status();
void Trimite_web();
void Ecran_lcd();
void lumina_fata();
void alarma();
void usa();
void garaj();

void Lumina_camera();
void Caldura();
void Aer_conditionat();
void Lumina_sufragerie() ;
void Trimite_XML();

void setup()
{
    Serial.begin(115200);

    Wire.begin();
    lumina.begin();
    bmp.begin(0x76);
    lcd.init();  
    lcd.backlight();

  

    pinMode(pin_led_camera, OUTPUT);
    LED1 = false;
    digitalWrite(pin_led_camera, LED1);

    pinMode(pin_led_sufragerie, OUTPUT);
    LED2 = false;
    digitalWrite(pin_led_sufragerie, LED2);

    pinMode(pin_led_fata, OUTPUT);
    
    pinMode(pin_pir_fata, INPUT);

    pinMode(pin_pir_alarma, INPUT);
    pinMode(pin_alarama, OUTPUT);
    digitalWrite(pin_alarama, LOW);

    pinMode(pin_led_ac, OUTPUT);
    AC = false;
    digitalWrite(pin_led_ac, AC);

    pinMode(pin_led_ca, OUTPUT);
    CA = false;
    digitalWrite(pin_led_ca,CA);

     pinMode(pin_buton2, INPUT_PULLUP);  
    myServo.attach(pin_servo_usa);
    myServo.write(20);  

     pinMode(pin_buton, INPUT_PULLUP);  
    myServo1.attach(pin_servo_graraj);
    myServo1.write(90);  



    WiFi.begin(Numele_retelei, parola_wifi);
    while(WiFi.status()!=WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Internet_Status();
    server.on("/", Trimite_web);

    server.on("/LUMINA_1", Lumina_camera);
    server.on("/LUMINA_2", Lumina_sufragerie);
    server.on("/AC", Aer_conditionat);
    server.on("/CA", Caldura);
    server.on("/xml", Trimite_XML);

    server.begin();


}

void loop()
{
    Ecran_lcd();
    lumina_fata();
    alarma();
    usa();
    garaj();
  
   
    
    


    server.handleClient();
}

void Ecran_lcd()
{
    if (millis() - ultima_citire > 2000) 
    {   
    ultima_citire = millis();
    LUX = lumina.readLightLevel();

    temperatura_adevarata = bmp.readTemperature();
    if(AC& CA)
    {
      temperatura_falsa=temperatura_adevarata;
    }
    else if(AC)
    {
      temperatura_falsa=temperatura_adevarata-5;
    }
    else if(CA)
    {
      temperatura_falsa=temperatura_adevarata+5;
    }
    else
    {
      temperatura_falsa=temperatura_adevarata;
    }
 
  
    TEMPERATURA =  temperatura_falsa;

    PRESIUNE = bmp.readPressure();
    ALTITUDINE = bmp.readAltitude(1016);
    }
     lcd.setCursor(0, 0);
     lcd.print("LUX: ");
    lcd.print(LUX);

    lcd.setCursor(0, 1);
     lcd.print("Temp: ");
     lcd.print(TEMPERATURA);

    lcd.setCursor(0 , 2);
    lcd.print("Presiune: ");
    lcd.print(PRESIUNE/1000);

    lcd.setCursor(0 , 3);
    lcd.print("Altitudine: ");
    lcd.print(ALTITUDINE);
}

void lumina_fata()
{
    valoare_pir = digitalRead(pin_pir_fata);

   if( valoare_pir== HIGH && !LED3)
   {
    LED3= true;
    lumina_fata_stare = millis();
    digitalWrite(pin_led_fata, HIGH);
   }
   if(LED3 && millis() - lumina_fata_stare > 10000)
   {
    LED3 = false;
    digitalWrite(pin_led_fata, LOW);
   }
}
void alarma()
{
    bool motionDetected = (digitalRead(pin_pir_alarma) == HIGH);
    if (motionDetected && !buzzON) {
    tone(pin_alarama, 1000); 
    buzzON = true;
    alarma_pornire = millis();
   
  }

 
  if (buzzON && (millis() - alarma_pornire >= timp)) {
    noTone(pin_alarama);
    buzzON = false;
    
  }
}

void usa()
{
  if (digitalRead(pin_buton2) == LOW && !buttonHandled2) {
    buttonHandled2 = true;

    if (!servoAt120) {
      Serial.println("Mut la 120°");
      myServo.write(120);
      servoAt120 = true;
    } else {
      Serial.println("Mut la 20°");
      myServo.write(20);
      servoAt120 = false;
    }

    delay(5000);  
  }

 
  if (digitalRead(pin_buton2) == HIGH && buttonHandled2) {
    buttonHandled2 = false;
  }

  delay(10);  
}

void garaj()
{
  if (digitalRead(pin_buton) == LOW && !buttonHandled) {
    buttonHandled = true;

    if (!servoAt90) {
      Serial.println("Mut la 120°");
      myServo1.write(0);
      servoAt90 = true;
    } else {
      Serial.println("Mut la 20°");
      myServo1.write(90);
      servoAt90 = false;
    }

    delay(5000); 
  }

  
  if (digitalRead(pin_buton) == HIGH && buttonHandled) {
    buttonHandled = false;
  }

  delay(10);  
}








void Internet_Status()
{
    ip_casei = WiFi.localIP();
     Serial.print("Open http://");
     Serial.println(ip_casei);
   
}

void Trimite_web() {


 
  server.send(200, "text/html", PAGE_MAIN);
}

void Lumina_camera() {
  LED1 = !LED1;
  digitalWrite(pin_led_camera, LED1);
  Serial.print("Camera lumina "); Serial.println(LED1);
  if(LED1) {
    server.send(200, "text/plain", "1");
  }
  else {
    server.send(200, "text/plain", "0");
  }
}

void Caldura() {
  CA=!CA;
  digitalWrite(pin_led_ca, CA);
  Serial.print("Caldura : "); Serial.println(CA);
  if(CA) {
    server.send(200, "text/plain", "1");
  }
  else {
    server.send(200, "text/plain", "0");
  }
}

void Aer_conditionat() {
  AC=!AC;
  digitalWrite(pin_led_ac, AC);
  Serial.print("Aer conditionat : "); Serial.println(AC);
  if(AC) {
    server.send(200, "text/plain", "1");
  }
  else {
    server.send(200, "text/plain", "0");
  }
}

void Lumina_sufragerie() {
  LED2 = !LED2;
  digitalWrite(pin_led_sufragerie, LED2);
  Serial.print("Lumina sufragerie "); Serial.println(LED2);
  if(LED2) {
    server.send(200, "text/plain", "1");
  }
  else {
    server.send(200, "text/plain", "0");
  }
}

void Trimite_XML() {
 
  Serial.println("sending xml");

    snprintf(fisier_transmitere_date, sizeof(fisier_transmitere_date), "<?xml version='1.0'?>\n<Data>\n<LED>%d</LED>\n", LED1 ? 1 : 0);
    snprintf(fisier_transmitere_date + strlen(fisier_transmitere_date), sizeof(fisier_transmitere_date) - strlen(fisier_transmitere_date), "<LED2>%d</LED2>\n", LED2 ? 1 : 0);
    snprintf(fisier_transmitere_date + strlen(fisier_transmitere_date), sizeof(fisier_transmitere_date) - strlen(fisier_transmitere_date), "<LUX>%.2f</LUX>\n", LUX);
    snprintf(fisier_transmitere_date + strlen(fisier_transmitere_date), sizeof(fisier_transmitere_date) - strlen(fisier_transmitere_date), "<TEMPERATURE>%.2f</TEMPERATURE>\n", TEMPERATURA);
    snprintf(fisier_transmitere_date + strlen(fisier_transmitere_date), sizeof(fisier_transmitere_date) - strlen(fisier_transmitere_date), "<PRESSURE>%.2f</PRESSURE>\n", PRESIUNE/1000);
    snprintf(fisier_transmitere_date + strlen(fisier_transmitere_date), sizeof(fisier_transmitere_date) - strlen(fisier_transmitere_date), "<ALTITUDE>%.2f</ALTITUDE>\n", ALTITUDINE);
    snprintf(fisier_transmitere_date + strlen(fisier_transmitere_date), sizeof(fisier_transmitere_date) - strlen(fisier_transmitere_date), "<CA>%d</CA>\n", CA ? 1 : 0);
    snprintf(fisier_transmitere_date + strlen(fisier_transmitere_date), sizeof(fisier_transmitere_date) - strlen(fisier_transmitere_date), "<AC>%d</AC>\n", AC ? 1 : 0);
    snprintf(fisier_transmitere_date + strlen(fisier_transmitere_date), sizeof(fisier_transmitere_date) - strlen(fisier_transmitere_date), "<LED3>%d</LED3>\n",LED3 ? 1 : 0 );
    

    snprintf(fisier_transmitere_date + strlen(fisier_transmitere_date), sizeof(fisier_transmitere_date) - strlen(fisier_transmitere_date), "</Data>\n");
 
  Serial.println(fisier_transmitere_date);

  server.send(200, "text/xml", fisier_transmitere_date);


}
```
## Codul de software
```
        const char PAGE_MAIN[] PROGMEM = R"=====(
        <!DOCTYPE html>
        <html>

        <head>
            <meta charset ="utf-8">
            <meta name ="viewport" content="width=device-width, initial-scale=1">
            <title>Smart.HM</title>
        </head>

        <style>
            body{
                background-color : #3eebff;
                margin : 0;
                color:black;
                display: flex;
                flex-direction :column;
                align-items: center;
            }
            header{
                width:35%;
                padding : 20px 0;
                background-color: white;
                text-align:  center;
                border-radius: 20px;
                box-shadow: 0 5px 11px rgba(0,0,0,0.3);
            }
            header h1{
                font-size: 24px;
                margin: 15px;
                color: black;;
                letter-spacing: 2px;
            }
            .iluminare{
                display :flex;
                justify-content: center;
                gap: 50px;
                margin-top: 30px;
            }
            .box{
                width :200px;
                height: 200px;
                background-color: white ;
                color :black;
                border-radius: 20px;
                box-shadow: 0 5px 11px rgba(0,0,0,0.3);
                padding : 15 px;
                text-align : center;
                position: relative;
            }
            .box p
            {
                font-size: 20px;
                
                margin-bottom:15px;
            }
        .buton_on_off
            {
                position: relative;
                display: inline-block;
                width: 70px;
                height: 36px;
            }

            .buton_on_off input
            {
                opacity: 0.1;
                width: 0.1;
                height: 0.1;
            }

            .glisare   
            {
                position: absolute;
                    top: 0;
                left: 0;
                right: 0;
                bottom: 0;
                background: linear-gradient(145deg, #d1d1d1, #f0f0f0);
                border-radius: 40px;
                transition: background-color 0.3s ease-in-out;
                box-shadow: inset 0 0 5px rgba(0,0,0,0.2);
            }

            .glisare::before 
            {
                content: "";
                position: absolute;
                height: 28px;
                width: 28px;
                left: 4px;
                top: 4px;
                background-color: #ffffff;
                border-radius: 50%;
                transition: transform 0.3s ease-in-out, background-color 0.3s;
                box-shadow: 0 2px 5px rgba(0, 0, 0, 0.2);
            }           

            input:checked + .glisare
            {
                background: linear-gradient(145deg, #4facfe, #00f2fe);
                box-shadow: 0 0 5px #4facfe, 0 0 15px #00f2fe;
            }

            input:checked + .glisare::before
            {
                transform: translateX(34px);
                background-color: #e0f7ff;
            }

            input:checked + .glisare
            {
                background-color: #2196F3;
            }
            input:checked + .glisare:before
            {
                transform: translateX(28px);
            }
            .section-header {
                font-size :24px;
                width:35%;
                padding : 20px 0;
                font-size: 24px;
                margin-top: 20px;
                margin-bottom: 20px;
                background-color: white;
                text-align:  center;
                border-radius: 20px;
                box-shadow: 0 5px 11px rgba(0,0,0,0.3);
            }
            table{
                width: 50%;
                border-collapse: collapse;
                margin-bottom : 40px;
                background-color: rgb(193, 193, 193);
                color :rgb(17, 12, 12);
                
                overflow: hidden;
                border-radius: 27px;
                box-shadow: 1 5px 11px rgba(7, 1, 1, 0.3);
            }
            th, td {
            padding: 15px;
            text-align: center;
            font-size: 18px;
            }

            th {
            background-color: #3c465a;
            color: white;
            }

            tr:nth-child(even) {
            background-color: #f2f2f2;
            }

            tr:hover {
            background-color: #ddd;
            }
            .temp-control {
            display: flex;
            justify-content: center;
            gap: 50px;
            margin-bottom: 40px;
            margin-top: 30px;
        }



        </style>
        

        <body>
            <header><h1>Light monitor</h1></header>


            <div class="iluminare">
                <div class="box">
                    <p>Lumina sufragerie</p>
                    <label class="buton_on_off">
                        <input type ="checkbox" id="led1" onchange="Switch_Lumina_1 ()">
                        <span class="glisare"></span>
                    </label>

                </div>

                <div class="box">
                    <p>Lumina garaj</p>
                    <label class="buton_on_off">
                        <input type ="checkbox" id="led2" onchange="Switch_Lumina_2 ()">
                        <span class="glisare"></span>
                    </label>

                </div>

                <div class="box">
                    <p>Lumina din fata</p>
                    <p id="led3status">Status: Necunoscut</p>
                </div>
            </div>

            <div class="section-header">Sensor Monitor</div>

            <table>
                <thead>
                    <tr>
                        <th><div class="heading">Sensor</div></th>
                        <th><div class="heading">Valoare</div></th>
                    </tr>
                </thead>
                <tbody>
                    <tr>
                        <td><div class="titlutabel">Luminozitate</div></td>
                        <td><div class="valorilesenzoriilor" id="lux"></div></td>
                    </tr>
                    <tr>
                        <td><div class="titlutabel">Temperatură</div></td>
                        <td><div class="valorilesenzoriilor" id="temp"></div></td>
                    </tr>
                    <tr>
                        <td><div class="titlutabel">Presiune</div></td>
                        <td><div class="valorilesenzoriilor" id="presiune"></div></td>
                    </tr>
                    <tr>
                        <td><div class="titlutabel">Altitudine</div></td>
                        <td><div class="valorilesenzoriilor" id="alt"></div></td>
                    </tr>
                </tbody>
            </table>


            <div class="temp-control">
            <div class="box">
                <p>Răcire</p>
                <p style="font-size: 14px;">Va scădea temperatura cu aproximativ 5°C</p>
                <label class="buton_on_off">
                    <input type="checkbox" id="ac" onchange="Racire()">
                    <span class="glisare"></span>
                </label>
            </div>

            <div class="box">
                <p>Încălzire</p>
                <p style="font-size: 14px;">Va crește temperatura cu aproximativ 5°C</p>
                <label class="buton_on_off">
                    <input type="checkbox" id="ca" onchange="Incalzire()">
                    <span class="glisare"></span>
                </label>
            </div>
        </div>

            </div>

        </body>

        <script type ="text/javascript">
            var xmlHttp = XMLObject();

            function XMLObject()
            {
                if (window.XMLHttpRequest){
                    xmlHttp= new XMLHttpRequest();
                }
                else{
                    xmlHttp= new ActiveXObject("Microsoft.XMLHTTP")
                }
                return xmlHttp;
            }

            function Switch_Lumina_1()
            {
                var XL= new XMLHttpRequest();
                var message;
                XL.onreadystatechange=function()
                {
                if(this.readyState==4 && this.status==200)
                message=this.responseText;
                }
                XL.open("PUT", "LUMINA_1", false);
                XL.send();
            }

            function Switch_Lumina_2()
            {
                var XL= new XMLHttpRequest();
                var message;
                XL.onreadystatechange=function()
                {
                if(this.readyState==4 && this.status==200)
                message=this.responseText;
                }
                XL.open("PUT", "LUMINA_2", false);
                XL.send();
            }

            function Incalzire()
            {
                var XL= new XMLHttpRequest();
                var message;
                XL.onreadystatechange=function()
                {
                if(this.readyState==4 && this.status==200)
                message=this.responseText;
                }
                XL.open("PUT", "CA", false);
                XL.send();
            }

            function Racire()
            {
                var XL= new XMLHttpRequest();
                var message;
                XL.onreadystatechange=function()
                {
                if(this.readyState==4 && this.status==200)
                message=this.responseText;
                }
                XL.open("PUT", "AC", false);
                XL.send();
            }

            

            function response()
            {
                if(xmlHttp.readyState==4 && xmlHttp.status==200)
                {
                    try
                    {
                        //sistem iluminare
                        var xmlResponse=xmlHttp.responseXML;
                        if(xmlResponse &&xmlResponse.getElementsByTagName("led1").length>0)
                        {
                            var led1Status = xmlResponse.getElementsByTagName("LED")[0].firstChild.nodeValue;

                        }

                        if(xmlResponse &&xmlResponse.getElementsByTagName("led2").length>0)
                        {
                            var led2Status = xmlResponse.getElementsByTagName("LED2")[0].firstChild.nodeValue;

                        }

                        //led afara
                        var led3 = xmlResponse.getElementsByTagName("LED3")[0].childNodes[0].nodeValue;
                        var statusText = (led3 === "1") ? "Lumina este aprinsă" : "Lumin este stinsă";
                        document.getElementById("led3status").innerHTML = statusText;


                        //aici este tabelu +senzorii

                        if(xmlResponse.getElementsByTagName("LUX")[0])
                        document.getElementById("lux").textContent =xmlResponse.getElementsByTagName("LUX")[0].textContent;

                        if(xmlResponse.getElementsByTagName("TEMPERATURE")[0])
                        document.getElementById("temp").textContent = xmlResponse.getElementsByTagName("TEMPERATURE")[0].textContent;

                        if(xmlResponse.getElementsByTagName("PRESSURE")[0])
                        document.getElementById("presiune").textContent = xmlResponse.getElementsByTagName("PRESSURE")[0].textContent;

                        if(xmlResponse.getElementsByTagName("ALTITUDE")[0])
                        document.getElementById("alt").textContent = xmlResponse.getElementsByTagName("ALTITUDE")[0].textContent;



                        //siste de racire/incalzire
                        if(xmlResponse &&xmlResponse.getElementsByTagName("ca").length>0)
                        {
                            var led3Status = xmlResponse.getElementsByTagName("CA")[0].firstChild.nodeValue;

                        }

                        if(xmlResponse &&xmlResponse.getElementsByTagName("ac").length>0)
                        {
                            var led4Status = xmlResponse.getElementsByTagName("AC")[0].firstChild.nodeValue;

                        }




                    } catch(e){console.error("Eroare la parsarea XML:", e);}

                }
            }

            function process()
            {
                if(xmlHttp.readyState==0 || xmlHttp.readyState==4)
                {
                    xmlHttp.open("PUT", "xml" , true);
                    xmlHttp.onreadystatechange=response;
                    xmlHttp.send(null);
                }
                setTimeout("process()", 1000);

            }
            window.onload = process;

        </script>



        </html>


        )=====";
```
