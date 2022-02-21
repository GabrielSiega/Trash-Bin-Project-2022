#define BLYNK_PRINT Serial
#include <ESP32Servo.h>
#include<DHT.h>
#include<WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

Servo myservo; // create servo object to control a servo

char auth[] = "ZwuV8unD9nmWfab2XDdSwVdpqGNH6p7D";
String apiKey ="Z6WMPT0PZ7YK3F82";
const char *ssid= "IIoT Center SIM 2.4G";
const char *pass= "iiot0321";
//const char *ssid= "Gabriel's iPhone 12Pro Max";
//const char *pass= "Te@mSieg@2004";
const char *server= "api.thingspeak.com";

int pos = 0; // variable to store the servo position
int servoPin = 13;
const int trigPin0 = 12;
const int echoPin0 = 14;
const int trigPin1 = 25;
const int echoPin1 = 26;
#define DHTPIN 27 // What digital pin we're connected to DHT11
DHT dht(DHTPIN, DHT11);
const int ledRed = 5; // temperature's led
const int ledGreen = 17; // Trash Level led
const int ledYellow = 16; // Humidity's led
const int ledBlue = 4; // Lid led

WiFiClient client;
long duration, distance, duration1, distance1;

void setup(){
  pinMode(trigPin0, OUTPUT); // Sets the trigpin as an output
  pinMode(echoPin0, INPUT); // Sets the echoPin as an Input
  pinMode(trigPin1, OUTPUT); 
  pinMode(echoPin1, INPUT);
  pinMode(ledRed, OUTPUT);// temperature's LED
  pinMode(ledGreen, OUTPUT); // Distance's LED
  pinMode(ledYellow, OUTPUT);   // Humdity's LED
  pinMode(ledBlue, OUTPUT);   // Lid LED


  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  myservo.setPeriodHertz(50); // standard 50hz servo
  myservo.attach(servoPin, 500, 2400);

  Serial.begin(115200);//Starts the serial communication
  Blynk.begin(auth, ssid, pass);
  delay(100);
  dht.begin();
  Serial.println("Connecting to");
  Serial.println(ssid);
  WiFi.begin(ssid,pass);
  while (WiFi.status()!=WL_CONNECTED)
  {
    delay(1000);
    Serial.print(".");
  }
    Serial.println("");
 Serial.println("WiFi connected");
  }
  
 void loop(){


  digitalWrite(trigPin1, LOW);
  delayMicroseconds (2);
  digitalWrite(trigPin1, HIGH);
  delayMicroseconds (10);
  digitalWrite(trigPin1, LOW);
  duration1 = pulseIn(echoPin1, HIGH);
  distance1=duration1*0.034/2;
  Serial.print("Distance1:");
  Serial.print(distance1);

  
  float h =dht.readHumidity();
  float t =dht.readTemperature();
  WidgetLCD lcd(V2);
  digitalWrite(trigPin0, LOW);
  delayMicroseconds (2);
  digitalWrite(trigPin0, HIGH);
  delayMicroseconds (10);
  digitalWrite(trigPin0, LOW);// Reads the echopin, returns the sound wave travel time in microseconds
  duration=pulseIn(echoPin0,HIGH);//calculating the distance
  distance=duration*0.034/2;//Prints the distance on the Serial Monitor
  Serial.print("Distance:");
  Serial.print(distance);

  
      if (isnan(h) || isnan(t))
           {
              Serial.println("Failed to read from DHT sensor!");
               return;
           }
           
           if(client.connect(server,80))
           {
                String postStr = apiKey;
                postStr +="&field1=";
                postStr += String(t);
                postStr +="&field2=";
                postStr += String(h);
                postStr +="&field3=";
                postStr += String(distance);                
                postStr +="\r\n\r\n";
               
                client.print("POST /update HTTP/1.1\n");
                client.print("Host: api.thingspeak.com\n");
                client.print("Connection: close\n");
                client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
                client.print("Content-Type: application/x-www-form-urlencoded\n");
                client.print("Content-Length: ");
                client.print(postStr.length());
                client.print("\n\n");
                client.print(postStr);

                delay(500);

                Serial.print("Temperature: ");
                Serial.print(t);
                Serial.print(" degrees Celcius, Humidity: ");
                Serial.print(h);
                Serial.print("distance: ");
                Serial.print(distance);

                delay(500);

                Serial.print("Temperature: ");
                Serial.print(t);
                Serial.print(" degrees Celcius, Humidity: ");
                Serial.print(h);
                Serial.print("distance: ");
                Serial.print(distance);
                Serial.print("%. Send to Thingspeak.");

            }
 
  client.stop();

  Serial.println("Waiting...");

  if (distance1 <=10) { 
  
  myservo.attach(servoPin);
  delay(1);
  myservo.write(180);
  digitalWrite(ledBlue,HIGH);
   delay(1000);
 }

 else {
  myservo.write(0);
  digitalWrite(ledBlue,LOW);
  delay(1000);
  
 }


  // thingspeak needs minimum 15 sec delay between updates, i've set it to 30 seconds
  delay(1000);
  if (distance< 3){
    Blynk.notify("The bin is FULL!"); 
    digitalWrite(ledGreen,HIGH);                       //Green Led
    lcd.clear();
    lcd.print(0,0, "The Bin is Full");
    delay(1000);
   }
   else{
    digitalWrite(ledGreen,LOW);
    delay(1000);
   }
   if (distance < 7) {
    lcd.clear();
    lcd.print(0,0, "The bin is HALF full");
   }
      else{
    digitalWrite(ledGreen,LOW);
    delay(1000);
   }
   if (distance > 12) {
    lcd.clear();
    lcd.print(0,0, "The bin is EMPTY!");
    delay(1000);
   }

   if (t>40){  
    Blynk.notify("Abnormal Temp!");
  digitalWrite(ledRed,HIGH);                           //Red Led
  lcd.clear();
  lcd.print(0,0, "The Bin is Overheated");
  delay(1000);
   }
  else{
  digitalWrite(ledRed,LOW);
  lcd.clear();
    lcd.print(0,0, "Bin Temperature OK");
    delay(1000);
  }
  if (h>70){
    digitalWrite(ledYellow, HIGH);                     //Yellow Led
    lcd.clear();
    lcd.print(0,0, "High Humidity, Remove Trash");
    lcd.print(0,1, "To Prevent Odour");
    delay(1000);
  }
  else {
    digitalWrite(ledYellow, LOW);
    lcd.clear();
    lcd.print(0,0, "Humidity OK");
    delay(1000);
  }
  Blynk.virtualWrite(V0,distance);
  Blynk.virtualWrite(V1,t);
  Blynk.virtualWrite(V3,h);
  Blynk.run();
}           
                
  
