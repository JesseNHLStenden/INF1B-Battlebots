#include <Adafruit_NeoPixel.h>
#include <Servo.h>

#define NUM_PIXELS 4 // Number of NeoPixels in the strip
#define PIXEL_PIN 7  // Pin connected to the NeoPixels

// Create NeoPixel object
Adafruit_NeoPixel pixels(NUM_PIXELS, PIXEL_PIN, NEO_GRB + NEO_KHZ800);

/*
NEOPIXEL LIGHTS CONFIGURATION
Linksachter: 0
Rechtsachter: 1
Rechtsvoor: 2
Linksvoor: 3
*/

int colorValues[] = {0, 0, 0, 0, 0, 0};

const int MOTOR_SERVO = 9;

const int MOTOR_A_1 = 5;
const int MOTOR_A_2 = 3;
const int MOTOR_B_1 = 6;
const int MOTOR_B_2 = 11;
const int MOTOR_B_Afwijking = 50;

const int LIGHT_SENSOR_1 = A5;
const int LIGHT_SENSOR_2 = A4;
const int LIGHT_SENSOR_3 = A6;
const int LIGHT_SENSOR_4 = A3;
const int LIGHT_SENSOR_5 = A2;
const int LIGHT_SENSOR_6 = A1;
const int LIGHT_SENSOR_7 = A0;
const int LIGHT_SENSOR_8 = A7;

const int MOTOR_SENSOR_1 = 12;
const int MOTOR_SENSOR_2 = 10;

const int AFSTAND_ECHO = 4;
const int AFSTAND_TRIGGER = 2;

int COLOR_BLACK = 900;

float duration_us;

bool START_READY = false;

Servo gripperServo;

void setup()
{
  Serial.begin(9600);
  pinMode(MOTOR_SERVO, OUTPUT);
  pinMode(MOTOR_A_1, OUTPUT);
  pinMode(MOTOR_A_2, OUTPUT);
  pinMode(MOTOR_B_1, OUTPUT);
  pinMode(MOTOR_B_2, OUTPUT);
  pinMode(AFSTAND_ECHO, INPUT);
  pinMode(AFSTAND_TRIGGER, OUTPUT);
  gripperServo.attach(MOTOR_SERVO);
  pixels.begin();

}

