/*
 * This ESP32 code is created by esp32io.com
 *
 * This ESP32 code is released in the public domain
 *
 * For more detail (instruction and wiring diagram), visit https://esp32io.com/tutorials/esp32-ultrasonic-sensor-servo-motor
 */

#define BLYNK_PRINT Serial
#include <ESP32Servo.h>
#include<DHT.h>
#include<WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

#define TRIG_PIN  23  // ESP32 pin GIOP23 connected to Ultrasonic Sensor's TRIG pin
#define ECHO_PIN  22  // ESP32 pin GIOP22 connected to Ultrasonic Sensor's ECHO pin
#define SERVO_PIN 26  // ESP32 pin GIOP26 connected to Servo Motor's pin
#define DISTANCE_THRESHOLD  50 // centimeters
const int ledBlue = 27; // Lid led

Servo servo; // create servo object to control a servo

// The below are variables, which can be changed
float duration_us, distance_cm;

void setup() {

 pinMode(ledBlue, OUTPUT);   // Humdity's LED
  
  Serial.begin (9600);       // initialize serial port
  pinMode(TRIG_PIN, OUTPUT); // set ESP32 pin to output mode
  pinMode(ECHO_PIN, INPUT);  // set ESP32 pin to input mode
  servo.attach(SERVO_PIN);   // attaches the servo on pin 9 to the servo object
  servo.write(0);
}

void loop() {
  // generate 10-microsecond pulse to TRIG pin
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // measure duration of pulse from ECHO pin
  duration_us = pulseIn(ECHO_PIN, HIGH);
  // calculate the distance
  distance_cm = 0.017 * duration_us;

  if (distance_cm < DISTANCE_THRESHOLD){

   servo.write(180); // rotate servo motor to 90 degree
   digitalWrite(ledBlue,HIGH);
   delay(8000);
  }
  
  else{
    servo.write(0);  // rotate servo motor to 0 degree
    digitalWrite(ledBlue,LOW);
    delay(1000);
  }
  // print the value to Serial Monitor
  Serial.print("distance: ");
  Serial.print(distance_cm);
  Serial.println(" cm");

  delay(500);
}
