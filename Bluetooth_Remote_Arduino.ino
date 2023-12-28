#include<SoftwareSerial.h>
#include<string.h>
#include<stdio.h>
#include <Wire.h>

int rightamount = 6;  //Blue   EnA PWM
int rightfront = 7;    //Green  In1
int rightback = 8;   //Yellow In2

int leftback = 9 ;   //Orange In3
int leftfront = 10 ;   //Red    In4
int leftamount = 11;  //Brown  EnB PWM

char BluetoothInput = 0;
int speed = 100;

int IR_SensorL = 4; //Grey    Left  IR
int IR_SensorR = 5; //Purple  Right IR

bool lineFollowingActivated = 0;

void setup();


void loop(){
  //int IR_SensorValueL = digitalRead(IR_SensorL);
  //int IR_SensorValueR = digitalRead(IR_SensorR);

  if (Serial.available() >0){
    BluetoothInput = Serial.read();
    //Serial.print("Y");

    if (BluetoothInput == '2'){ //back
      Serial.print("Back\n");
      leftWheels(speed,0);
      rightWheels(speed,0);
    }

    else if (BluetoothInput == '1'){ //forward
      Serial.print("Front\n");
      leftWheels(speed,1);
      rightWheels(speed,1);
    }

    else if (BluetoothInput == '3'){ //right
      Serial.print("Right\n");
      leftWheels(speed,1);
      rightWheels(speed + 20,0);
    }

    else if (BluetoothInput == '4'){ //left
      Serial.print("left\n");
      leftWheels(speed + 20,0);
      rightWheels(speed,1);
    }

    else if (BluetoothInput == 'T'){   //increase speed
        speed += 20;
        if (speed >= 220){
          speed = 220;
        }
        Serial.print(speed);  
        Serial.print("\n");   
    }

    else if (BluetoothInput == 'X'){  //reduce speed
        speed -= 20;
        if (speed <= 80){
          speed = 80;
        }
      Serial.print(speed);   
      Serial.print("\n");
    }

    else if(BluetoothInput == 'S'){ //counterClockWise
      analogWrite(leftamount, 200);
      analogWrite(rightamount, 100);
      digitalWrite(rightfront, 1);
      digitalWrite(rightback, 0);
      digitalWrite(leftfront, 0);
      digitalWrite(leftback, 1);
      Serial.print("SPINNNNN CW \n");
      delay(3500);
    }

    else if(BluetoothInput == 'C'){ //ClockWise
      analogWrite(leftamount, 200);
      analogWrite(rightamount, 100);
      digitalWrite(rightfront, 0);
      digitalWrite(rightback, 1);
      digitalWrite(leftfront, 1);
      digitalWrite(leftback, 0);
      Serial.print("SPINNNNN CCW \n");
      delay(3500);
    }

    else {                      //else stop
      //Serial.print("NA\n");
      leftWheels(0,0);
      rightWheels(0,1);
    }



    Serial.print(BluetoothInput);

    if (BluetoothInput == 'A'){
      lineFollowingActivated = 1;
    }


    if (lineFollowingActivated == 1 && BluetoothInput == '0'){
      while (BluetoothInput == '0'){
        
      BluetoothInput = Serial.read();
      Serial.print(BluetoothInput);
      Serial.print("\n");
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

    }

    else if (lineFollowingActivated == 1 && BluetoothInput != '0'){
      lineFollowingActivated == 0;
    }
    
    /*
    while(BluetoothInput == 'A'){ //Line Following
      Serial.print(BluetoothInput);
      Serial.print("\n");
      int IR_SensorValueL = digitalRead(IR_SensorL);
      int IR_SensorValueR = digitalRead(IR_SensorR);

      //stop
      if(IR_SensorValueR == 0 && IR_SensorValueL == 0){
        break;
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

      else if(BluetoothInput == 'B'){
        break;
      }

    }
    */
  }
}

void setup(){
  Serial.begin(9600);

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