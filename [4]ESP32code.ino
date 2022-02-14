#define BLYNK_PRINT Serial
#include<DHT.h>
#include<WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

char auth[] = "ZwuV8unD9nmWfab2XDdSwVdpqGNH6p7D";
String apiKey ="V0XWFN7SRQTUPGEL";
const char *ssid= "IIoT Center SIM 2.4G";
const char *pass= "iiot0321";
const char *server= "api.thingspeak.com";

const int trigPin = 12;
const int echoPin = 14;
#define DHTPIN 16      // What digital pin we're connected to DHT11
DHT dht(DHTPIN, DHT11);
const int ledRed = 26; // temperature's led
const int ledGreen = 27; // Distance's led
WiFiClient client;
long duration;
int distance;
void setup(){
  pinMode(trigPin,OUTPUT); // Sets the trigpin as an output
  pinMode(echoPin,INPUT); // Sets the echoPin as an Input
  pinMode(ledRed,OUTPUT); // temperature's LED
  pinMode(ledGreen,OUTPUT);// Distance's LED
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
  float h =dht.readHumidity();
  float t =dht.readTemperature();
  WidgetLCD lcd(V2);
  digitalWrite(trigPin, LOW);
  delayMicroseconds (2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds (10);
  digitalWrite(trigPin, LOW);// Reads the echopin, returns the sound wave travel time in microseconds
  duration=pulseIn(echoPin,HIGH);//calculating the distance
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

//12thbn3245 64 thingspeak needs minimum 15 sec delay between updates, i've set it to 30 seconds
  delay(1000);
  if (distance< 10){
    digitalWrite(ledGreen,HIGH);
    lcd.clear();
    lcd.print(0,0, "The Bin is Full");
    delay(1000);
   }
   else{
    digitalWrite(ledGreen,LOW);
    lcd.clear();
    lcd.print(0,0, "The Bin is empty");
    delay(1000);
   }
  
   if (t>28){  
  digitalWrite(ledRed,HIGH);
  lcd.clear();
  lcd.print(0,0, "The Bin is Overheated");
  delay(1000);
   }
  else{
       digitalWrite(ledRed,LOW);
  lcd.clear();
    lcd.print(0,0, "      ");
    delay(1000);
  }
  Blynk.virtualWrite(V0,distance);
  Blynk.virtualWrite(V1,t);
  Blynk.run();
}        
