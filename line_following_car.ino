#include <Wire.h>

int IR_SensorL = 4; //Grey    Left  IR
int IR_SensorR = 5; //Purple  Right IR


int rightamount = 6;  //Blue   EnA PWM
int rightfront = 7;    //Green  In1
int rightback = 8;   //Yellow In2

int leftback = 9 ;   //Orange In3
int leftfront = 10 ;   //Red    In4
int leftamount = 11;  //Brown  EnB PWM


void setup()
{
  //Set all IR Sensor Pins to Input
  pinMode(IR_SensorL, INPUT);
  pinMode(IR_SensorR, INPUT) ;

  //Set all the motor control pins to outputs
  pinMode(rightamount,OUTPUT);
  pinMode(rightback,  OUTPUT);
  pinMode(rightfront, OUTPUT);
  pinMode(leftfront,  OUTPUT);
  pinMode(leftback,   OUTPUT);
  pinMode(leftamount, OUTPUT);
}


void loop(){ 
  //Line Following Code
  lineFollowing();
}


//wheels shortcut function to use in lineFollowing() function later
void leftWheels(int amount, bool value){
  analogWrite(leftamount, amount);
  digitalWrite(leftfront, value);
  digitalWrite(leftback,  value - 1);
}
void rightWheels(int amount, bool value){
  analogWrite(rightamount, amount);
  digitalWrite(rightfront, value);
  digitalWrite(rightback, value - 1);
}


void lineFollowing(){
  //Set to always read IR Sensors
  int IR_SensorValueL = digitalRead(IR_SensorL);
  int IR_SensorValueR = digitalRead(IR_SensorR);

  //stop
  if(IR_SensorValueR == 0 && IR_SensorValueL == 0){
    leftWheels(0, 1);
    rightWheels(0, 1);
  }

  //forward
  else if (IR_SensorValueR == 1 && IR_SensorValueL == 1){
    leftWheels(80, 1);
    rightWheels(80, 1);
  }

  //left Turn
  else if(IR_SensorValueR == 0 && IR_SensorValueL == 1){
    leftWheels(130, 1);
    rightWheels(130, 0);
  }

  //right Turn
  else if(IR_SensorValueR == 1 && IR_SensorValueL == 0){
    leftWheels(130, 0);
    rightWheels(130, 1);
  }
}