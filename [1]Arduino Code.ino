#include <Servo.h>   //servo library
Servo servo;  
int servoPin = 7;
#define trigPin1 A1
#define echoPin1 A0

int ALARM = 8; //Built in Piezo (Arduino Maker)


#define LEDbluemove 13




long duration, distance, FIRSTSensor,SECONDSensor,THIRDSensor;
void setup()
{

Serial.begin (9600);
 
servo.attach(servoPin);   

pinMode(trigPin1, OUTPUT);
pinMode(echoPin1, INPUT);

pinMode(ALARM, OUTPUT);
digitalWrite(ALARM, LOW);

servo.write(0);         //close cap on power on
delay(100);
servo.detach();
  
  
  
 
  
  
  
pinMode(LEDbluemove,OUTPUT);  
   
  
  
}

void loop() 
{
/////////////////////////////////////////////////////// 
SonarSensor(trigPin1, echoPin1);
FIRSTSensor = distance;


/////////////////////////////////////////////////////////////
digitalWrite(ALARM, LOW);
//////////////////////////////////////////////////////////////////////////////////////
Serial.print("S1:");Serial.println(FIRSTSensor); delayMicroseconds(10000);

///////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////
if((FIRSTSensor >= 10) & (FIRSTSensor <= 50)) 
{
 // digitalWrite(ALARM, HIGH);
  
  digitalWrite( LEDbluemove, HIGH);
 delay(1000);
Serial.println("Blue ON");
  tone(ALARM, 950);  //High pitch 950; Low pitch 450
  delay(1000);
  noTone(ALARM);  //Alarm Buzz for 1 second
  delay(1000); 
  servo.attach(servoPin);
  delay(1);
 servo.write(0);  
 delay(8000);     //Open for 8 seconds                  ////LID SENSOR/////!
 servo.write(150);    
 delay(1000);
 servo.detach();
 
}
else 
{
digitalWrite( LEDbluemove, LOW);
delay(1000);
Serial.println("Blue ON");
}  
  
  


  


}

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
