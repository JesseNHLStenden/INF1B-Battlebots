#include <Adafruit_NeoPixel.h>

#define NUM_PIXELS 4 // Number of NeoPixels in the strip
#define PIXEL_PIN 7  // Pin connected to the NeoPixels

#define GRIPPER_OPEN 1600   // Puls lengte Open
#define GRIPPER_CLOSE 800   // Puls lengte Open
#define SERVO_INTERVAL 20   // Servo millis interval
#define GRIPPER_TOGGLE 1000 // Toggle interval

// Create NeoPixel object
Adafruit_NeoPixel pixels(NUM_PIXELS, PIXEL_PIN, NEO_GRB + NEO_KHZ800);

/*
NEOPIXEL LIGHTS CONFIGURATION
Linksachter: 0
Rechtsachter: 1
Rechtsvoor: 2
Linksvoor: 3
*/

int colorValues[] = {0, 0, 0, 0, 0, 0}; // Lege array om de waardes van de waardes te lezen

const int MOTOR_SERVO = 9; // Pin voor gripper

const int MOTOR_A_1 = 3;          // Motor A1
const int MOTOR_A_2 = 5;          // Motor A2
const int MOTOR_B_1 = 6;          // Motor B1
const int MOTOR_B_2 = 11;         // Motor B2
const int MOTOR_B_Afwijking = 55; // Motor b afwijking

const int LIGHT_SENSOR[8] = {A5, A4, A6, A3, A2, A1, A0, A7}; // Array voor de pins van de lichtsensoren

const int MOTOR_SENSOR_1 = 12; // Pin voor R1
const int MOTOR_SENSOR_2 = 10; // Pin voor R2

const int AFSTAND_ECHO = 4;    // Pin voor echo's versturen
const int AFSTAND_TRIGGER = 2; // Pin voor echo's ontvangen (trigger)

int COLOR_BLACK = 900; // De lichtwaarde van zwart voor de calibratie om zwart te onderscheiden wit

float duration_us;

bool START_READY = false; // Zorgt ervoor dat de startsetup en calibratie eenmalig gebeurd

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
    pinMode(LIGHT_SENSOR[i], INPUT); // Deze functie zorgt ervoor dat elke lichtsensor bij de juiste pinMode wordt gezet
  }

  pinMode(MOTOR_SERVO, OUTPUT);
  digitalWrite(MOTOR_SERVO, LOW);

  pixels.begin();

  lightsOff();
}

void loop()
{
  if (START_READY) // Dit gebeurt er na de calibratie en de startsetup
  {
    standardLight();
    followLine();
  }
  else
  {
    getDistanceCM();

    for (int i = 0; i < 3; i++)
    {
      int lightPlace = 3;
      do
      {
        lightsOff();
        pixels.setPixelColor(lightPlace, pixels.Color(i, 255, i));
        pixels.show();
        lightPlace--;
        if (lightPlace == -1)
        {
          lightPlace = 3;
        }
        delay(100);
        servoState(GRIPPER_OPEN);
      } while (getDistanceCM() > 24); // Als er een object binnen 24 cm bereik staat dan begint de code
    }

    lightsOff();

    for (int i = 0; i < 4; i++) // Deze functie telt af voordat de robot begint
    {
      setLightStartUpLight(i);
      delay(500);
    }

    motorRight(200);
    delay(50);
    motorForward(255);

    for (int i = 0; i < 6; i += 2) // Deze functie zorgt voor de calibratie bij de startsetup. Deze code wordt 3 keer uitgevoerd.
    {
      while (analogRead(LIGHT_SENSOR[0]) > COLOR_BLACK && analogRead(LIGHT_SENSOR[6]) > COLOR_BLACK)
      {
        delay(1);
      }

      colorValues[i] = analogRead(LIGHT_SENSOR[3]); // Hier worden de gemeten waardes van zwart op de juiste variabele gezet
      Serial.println(colorValues[i]);

      delay(100);

      motorForward(210);
      while (analogRead(LIGHT_SENSOR[0]) < COLOR_BLACK && analogRead(LIGHT_SENSOR[6]) < COLOR_BLACK)
      {
        delay(1);
      }

      colorValues[i + 1] = analogRead(LIGHT_SENSOR[3]); // Deze functie zorgt ervoor dat alle sensoren de waardes van zwart meekrijgen
      Serial.println(colorValues[i + 1]);

      delay(100);

      motorForward(210);
    }

    motorForward(400);

    delay(200);

    COLOR_BLACK = average(colorValues, 6) + 250; // Deze functie zorgt ervoor dat er een duidelijke drempel tussen wit en zwart is

    servoState(GRIPPER_CLOSE);

    while (analogRead(LIGHT_SENSOR[4]) > COLOR_BLACK && analogRead(LIGHT_SENSOR[5]) > COLOR_BLACK)
    {
      delay(1);
    }
    while (analogRead(LIGHT_SENSOR[4]) < COLOR_BLACK) // Deze functie zorgt ervoor dat na de calibratie, zodra er wit wordt gezien, draait Heinrich 90 graden naar links
    {
      delay(1);
    }
    motorStop();
    for (int i = 0; i < 100; i++) // Deze functie sluit de gripper
    {
      delay(1);
      servoState(GRIPPER_CLOSE);
    }

    motorLeft(210);
    delay(500);
    while (true)
    {
      motorLeft(200);
      if (analogRead(LIGHT_SENSOR[6]) > COLOR_BLACK || analogRead(LIGHT_SENSOR[5]) > COLOR_BLACK) // Zodra de robot na calibreren en linksaf slaat de zwarte lijn weer ziet, is de startup klaar
      {
        break;
      }
    }
    START_READY = true;
  }
}

