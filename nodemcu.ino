/*Water level monitoring system with the New Blynk app
   https://srituhobby.com
*/
//Include the library files
//#include <LiquidCrystal_I2C.h>
#define BLYNK_PRINT Serial

#define BLYNK_TEMPLATE_ID "TMPL6vcQzZ2A5"
#define BLYNK_TEMPLATE_NAME "Water Level Indicator"
#define BLYNK_AUTH_TOKEN "SmDI9cOIES4kMz2Jzf9CqV4H-bxeFOSp"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

//Initialize the LCD display
//LiquidCrystal_I2C lcd(0x27, 16, 2);



char auth[] = "SmDI9cOIES4kMz2Jzf9CqV4H-bxeFOSp";//Enter your Auth token
char ssid[] = "DCS";//Enter your WIFI name
char pass[] = "poiuytrewq";//Enter your WIFI password

BlynkTimer timer;

// Define the component pins
#define trig D5
#define echo D6
#define LED1 D0
#define LED2 D3
#define LED3 D4
#define LED4 D8
#define LED5 D7
#define relay 3
#define watertap 1


//Enter your tank max value(CM)
int MaxLevel = 23;

int Level1 = (MaxLevel * 75) / 100;
int Level2 = (MaxLevel * 65) / 100;
int Level3 = (MaxLevel * 55) / 100;
int Level4 = (MaxLevel * 45) / 100;
int Level5 = (MaxLevel * 35) / 100;

void setup() {
  Serial.begin(9600);
 // lcd.init();
 // lcd.backlight();
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  pinMode(LED5, OUTPUT);
  pinMode(relay, OUTPUT);
  pinMode(watertap, OUTPUT);

  digitalWrite(relay, HIGH);
  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);

  //lcd.setCursor(0, 0);
   //println("Water level");
  //lcd.setCursor(4, 1);
 // lcd.print("Monitoring");
 // delay(4000);
 // lcd.clear();

  //Call the functions
  timer.setInterval(100L, ultrasonic);
}

//Get the ultrasonic sensor values
void ultrasonic() {
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  long t = pulseIn(echo, HIGH);
  long distance = t / 29 / 2;


  Serial.print(distance);
  Serial.println(" cm \t");

  int blynkDistance = (MaxLevel - distance) *100 / MaxLevel;
  if (distance <= MaxLevel) {
    Blynk.virtualWrite(V0, blynkDistance);
    Blynk.virtualWrite(V2,blynkDistance );
  } else {
    Blynk.virtualWrite(V0, 0);
  }
 // lcd.setCursor(0, 0);
  //Serial.println("WLevel:");

  if (Level1 <= distance) {

    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, LOW);
    digitalWrite(LED4, LOW);
    digitalWrite(LED5, LOW);
    Blynk.logEvent("low_water_level_alert");
    

  } else if (Level2 <= distance && Level1 > distance) {

    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, HIGH);
    digitalWrite(LED3, LOW);
    digitalWrite(LED4, LOW);
    digitalWrite(LED5, LOW);
  } else if (Level3 <= distance && Level2 > distance) {
 
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, HIGH);
    digitalWrite(LED3, HIGH);
    digitalWrite(LED4, LOW);
    digitalWrite(LED5, LOW);
  } else if (Level4 <= distance && Level3 > distance) {

    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, HIGH);
    digitalWrite(LED3, HIGH);
    digitalWrite(LED4, HIGH);
    digitalWrite(LED5, LOW);
  } else if (Level5 >= distance) {

    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, HIGH);
    digitalWrite(LED3, HIGH);
    digitalWrite(LED4, HIGH);
    digitalWrite(LED5, HIGH);
    digitalWrite(relay, LOW);
  }
}

//Get the button value
BLYNK_WRITE(V1) {
  bool Relay = param.asInt();
  if (Relay == 1) {
    digitalWrite(relay, LOW);
 
    Serial.println("Motor is ON ");
  } else {
    digitalWrite(relay, HIGH);
 
    Serial.println("Motor is OFF");
  }
}

BLYNK_WRITE(V3) {
  bool Watertap = param.asInt();
  if (Watertap == 1) {
    digitalWrite(watertap, LOW);
 
    Serial.println("water tap is ON ");
  } else {
    digitalWrite(watertap, HIGH);
 
    Serial.println("water tap is OFF");
  }
}



void loop() {
  Blynk.run();//Run the Blynk library
  timer.run();//Run the Blynk timer
}