void loop()
{
  if (START_READY)
  {
    standardLight();
    followLine();
  }
  else
  {
    getDistanceCM() ;

    do
    {
      delay(10);
    } while (getDistanceCM() > 20);

    for (int i = 0; i < 4; i++)
    {
      setLightStartUpLight(i);
      delay(1250);
    }

    motorForward(255);

    for (int i = 0; i < 6; i += 2)
    {
      while (analogRead(LIGHT_SENSOR_1) > COLOR_BLACK && analogRead(LIGHT_SENSOR_7) > COLOR_BLACK)
      {
        delay(1);
      }
      motorStop();
      delay(500);
      colorValues[i] = analogRead(LIGHT_SENSOR_4);
      Serial.println(colorValues[i]);
      motorForward(210);
      while (analogRead(LIGHT_SENSOR_1) < COLOR_BLACK && analogRead(LIGHT_SENSOR_7) < COLOR_BLACK)
      {
        delay(1);
      }
      motorStop();
      delay(500);
      colorValues[i + 1] = analogRead(LIGHT_SENSOR_4);
      Serial.println(colorValues[i + 1]);
      motorRight(200);
      delay(50);
      motorForward(210);
    }

    motorForward(200);

    delay(200);

    COLOR_BLACK = average(colorValues, 6) + 250;

    while (analogRead(LIGHT_SENSOR_1) > COLOR_BLACK && analogRead(LIGHT_SENSOR_7) > COLOR_BLACK)
    {
      delay(1);
    }
    motorForwardWithPulses(230, 20);
    motorStop();
    motorGripperClose();
    motorTurnLeft(230);
    motorForwardWithPulses(230, 15);
    START_READY = true;
  }
}
void followLine()
{
  if (analogRead(LIGHT_SENSOR_1) > COLOR_BLACK || analogRead(LIGHT_SENSOR_2) > COLOR_BLACK)
  {
    motorForwardWithPulses(200, 9);
    motorTurnRight(200);
    while (true)
    {
      motorRight(200);
      if(analogRead(LIGHT_SENSOR_4) > COLOR_BLACK){
        break;
      }
    }
    
    
  }
  // 
  else if (analogRead(LIGHT_SENSOR_6) > COLOR_BLACK || analogRead(LIGHT_SENSOR_5) > COLOR_BLACK || analogRead(LIGHT_SENSOR_4) > COLOR_BLACK || analogRead(LIGHT_SENSOR_7) > COLOR_BLACK)
  {
    motorForward(210);
  }
  else
  {
    checkBothSides();
  }
}
void checkBothSides()
{
  motorStop();

  bool colorDetected = false;

  if (analogRead(LIGHT_SENSOR_8) > COLOR_BLACK){
    do{
      motorLeft(200);
      if(analogRead(LIGHT_SENSOR_7) > COLOR_BLACK){
        break;
      }
    } while (true);
  } else if (analogRead(LIGHT_SENSOR_3) > COLOR_BLACK ||  analogRead(LIGHT_SENSOR_2) > COLOR_BLACK || analogRead(LIGHT_SENSOR_1) > COLOR_BLACK) {
    do{
      motorRight(200);
      if(analogRead(LIGHT_SENSOR_4) > COLOR_BLACK){
        break;
      }
    } while (true);
  } else {
    do{
      motorLeft(200);
      if(analogRead(LIGHT_SENSOR_7) > COLOR_BLACK){
        break;
      }
    } while (true);
  }
  motorStop();

  // // check Right
  // for (int i = 0; i < 400; i++)
  // {
  //   motorRight(200);
  //   if (analogRead(LIGHT_SENSOR_5) > COLOR_BLACK)
  //   {
  //     colorDetected = true;
  //     break;
  //   }
  //   delay(1);
  // }
  // motorStop();
  // delay(200);

  // // check Left
  // if (!colorDetected)
  // {
  //   for (int i = 0; i < 900; i++)
  //   {
  //     motorLeft(200);
  //     if (analogRead(LIGHT_SENSOR_5) > COLOR_BLACK)
  //     {
  //       colorDetected = true;
  //       break;
  //     }
  //     delay(1);
  //   }
  // }
  // if (!colorDetected)
  // {
  //   while (colorDetected)
  //   {
  //     motorLeft(200);
  //     if (analogRead(LIGHT_SENSOR_6) > COLOR_BLACK || analogRead(LIGHT_SENSOR_5) > COLOR_BLACK)
  //     {
  //       colorDetected = true;
  //       break;
  //     }
  //     delay(1);
  //   }
  // }
  // motorStop();
}

void motorForward(int motorSpeed)
{
  standardLight();
  analogWrite(MOTOR_A_1, motorSpeed);
  analogWrite(MOTOR_A_2, 0);
  analogWrite(MOTOR_B_1, 0);
  analogWrite(MOTOR_B_2, motorSpeed - MOTOR_B_Afwijking);
}

