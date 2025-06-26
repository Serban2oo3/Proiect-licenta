#include <WiFi.h>       
#include <WebServer.h> 
#include <BH1750.h>
#include <Wire.h>
#include <Adafruit_BMP280.h>
#include<LiquidCrystal_I2C.h>
#include <ESP32Servo.h>
#include "site.h"

#define Numele_retelei "Vodafone-C8ED"
#define parola_wifi "Af32Y7jpsfhkBbuu"

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
Servo servomotor;
Servo servomotor_usa;


unsigned long ultima_apasare_buton_timp = 0 , ultima_citire=0 ,alarma_pornire=0, lumina_fata_stare=0 , usa_fata;
bool LED1 = false , LED2 = false ,LED3 = false, buzzON =false , AC=false, CA=false;
uint32_t SensorUpdate = 0;


float LUX=0 ,TEMPERATURA=0, PRESIUNE=0, ALTITUDINE=0;
float temperatura_falsa = 0 , temperatura_adevarata=0;

int valoare_pir;
int ultima_valoare_pir = LOW;

Servo myServo;
bool servoAt120 = false;     // Poziție actuală
bool buttonHandled2 = false; 

Servo myServo1;
bool servoAt90 = false;     // Poziție actuală
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

    delay(5000);  // Așteaptă 5 secunde între comenzi
  }

  // Așteaptă eliberarea butonului
  if (digitalRead(pin_buton2) == HIGH && buttonHandled2) {
    buttonHandled2 = false;
  }

  delay(10);  // debounce
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

    delay(5000);  // Așteaptă 5 secunde între comenzi
  }

  // Așteaptă eliberarea butonului
  if (digitalRead(pin_buton) == HIGH && buttonHandled) {
    buttonHandled = false;
  }

  delay(10);  // debounce
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