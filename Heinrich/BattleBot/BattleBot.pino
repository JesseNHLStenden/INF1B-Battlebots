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

const int MOTOR_A_1 = 3;
const int MOTOR_A_2 = 5;
const int MOTOR_B_1 = 6;
const int MOTOR_B_2 = 11;
const int MOTOR_B_Afwijking = 45;

const int LIGHT_SENSOR[8] = {A5, A4, A6, A3, A2, A1, A0, A7};

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

  for (int i = 0; i < 8; i++)
  {
    pinMode(LIGHT_SENSOR[i], INPUT);
  }

  gripperServo.attach(MOTOR_SERVO);
  pixels.begin();

  lightsOff();
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
    getDistanceCM();

    for (int i = 0; i < 3; i++)
    {
      do
      {
        delay(100);
      } while (getDistanceCM() > 24);
    }

    for (int i = 0; i < 4; i++)
    {
      setLightStartUpLight(i);
      delay(1250);
    }

    motorForward(255);

    for (int i = 0; i < 6; i += 2)
    {
      while (analogRead(LIGHT_SENSOR[0]) > COLOR_BLACK && analogRead(LIGHT_SENSOR[6]) > COLOR_BLACK)
      {
        delay(1);
      }

      colorValues[i] = analogRead(LIGHT_SENSOR[3]);
      Serial.println(colorValues[i]);

      delay(100);

      motorForward(190);
      while (analogRead(LIGHT_SENSOR[0]) < COLOR_BLACK && analogRead(LIGHT_SENSOR[6]) < COLOR_BLACK)
      {
        delay(1);
      }

      colorValues[i + 1] = analogRead(LIGHT_SENSOR[3]);
      Serial.println(colorValues[i + 1]);

      delay(100);

      motorForward(190);
    }

    motorForward(200);

    delay(200);

    COLOR_BLACK = average(colorValues, 6) + 250;

    while (analogRead(LIGHT_SENSOR[4]) > COLOR_BLACK && analogRead(LIGHT_SENSOR[5]) > COLOR_BLACK)
    {
      delay(1);
    }
    while (analogRead(LIGHT_SENSOR[4]) < COLOR_BLACK)
    {
      delay(1);
    }
    motorGripperClose();
    motorLeft(200);
    delay(200);

    do
    {
      motorLeft(200);
      if (analogRead(LIGHT_SENSOR[6]) > COLOR_BLACK)
      {
        break;
      }
    } while (true);
    START_READY = true;
  }
}
void followLine()
{
  // if (analogRead(LIGHT_SENSOR[2]) > COLOR_BLACK && analogRead(LIGHT_SENSOR[3]) > COLOR_BLACK && analogRead(LIGHT_SENSOR[6]) > COLOR_BLACK && analogRead(LIGHT_SENSOR[7]) > COLOR_BLACK)
  // {
  //   delay(200);
  //   if (analogRead(LIGHT_SENSOR[2]) > COLOR_BLACK && analogRead(LIGHT_SENSOR[3]) > COLOR_BLACK && analogRead(LIGHT_SENSOR[6]) > COLOR_BLACK && analogRead(LIGHT_SENSOR[7]) > COLOR_BLACK)
  //   {
  //     motorStop();
  //     motorGripperOpen();
  //     motorBackward(255);
  //     delay(2000);
  //     motorStop();
  //     exit(0);
  //   }
  // }
  // else
  if (analogRead(LIGHT_SENSOR[0]) > COLOR_BLACK || analogRead(LIGHT_SENSOR[1]) > COLOR_BLACK)
  {
    motorForwardWithPulses(200, 3);
    if (analogRead(LIGHT_SENSOR[0]) > COLOR_BLACK && analogRead(LIGHT_SENSOR[1]) > COLOR_BLACK && analogRead(LIGHT_SENSOR[2]) > COLOR_BLACK){
      motorStop();
      motorGripperOpen();
      motorBackward(255);
      delay(2000);
      motorStop();
      while (true)
      {
        int randomColor = random(0, 255);
        for (int i = 0; i <= 255; i++){
          delay(10);
          for (int j = 0; j < 4; j++)
          {
            pixels.setPixelColor(0, pixels.Color(i, i, randomColor));
            pixels.setPixelColor(1, pixels.Color(i, i, randomColor));
            pixels.setPixelColor(2, pixels.Color(i, i, randomColor));
            pixels.setPixelColor(3, pixels.Color(i, i, randomColor));
            pixels.show();
          }
        }
        for (int i = 0; i <= 255; i++){
          delay(10);
          for (int j = 0; j < 4; j++)
          {
            pixels.setPixelColor(0, pixels.Color(randomColor, i, i));
            pixels.setPixelColor(1, pixels.Color(randomColor, i, i));
            pixels.setPixelColor(2, pixels.Color(randomColor, i, i));
            pixels.setPixelColor(3, pixels.Color(randomColor, i, i));
            pixels.show();
          }
        }
        for (int i = 0; i <= 255; i++){
          delay(10);
          for (int j = 0; j < 4; j++)
          {
            pixels.setPixelColor(0, pixels.Color(i, randomColor, i));
            pixels.setPixelColor(1, pixels.Color(i, randomColor, i));
            pixels.setPixelColor(2, pixels.Color(i, randomColor, i));
            pixels.setPixelColor(3, pixels.Color(i, randomColor, i));
            pixels.show();
          }
        }
      }
    } else {
      motorForwardWithPulses(200, 6);
      motorTurnRight(200);
      while (true)
      {
        motorRight(200);
        if (analogRead(LIGHT_SENSOR[3]) > COLOR_BLACK)
        {
          break;
        }
      }
    }
  }
  else if (analogRead(LIGHT_SENSOR[5]) > COLOR_BLACK || analogRead(LIGHT_SENSOR[4]) > COLOR_BLACK || analogRead(LIGHT_SENSOR[3]) > COLOR_BLACK || analogRead(LIGHT_SENSOR[6]) > COLOR_BLACK)
  {
    motorForward(220);
  }
  else if (analogRead(LIGHT_SENSOR[0]) < COLOR_BLACK && analogRead(LIGHT_SENSOR[1]) < COLOR_BLACK && analogRead(LIGHT_SENSOR[2]) < COLOR_BLACK && analogRead(LIGHT_SENSOR[3]) < COLOR_BLACK && analogRead(LIGHT_SENSOR[4]) < COLOR_BLACK && analogRead(LIGHT_SENSOR[5]) < COLOR_BLACK && analogRead(LIGHT_SENSOR[4]) < COLOR_BLACK && analogRead(LIGHT_SENSOR[3]) < COLOR_BLACK && analogRead(LIGHT_SENSOR[6]) < COLOR_BLACK && analogRead(LIGHT_SENSOR[7]) < COLOR_BLACK)
  {
    motorForwardWithPulses(200, 9);
    motorTurnLeft(255);
    while (true)
    {
      motorLeft(210);
      if (analogRead(LIGHT_SENSOR[4]) > COLOR_BLACK)
      {
        break;
      }
    }
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

  if (analogRead(LIGHT_SENSOR[7]) > COLOR_BLACK)
  {
    do
    {
      motorLeft(200);
      if (analogRead(LIGHT_SENSOR[6]) > COLOR_BLACK)
      {
        break;
      }
    } while (true);
  }
  else if (analogRead(LIGHT_SENSOR[2]) > COLOR_BLACK || analogRead(LIGHT_SENSOR[1]) > COLOR_BLACK || analogRead(LIGHT_SENSOR[0]) > COLOR_BLACK)
  {
    do
    {
      motorRight(200);
      if (analogRead(LIGHT_SENSOR[3]) > COLOR_BLACK)
      {
        break;
      }
    } while (true);
  }
  else
  {
    do
    {
      motorLeft(200);
      if (analogRead(LIGHT_SENSOR[6]) > COLOR_BLACK)
      {
        break;
      }
    } while (true);
  }
  motorStop();
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
  analogWrite(MOTOR_A_1, 255);
  analogWrite(MOTOR_A_2, 0);
  analogWrite(MOTOR_B_1, 255 - MOTOR_B_Afwijking);
  analogWrite(MOTOR_B_2, 0);

  delay(10);

  analogWrite(MOTOR_A_1, motorSpeed);
  analogWrite(MOTOR_A_2, 0);
  analogWrite(MOTOR_B_1, motorSpeed - MOTOR_B_Afwijking);
  analogWrite(MOTOR_B_2, 0);
}
void motorLeft(int motorSpeed)
{
  leftLights();
  analogWrite(MOTOR_A_1, 0);
  analogWrite(MOTOR_A_2, 255);
  analogWrite(MOTOR_B_1, 0);
  analogWrite(MOTOR_B_2, 255 - MOTOR_B_Afwijking);

  delay(10);

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
  reversingLight();
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
void reversingLight()
{
  pixels.setPixelColor(0, pixels.Color(0, 50, 0));
  pixels.setPixelColor(1, pixels.Color(255, 255, 255));
  pixels.setPixelColor(2, pixels.Color(50, 50, 50));
  pixels.setPixelColor(3, pixels.Color(50, 50, 50));
  pixels.show();
}
void setLightStartUpLight(int lightNR)
{
  pixels.setPixelColor(lightNR, pixels.Color(0, 0, 255));
  pixels.show();
}
void lightsOff()
{
  pixels.setPixelColor(0, pixels.Color(0, 0, 0));
  pixels.setPixelColor(1, pixels.Color(0, 0, 0));
  pixels.setPixelColor(2, pixels.Color(0, 0, 0));
  pixels.setPixelColor(3, pixels.Color(0, 0, 0));
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
