
/*
 Stepper Motor Control - one revolution

 This program drives a unipolar or bipolar stepper motor.
 The motor is attached to digital pins 8 - 11 of the Arduino.

 The motor should revolve one revolution in one direction, then
 one revolution in the other direction.


 Created 11 Mar. 2007
 Modified 30 Nov. 2009
 by Tom Igoe

 */

#include <Stepper.h>

const int stepsPerRevolution = 64;  // change this to fit the number of steps per revolution
//const int stepsPerRevolution = 100;  // change this to fit the number of steps per revolution
//const int stepsPerRevolution = 4096;  // change this to fit the number of steps per revolution
// for your motor
//int count = 10;
int count = 32;
long end1 = 600;
int items[] = {"rice", "tea", "milk"};
int times[] = {25, 10, 15};

// initialize the stepper library on pins 8 through 11:
Stepper myStepper(stepsPerRevolution, 8, 9, 10, 11);

void setup() {
  // set the speed at 60 rpm:
  myStepper.setSpeed(200);
//  myStepper.setSpeed(60);
//  myStepper.setSpeed(200);
  // initialize the serial port:
  Serial.begin(9600);
}

void loop() {
  String item = "Rice";
  int index;
  for(int i = 0; i < 3; i++){
    if(items[i] = item)
    {
      index = i;
    }
  }
  int time1 = times[index];
  // step one revolution  in one direction:
//  count += 1;
//  long curr = millis();
//  Serial.println(curr-end1);
//if(count == 0)
//{
//  myStepper.step(stepsPerRevolution);
//  count++; 
//}
//
//delay(500);
//Serial.println("Hi");
  while(count > 0)
  {
    Serial.println(count);
//    Serial.println("clockwise");
//Serial.println(millis());
    myStepper.step(stepsPerRevolution);
    count--;
  } 
//    Serial.println(millis());
//  delay(500);
//
//  // step one revolution in the other direction:
//  Serial.println("counterclockwise");
//  myStepper.step(-stepsPerRevolution);
//  delay(500);
}