void followLine()
{
  servoState(GRIPPER_CLOSE);

  if (analogRead(LIGHT_SENSOR[0]) > COLOR_BLACK || analogRead(LIGHT_SENSOR[1]) > COLOR_BLACK) // Deze functie zorgt ervoor dat Heinrich vooruit rijdt zodra zwart wordt gezien
  {
    motorForwardWithPulses(200, 3);
    if (analogRead(LIGHT_SENSOR[0]) > COLOR_BLACK && analogRead(LIGHT_SENSOR[1]) > COLOR_BLACK && analogRead(LIGHT_SENSOR[2]) > COLOR_BLACK)
    { // Zodra Heinrich weer zwart ziet na 3 pulses stopt Heinrich met de code.
      motorStop();
      servoState(GRIPPER_OPEN); // De pion wordt losgelaten
      motorBackward(255);
      delay(3000);
      motorStop();
      while (true)
      { // Deze functie zorgt ervoor dat na de race Heinrich leuke verschillende kleuren laat zien
        int randomColorGreen = random(0, 255);
        int randomColorRed = random(0, 255);
        int randomColorBlue = random(0, 255);
        
        for (int i = 3; i >= 0; i--)
        {
          lightsOff();
          pixels.setPixelColor(i, pixels.Color(randomColorGreen, randomColorRed, randomColorBlue));
          pixels.show();
          delay(100);
        }
        
      }
    }
    else
    { // Deze functie zorgt ervoor dat als Heinrich geen zwart ziet na 3 pulses, dat hij rechtsaf gaat tot hij weer zwart ziet op de middelste sensoren.
      motorForwardWithPulses(200, 6);
      motorTurnRight(200);
      while (true)
      {
        motorRight(180);
        if (analogRead(LIGHT_SENSOR[3]) > COLOR_BLACK || analogRead(LIGHT_SENSOR[4]) > COLOR_BLACK)
        {
          break;
        }
      }
    }
  }
  else if (analogRead(LIGHT_SENSOR[5]) > COLOR_BLACK || analogRead(LIGHT_SENSOR[4]) > COLOR_BLACK || analogRead(LIGHT_SENSOR[3]) > COLOR_BLACK || analogRead(LIGHT_SENSOR[6]) > COLOR_BLACK)
  {
    motorForward(220); // Deze functie zorgt ervoor dat als Heinrich zwart ziet op de middelste (als de rechter sensoren geen zwart zien), dat hij door blijft rijden.
  }
  else if (analogRead(LIGHT_SENSOR[0]) < COLOR_BLACK && analogRead(LIGHT_SENSOR[1]) < COLOR_BLACK && analogRead(LIGHT_SENSOR[2]) < COLOR_BLACK && analogRead(LIGHT_SENSOR[3]) < COLOR_BLACK && analogRead(LIGHT_SENSOR[4]) < COLOR_BLACK && analogRead(LIGHT_SENSOR[5]) < COLOR_BLACK && analogRead(LIGHT_SENSOR[4]) < COLOR_BLACK && analogRead(LIGHT_SENSOR[3]) < COLOR_BLACK && analogRead(LIGHT_SENSOR[6]) < COLOR_BLACK && analogRead(LIGHT_SENSOR[7]) < COLOR_BLACK)
  {
    motorForwardWithPulses(200, 9); // Deze functie zorgt ervoor dat als Heinrich links zwart ziet (als de rechter en middelste sensoren geen zwart zien), dat hij linksaf gaat.
    // motorTurnLeft(255);
    while (true)
    {
      motorLeft(210);
      leftLights();
      if (analogRead(LIGHT_SENSOR[4]) > COLOR_BLACK)
      {
        break;
      }
    }
  }
  else
  {
    checkBothSides(); // Deze functie zorgt ervoor dat als Heinrich helemaal niks herkent en alles wit ziet (dus geen links, midden of rechts een zwarte kleur) dat hij linksaf gaat totdat hij weer zwart ziet in de middelste sensoren.
  }
}

