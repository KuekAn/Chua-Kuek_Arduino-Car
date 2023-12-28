#include <LiquidCrystal.h>
#include <Wire.h>

// select the pins used for the LCD keypad
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

int leftamount = 11; //A5 initially - A3 = Brown = EnB
int leftfront = 1 ; //A3 initially -A4 = Orange
int leftback = A3 ;//A4 initially -A5 = Red

int rightfront = A1; //Yellow
int rightback = A2; // Green
int rightamount = 2; //EnA = Blue

int IR_Sensor1 = 12; //Purple
int IR_Sensor2 = 0; //Grey

float holes1;
float holes2;

float pi = 3.142 ;
float radius = 3.0 ;

int EncoderA = 3; //right
int prevState1;

float RateRoll, RatePitch, RateYaw;
float AccX, AccY, AccZ;
float AngleRoll, AnglePitch;
float LoopTimer;

float speed = 130 ;
bool isClimbed = 0;

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
  lcd.begin(16,2);
  prevState1 = digitalRead(EncoderA); //Saves the initial state

  Serial.begin(57600);
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  Wire.setClock(400000);
  Wire.begin();
  delay(250);
  Wire.beginTransmission(0x68); 
  Wire.write(0x6B);
  Wire.write(0x00);
  Wire.endTransmission();
}

void loop()
{ 
  gyro_signals();

  Serial.print("Acceleration Y [g]= ");
  Serial.print(AccX);
  Serial.print("  Acceleration X = ");
  Serial.print(AccY);
  float xAxis = AccY * 360 ;
  Serial.print("  Degree = ");
  float degree = AccX * 90 ;
  Serial.print(degree);
  Serial.print("\n");
  //delay(50);
  lcd.setCursor(0,0);
  lcd.print(xAxis);
  lcd.setCursor(0,2);
  lcd.print("Degree : ");
  lcd.print(degree);

  int IR_SensorValue1 = digitalRead(IR_Sensor1);
  int IR_SensorValue2 = digitalRead(IR_Sensor2);

  int CurrentState1 = digitalRead(EncoderA);

  

  if (degree >= 34 && isClimbed == 0){
    isClimbed = 1;
    analogWrite(rightamount, 180);
    analogWrite(leftamount, 180);
    
    digitalWrite(leftfront, 0);
    digitalWrite(leftback, 1);
    digitalWrite(rightfront, 0);
    digitalWrite(leftback, 1);
    delay(5000);
    analogWrite(leftamount,0);
    analogWrite(rightamount,0);
    delay(4000);

    for(int i = 0;i<=15;i++){
      analogWrite(rightamount, 180);
      analogWrite(leftamount, 180);
      
      digitalWrite(rightfront, LOW);
      digitalWrite(rightback, HIGH);
      digitalWrite(leftfront, HIGH);
      digitalWrite(leftback, LOW);
    }
    delay(2000);
  }

  

  /*if (isClimbed = 1 && degree >= -10 && degree <= 10){
    delay(1000);
    analogWrite(leftamount, speed);
    analogWrite(rightamount, speed);
    digitalWrite(leftfront, LOW);
    digitalWrite(leftback, HIGH);
    digitalWrite(rightfront, LOW);
    digitalWrite(leftback, HIGH);
    delay(1000);
    isClimbed = 0;
  }*/

  if(IR_SensorValue1 == 0 && IR_SensorValue2 == 0){
    analogWrite(leftamount, 0);
    analogWrite(rightamount, 0);
  }

  else if (IR_SensorValue1 == 1 && IR_SensorValue2 == 1){
    analogWrite(leftamount, speed);
    analogWrite(rightamount, speed);
    digitalWrite(leftfront, LOW);
    digitalWrite(leftback, HIGH);
    digitalWrite(rightfront, LOW);
    digitalWrite(leftback, HIGH);
  }

  else if(IR_SensorValue1 == 0 && IR_SensorValue2 == 1){
    analogWrite(leftamount, speed);
    analogWrite(rightamount, speed);
    digitalWrite(rightfront, HIGH);
    digitalWrite(rightback, LOW);
    digitalWrite(leftfront, LOW);
    digitalWrite(leftback, HIGH);
  }
  
  else if(IR_SensorValue1 == 1 && IR_SensorValue2 == 0){
    analogWrite(leftamount, speed);
    analogWrite(rightamount, speed);
    digitalWrite(rightfront, LOW);
    digitalWrite(rightback, HIGH);
    digitalWrite(leftfront, HIGH);
    digitalWrite(leftback, LOW);
  }

  /*
  //Compares current digital signal with previous Signal saved
  if (CurrentState1 != prevState1 && CurrentState1 == 1){
    holes1++ ;
    //delay(200);
  }
  else{
    holes1=holes1 ;
    }

  lcd.setCursor(0, 1);
  float cycles1 = (holes1)/20 ;

  float distance1 = (cycles1)*(2*pi*radius);
  /*lcd.print("dis(cm) :");
  lcd.print(distance1);*/


  //updates the previous state
  /*
  prevState1 = CurrentState1 ;
  */
 /* gyro_signals();

  Serial.print("Acceleration Y [g]= ");
  Serial.print(AccX);
  Serial.print("  Degree = ");
  float degree = AccX * 90 ;
  Serial.print(degree);
  Serial.print("\n");
  //delay(50);
  lcd.setCursor(0,2);
  lcd.print("Degree : ");
  lcd.print(degree);*/

}

void gyro_signals(void) {
  Wire.beginTransmission(0x68);
  Wire.write(0x1A);
  Wire.write(0x05);
  Wire.endTransmission();
  Wire.beginTransmission(0x68);
  Wire.write(0x1C);
  Wire.write(0x10);
  Wire.endTransmission();
  Wire.beginTransmission(0x68);
  Wire.write(0x3B);
  Wire.endTransmission(); 
  Wire.requestFrom(0x68,6);
  int16_t AccXLSB = Wire.read() << 8 | Wire.read();
  int16_t AccYLSB = Wire.read() << 8 | Wire.read();
  int16_t AccZLSB = Wire.read() << 8 | Wire.read();
  Wire.beginTransmission(0x68);
  Wire.write(0x1B); 
  Wire.write(0x8);
  Wire.endTransmission();                                                   
  Wire.beginTransmission(0x68);
  Wire.write(0x43);
  Wire.endTransmission();
  Wire.requestFrom(0x68,6);
  int16_t GyroX=Wire.read()<<8 | Wire.read();
  int16_t GyroY=Wire.read()<<8 | Wire.read();
  int16_t GyroZ=Wire.read()<<8 | Wire.read();
  RateRoll=(float)GyroX/65.5;
  RatePitch=(float)GyroY/65.5;
  RateYaw=(float)GyroZ/65.5;
  AccX=(float)AccXLSB/4096;
  AccY=(float)AccYLSB/4096;
  AccZ=(float)AccZLSB/4096;
  AngleRoll=atan(AccY/sqrt(AccX*AccX+AccZ*AccZ))*1/(3.142/180);
  AnglePitch=-atan(AccX/sqrt(AccY*AccY+AccZ*AccZ))*1/(3.142/180);
}