void motorForwardWithPulses(int motorSpeed, double pulses)
{
  standardLight();
  int pulsesToDo = pulses * 2;
  int motor1PulsesDone = 0;
  int motor2PulsesDone = 0;
  int previous1Pulse = 2;
  int previous2Pulse = 2;
  bool motor1Ready = false;
  bool motor2Ready = false;

  while (!motor1Ready || !motor2Ready)
  {

    if (!motor1Ready)
    {
      analogWrite(MOTOR_A_1, motorSpeed);
      analogWrite(MOTOR_A_2, 0);
    }
    else
    {
      analogWrite(MOTOR_A_1, 0);
      analogWrite(MOTOR_A_2, 0);
    }

    if (!motor2Ready)
    {
      analogWrite(MOTOR_B_1, 0);
      analogWrite(MOTOR_B_2, motorSpeed - MOTOR_B_Afwijking);
    }
    else
    {
      analogWrite(MOTOR_B_1, 0);
      analogWrite(MOTOR_B_2, 0);
    }

    int pulseMotor1Readed = digitalRead(MOTOR_SENSOR_1);
    int pulseMotor2Readed = digitalRead(MOTOR_SENSOR_2);

    if (previous1Pulse != pulseMotor1Readed)
    {
      motor1PulsesDone++;
      previous1Pulse = pulseMotor1Readed;
    }

    if (previous2Pulse != pulseMotor2Readed)
    {
      motor2PulsesDone++;
      previous2Pulse = pulseMotor2Readed;
    }

    motor1Ready = (motor1PulsesDone >= pulsesToDo);
    motor2Ready = (motor2PulsesDone >= pulsesToDo);
  }
  motorStop();
}
void motorTurnRight(int motorSpeed)
{
  rightLights();
  int pulsesToDo = 9 * 2;
  int motor1PulsesDone = 0;
  int motor2PulsesDone = 0;
  int previous1Pulse = 2;
  int previous2Pulse = 2;
  bool motor1Ready = false;
  bool motor2Ready = false;

  while (!motor1Ready || !motor2Ready)
  {

    if (!motor1Ready)
    {
      analogWrite(MOTOR_A_1, motorSpeed);
      analogWrite(MOTOR_A_2, 0);
    }
    else
    {
      analogWrite(MOTOR_A_1, 0);
      analogWrite(MOTOR_A_2, 0);
    }

    if (!motor2Ready)
    {
      analogWrite(MOTOR_B_1, motorSpeed);
      analogWrite(MOTOR_B_2, 0);
    }
    else
    {
      analogWrite(MOTOR_B_1, 0);
      analogWrite(MOTOR_B_2, 0);
    }

    int pulseMotor1Readed = digitalRead(MOTOR_SENSOR_1);
    int pulseMotor2Readed = digitalRead(MOTOR_SENSOR_2);

    if (previous1Pulse != pulseMotor1Readed)
    {
      motor1PulsesDone++;
      previous1Pulse = pulseMotor1Readed;
    }

    if (previous2Pulse != pulseMotor2Readed)
    {
      motor2PulsesDone++;
      previous2Pulse = pulseMotor2Readed;
    }

    motor1Ready = (motor1PulsesDone >= pulsesToDo);
    motor2Ready = (motor2PulsesDone >= pulsesToDo);
  }
  motorStop();
}
void motorTurnLeft(int motorSpeed)
{
  leftLights();
  int pulsesToDo = 8 * 2;
  int motor1PulsesDone = 0;
  int motor2PulsesDone = 0;
  int previous1Pulse = 2;
  int previous2Pulse = 2;
  bool motor1Ready = false;
  bool motor2Ready = false;

  while (!motor1Ready || !motor2Ready)
  {

    if (!motor1Ready)
    {
      analogWrite(MOTOR_A_1, 0);
      analogWrite(MOTOR_A_2, motorSpeed);
    }
    else
    {
      analogWrite(MOTOR_A_1, 0);
      analogWrite(MOTOR_A_2, 0);
    }

    if (!motor2Ready)
    {
      analogWrite(MOTOR_B_1, 0);
      analogWrite(MOTOR_B_2, motorSpeed);
    }
    else
    {
      analogWrite(MOTOR_B_1, 0);
      analogWrite(MOTOR_B_2, 0);
    }

    int pulseMotor1Readed = digitalRead(MOTOR_SENSOR_1);
    int pulseMotor2Readed = digitalRead(MOTOR_SENSOR_2);

    if (previous1Pulse != pulseMotor1Readed)
    {
      motor1PulsesDone++;
      previous1Pulse = pulseMotor1Readed;
    }

    if (previous2Pulse != pulseMotor2Readed)
    {
      motor2PulsesDone++;
      previous2Pulse = pulseMotor2Readed;
    }

    motor1Ready = (motor1PulsesDone >= pulsesToDo);
    motor2Ready = (motor2PulsesDone >= pulsesToDo);
  }
  motorStop();
}

