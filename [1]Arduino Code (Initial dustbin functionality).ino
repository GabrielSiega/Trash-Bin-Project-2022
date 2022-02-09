#include <Servo.h>   //servo library
Servo servo;  
int servoPin = 8;
#define trigPin1 A1
#define echoPin1 A0
#define trigPin2 A3
#define echoPin2 A2
#define trigPin3 A5
#define echoPin3 A4
int ALARM = 7;


#define LEDbluemove 13
#define LEDGreenquarterfull 12
#define LEDYellow2quartfull 11
#define LEDOrange3quartfull 10
#define LEDRedfull 9



long duration, distance, FIRSTSensor,SECONDSensor,THIRDSensor;
void setup()
{

Serial.begin (9600);
 
servo.attach(servoPin);   

pinMode(trigPin1, OUTPUT);
pinMode(echoPin1, INPUT);
pinMode(trigPin2, OUTPUT);
pinMode(echoPin2, INPUT);
pinMode(trigPin3, OUTPUT);
pinMode(echoPin3, INPUT);
pinMode(ALARM, OUTPUT);
digitalWrite(ALARM, LOW);

servo.write(0);         //close cap on power on
delay(100);
servo.detach();
  
  
  
//int LEDbluemove = 4;          //Leds
//int LEDGreenquarterfull = 3;
//int LEDYellow2quartfull = 2;
//int LEDOrange3quartfull = A5;
//int LEDRedfull = A4;  
  
  
  
pinMode(LEDbluemove,OUTPUT);  
pinMode(LEDGreenquarterfull,OUTPUT);   
pinMode(LEDYellow2quartfull,OUTPUT);   
pinMode(LEDOrange3quartfull,OUTPUT);   
pinMode(LEDRedfull,OUTPUT);   
  
  
}

void loop() 
{
/////////////////////////////////////////////////////// 
SonarSensor(trigPin1, echoPin1);
FIRSTSensor = distance;
SonarSensor(trigPin2, echoPin2);
SECONDSensor = distance;
SonarSensor(trigPin3, echoPin3);
THIRDSensor = distance;
/////////////////////////////////////////////////////////////
digitalWrite(ALARM, LOW);
//////////////////////////////////////////////////////////////////////////////////////
Serial.print("S1:");Serial.println(FIRSTSensor); delayMicroseconds(10000);
Serial.print("S2:");Serial.println(SECONDSensor);delayMicroseconds(10000);
Serial.print("S3:");Serial.println(THIRDSensor); delayMicroseconds(10000);
///////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////
if((FIRSTSensor >= 10) & (FIRSTSensor <= 50)) 
{digitalWrite(ALARM, HIGH);delay(1000); servo.attach(servoPin);
  delay(1);
 servo.write(0);  
 delay(3000);                       ////LID SENSOR/////!
 servo.write(150);    
 delay(1000);
 servo.detach();
digitalWrite( LEDbluemove, HIGH);
Serial.println("Blue ON"); 
}
else 
{
digitalWrite( LEDbluemove, LOW);
Serial.println("Blue ON");
}  
  
  
//// IGNORE "1st Volume Sensor & 2nd Volume sunsor" -2/2/22 
///////////////////////////////////////////////////////

  
if((SECONDSensor >= 10) & (SECONDSensor <= 50)) //May Use 1 for ESP32
 {digitalWrite(ALARM, HIGH);delay(1000);}
///////////////////////////////////////////////////////

  
if((THIRDSensor >= 10) & (THIRDSensor <= 50))   //May Not be necessary
{digitalWrite(ALARM, HIGH);delay(1000);}
///////////////////////////////////////////////////////
}
/////////////////////////////////////////////////////////////////////////////////
void SonarSensor(int trigPin,int echoPin)
{
digitalWrite(trigPin, LOW);
delayMicroseconds(2);
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);
duration = pulseIn(echoPin, HIGH);
distance = (duration/2) / 29.1;
}
