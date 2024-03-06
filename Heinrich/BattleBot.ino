const int MOTOR_SERVO = 8;

const int MOTOR_A_1 = 9; 
const int MOTOR_A_2 = 3;
const int MOTOR_B_1 = 10;
const int MOTOR_B_2 = 11;
const int MOTOR_B_Afwijking = 50;

const int LIGHT_SENSOR_1 = A3;
const int LIGHT_SENSOR_3 = A1;
const int LIGHT_SENSOR_4 = A4;
const int LIGHT_SENSOR_5 = A5;
const int LIGHT_SENSOR_6 = A0;
const int LIGHT_SENSOR_8 = A2;

const int AFSTAND_ECHO = 4;
const int AFSTAND_TRIGGER = 2;

float duration_us;

bool SERVO_CLOSED = false;

unsigned long timerGripper = 0;

void setup() {
  Serial.begin(9600);
  pinMode(MOTOR_SERVO, OUTPUT);
  pinMode(MOTOR_A_1, OUTPUT);
  pinMode(MOTOR_A_2, OUTPUT);
  pinMode(MOTOR_B_1, OUTPUT);
  pinMode(MOTOR_B_2, OUTPUT);
  pinMode(AFSTAND_ECHO, INPUT);
  pinMode(AFSTAND_TRIGGER, OUTPUT);
}

void loop() {
  followLine();
}
void followLine(){
  if(analogRead(LIGHT_SENSOR_1) > 900){
    motorTurnRight(230);
  } 
  else if(analogRead(LIGHT_SENSOR_8) > 900){
    motorTurnLeft(230);
  } 
  else if(analogRead(LIGHT_SENSOR_4) > analogRead(LIGHT_SENSOR_5)){
    motorLeft(230);
  } 
  else if(analogRead(LIGHT_SENSOR_4) < analogRead(LIGHT_SENSOR_5)){
    motorRight(230);
  } 
  else {
    motorForward(255);
  }
}

void motorForward(int motorSpeed){
  analogWrite(MOTOR_A_1, motorSpeed);
  analogWrite(MOTOR_A_2, 0);
  analogWrite(MOTOR_B_1, 0);
  analogWrite(MOTOR_B_2, motorSpeed - MOTOR_B_Afwijking);
}

void motorTurnRight(int motorSpeed){
  analogWrite(MOTOR_A_1, motorSpeed);
  analogWrite(MOTOR_A_2, 0);
  analogWrite(MOTOR_B_1, 0);
  analogWrite(MOTOR_B_2, 0);
}

void motorTurnLeft(int motorSpeed){
  analogWrite(MOTOR_A_1, 0);
  analogWrite(MOTOR_A_2, 0);
  analogWrite(MOTOR_B_1, 0);
  analogWrite(MOTOR_B_2, motorSpeed - MOTOR_B_Afwijking);
}

void motorRight(int motorSpeed){
  analogWrite(MOTOR_A_1, motorSpeed);
  analogWrite(MOTOR_A_2, 0);
  analogWrite(MOTOR_B_1, 0);
  analogWrite(MOTOR_B_2, 150 - MOTOR_B_Afwijking);
}

void motorLeft(int motorSpeed){
  analogWrite(MOTOR_A_1, 150);
  analogWrite(MOTOR_A_2, 0);
  analogWrite(MOTOR_B_1, 0);
  analogWrite(MOTOR_B_2, motorSpeed - MOTOR_B_Afwijking);
}

void motorBackward(int motorSpeed){
  analogWrite(MOTOR_A_1, 0);
  analogWrite(MOTOR_A_2, motorSpeed);
  analogWrite(MOTOR_B_1, motorSpeed - MOTOR_B_Afwijking);
  analogWrite(MOTOR_B_2, 0);
}

void motorStop(){
  analogWrite(MOTOR_A_1, 0);
  analogWrite(MOTOR_A_2, 0);
  analogWrite(MOTOR_B_1, 0);
  analogWrite(MOTOR_B_2, 0);
}

void motorGripperClose(){
  int del=(7*0)+500;
  for (int pulseCounter=0; pulseCounter<=50; pulseCounter++){
      digitalWrite(MOTOR_SERVO,HIGH);
      delayMicroseconds(del);
      digitalWrite(MOTOR_SERVO,LOW);
      delay(20);
  }
}
void motorGripperOpen(){
  int del=(7*180)+500;
  for (int pulseCounter=0; pulseCounter<=50; pulseCounter++){
      digitalWrite(MOTOR_SERVO,HIGH);
      delayMicroseconds(del);
      digitalWrite(MOTOR_SERVO,LOW);
      delay(20);
  }
}
float getDistanceCM(){
  digitalWrite(AFSTAND_TRIGGER, HIGH);
  delayMicroseconds(10);
  digitalWrite(AFSTAND_TRIGGER, LOW);
  duration_us = pulseIn(AFSTAND_ECHO, HIGH);
  Serial.println(0.017 * duration_us);
  return 0.017 * duration_us;
}