void motorRight(int motorSpeed)
{
  rightLights();
  analogWrite(MOTOR_A_1, motorSpeed);
  analogWrite(MOTOR_A_2, 0);
  analogWrite(MOTOR_B_1, motorSpeed - MOTOR_B_Afwijking);
  analogWrite(MOTOR_B_2, 0);
}
void motorLeft(int motorSpeed)
{
  leftLights();
  analogWrite(MOTOR_A_1, 0);
  analogWrite(MOTOR_A_2, motorSpeed);
  analogWrite(MOTOR_B_1, 0);
  analogWrite(MOTOR_B_2, motorSpeed - MOTOR_B_Afwijking);
}
void motorBackward(int motorSpeed)
{

  analogWrite(MOTOR_A_1, 0);
  analogWrite(MOTOR_A_2, motorSpeed);
  analogWrite(MOTOR_B_1, motorSpeed - MOTOR_B_Afwijking);
  analogWrite(MOTOR_B_2, 0);
}
void motorStop()
{
  brakeLights();
  analogWrite(MOTOR_A_1, 0);
  analogWrite(MOTOR_A_2, 0);
  analogWrite(MOTOR_B_1, 0);
  analogWrite(MOTOR_B_2, 0);
}

void motorGripperClose()
{
  gripperServo.write(30);
}
void motorGripperOpen()
{
  gripperServo.write(90);
}
float getDistanceCM()
{
  digitalWrite(AFSTAND_TRIGGER, HIGH);
  delayMicroseconds(10);
  digitalWrite(AFSTAND_TRIGGER, LOW);
  duration_us = pulseIn(AFSTAND_ECHO, HIGH);
  Serial.println(0.017 * duration_us);
  return 0.017 * duration_us;
}
void brakeLights()
{
  pixels.setPixelColor(0, pixels.Color(0, 255, 0));
  pixels.setPixelColor(1, pixels.Color(0, 255, 0));
  pixels.setPixelColor(2, pixels.Color(50, 50, 50));
  pixels.setPixelColor(3, pixels.Color(50, 50, 50));
  pixels.show();
}
void frontLights()
{
  pixels.setPixelColor(0, pixels.Color(0, 50, 0));
  pixels.setPixelColor(1, pixels.Color(0, 50, 0));
  pixels.setPixelColor(2, pixels.Color(255, 255, 255));
  pixels.setPixelColor(3, pixels.Color(255, 255, 255));
  pixels.show();
}
void leftLights()
{
  pixels.setPixelColor(0, pixels.Color(64, 255, 0));
  pixels.setPixelColor(1, pixels.Color(0, 50, 0));
  pixels.setPixelColor(2, pixels.Color(50, 50, 50));
  pixels.setPixelColor(3, pixels.Color(64, 255, 0));
  pixels.show();
}
void rightLights()
{
  pixels.setPixelColor(0, pixels.Color(0, 50, 0));
  pixels.setPixelColor(1, pixels.Color(64, 255, 0));
  pixels.setPixelColor(2, pixels.Color(64, 255, 0));
  pixels.setPixelColor(3, pixels.Color(50, 50, 50));
  pixels.show();
}
void standardLight()
{
  pixels.setPixelColor(0, pixels.Color(0, 50, 0));
  pixels.setPixelColor(1, pixels.Color(0, 50, 0));
  pixels.setPixelColor(2, pixels.Color(50, 50, 50));
  pixels.setPixelColor(3, pixels.Color(50, 50, 50));
  pixels.show();
}
void setLightStartUpLight(int lightNR)
{
  pixels.setPixelColor(lightNR, pixels.Color(0, 0, 255));
  pixels.show();
}
int average(int numbers[], int size)
{
  double sum = 0;
  for (int x = 0; x < size; x++)
  {
    sum += numbers[x];
  }
  return (int)sum / (double)size;
}