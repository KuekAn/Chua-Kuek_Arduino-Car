#include <LiquidCrystal.h>
// select the pins used for the LCD keypad
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

int leftamount = A5;
int leftfront = A3;
int leftback = A4;

int rightfront = A1;
int rightback = A2;
int rightamount = 2;

int IR_Sensor1 = 12;
int IR_Sensor2 = 13;

float holes1;
float holes2;

float pi = 3.142 ;
float radius = 3.0 ;

int EncoderA = 3; //right
int prevState1;

void setup()
{
  // set all the motor control pins to outputs
  pinMode(leftamount, OUTPUT);
  pinMode(leftfront, OUTPUT);
  pinMode(leftback, OUTPUT);
  pinMode(rightfront, OUTPUT);
  pinMode(rightback, OUTPUT);
  pinMode(rightamount, OUTPUT);

  pinMode(IR_Sensor1, INPUT);
  pinMode(IR_Sensor2, INPUT) ;

  //pinMode(EncoderA,INPUT);
  //Serial.begin(9600);

  prevState1 = digitalRead(EncoderA); //Saves the initial state
}

void loop()
{ 
  int IR_SensorValue1 = digitalRead(IR_Sensor1);
  int IR_SensorValue2 = digitalRead(IR_Sensor2);

  int CurrentState1 = digitalRead(EncoderA);
  
  if(IR_SensorValue1 == 0 && IR_SensorValue2 == 0){
    analogWrite(leftamount, 0);
    analogWrite(rightamount, 0);
  }

  else if (IR_SensorValue1 == 1 && IR_SensorValue2 == 1){
    analogWrite(leftamount, 130);
    analogWrite(rightamount, 130);
    digitalWrite(leftfront, LOW);
    digitalWrite(leftback, HIGH);
    digitalWrite(rightfront, LOW);
    digitalWrite(leftback, HIGH);
  }

  else if(IR_SensorValue1 == 0 && IR_SensorValue2 == 1){
    analogWrite(leftamount, 130);
    analogWrite(rightamount, 130);
    digitalWrite(rightfront, HIGH);
    digitalWrite(rightback, LOW);
    digitalWrite(leftfront, LOW);
    digitalWrite(leftback, HIGH);
  }
  
  else if(IR_SensorValue1 == 1 && IR_SensorValue2 == 0){
    analogWrite(leftamount, 130);
    analogWrite(rightamount, 130);
    digitalWrite(rightfront, LOW);
    digitalWrite(rightback, HIGH);
    digitalWrite(leftfront, HIGH);
    digitalWrite(leftback, LOW);
  }


  //Compares current digital signal with previous Signal saved
  if (CurrentState1 != prevState1 && CurrentState1 == 1){
    holes1++ ;
    //delay(200);
  }
  else{
    holes1=holes1 ;
    }

  lcd.setCursor(0, 0);
  float cycles1 = (holes1)/20 ;

  float distance1 = (cycles1)*(2*pi*radius);
  lcd.print("dis(cm) :");
  lcd.print(distance1);


  //updates the previous state
  prevState1 = CurrentState1 ;
}