void checkBothSides()
{
  motorStop();

  if (analogRead(LIGHT_SENSOR[7]) > COLOR_BLACK) // Controleert of Heinrich rechts op de lijn zit
  {
    while (true)
    {
      motorLeft(210);
      leftLights();
      if (analogRead(LIGHT_SENSOR[6]) > COLOR_BLACK) // Heinrich gaat linksaf tot hij weer zwart ziet op
      {
        break;
      }
    }
  }
  else if (analogRead(LIGHT_SENSOR[2]) > COLOR_BLACK || analogRead(LIGHT_SENSOR[1]) > COLOR_BLACK || analogRead(LIGHT_SENSOR[0]) > COLOR_BLACK) // Controleert of Heinrich links op de lijn zit
  {
    while (true)
    {
      motorRight(200);
      if (analogRead(LIGHT_SENSOR[3]) > COLOR_BLACK) // Heinrich gaat rechtsaf tot hij weer zwart ziet op
      {
        break;
      }
    }
  }
  else // Als alle lichtsensoren wit weerkaatst
  {
    while (true)
    {
      motorLeft(210);
      if (analogRead(LIGHT_SENSOR[7]) > COLOR_BLACK) // Heinrich gaat linksaf tot hij weer zwart ziet op
      {
        break;
      }
    }
  }
}

void motorForward(int motorSpeed)
{ // Heinrich gaat vooruit
  standardLight();
  analogWrite(MOTOR_A_1, motorSpeed);
  analogWrite(MOTOR_A_2, 0);
  analogWrite(MOTOR_B_1, 0);
  analogWrite(MOTOR_B_2, motorSpeed - MOTOR_B_Afwijking);
}

void motorForwardWithPulses(int motorSpeed, double pulses)
{ // Heinrich gaat vooruit met behulp van pulses
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

  motorForward(255);

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
  analogWrite(MOTOR_B_2, 255 /* - MOTOR_B_Afwijking*/);

  delay(10);

  analogWrite(MOTOR_A_1, 0);
  analogWrite(MOTOR_A_2, motorSpeed);
  analogWrite(MOTOR_B_1, 0);
  analogWrite(MOTOR_B_2, motorSpeed /* - MOTOR_B_Afwijking*/);
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
  pixels.setPixelColor(lightNR, pixels.Color(255, 0, 0));
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

void gripperToggle()
{
  static unsigned long timer;
  static bool state;
  if (millis() > timer)
  {
    if (state)
    {
      servoState(GRIPPER_OPEN);
    }
    else
    {
      servoState(GRIPPER_CLOSE);
    }

    state = !state;

    timer = millis() + GRIPPER_TOGGLE;
  }
}

void servoState(int pulse)
{
  static unsigned long timer;
  static int pulseUsed;
  if (pulse > 0)
  {
    pulseUsed = pulse;
  }
  if (millis() > timer)
  {
    digitalWrite(MOTOR_SERVO, HIGH);
    delayMicroseconds(pulseUsed);
    digitalWrite(MOTOR_SERVO, LOW);
    timer = millis() + SERVO_INTERVAL;
  }
}
