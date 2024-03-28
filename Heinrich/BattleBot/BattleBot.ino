#include <Adafruit_NeoPixel.h>
#include <pitches.h>

#define NUM_PIXELS 4 // Number of NeoPixels in the strip
#define PIXEL_PIN 7  // Pin connected to the NeoPixels

#define GRIPPER_OPEN 1600   // Puls lengte Open
#define GRIPPER_CLOSE 800   // Puls lengte Open
#define SERVO_INTERVAL 20   // Servo millis interval
#define GRIPPER_TOGGLE 1000 // Toggle interval

#define BUZZER_PIN 8 // Define the pin to which the buzzer is connected

// Define the durations for each note (in milliseconds)
#define QUARTER_NOTE 250
#define EIGHTH_NOTE 125
#define HALF_NOTE 500

// Create NeoPixel object
Adafruit_NeoPixel pixels(NUM_PIXELS, PIXEL_PIN, NEO_GRB + NEO_KHZ800);

/*
NEOPIXEL LIGHTS CONFIGURATION
Linksachter: 0
Rechtsachter: 1
Rechtsvoor: 2
Linksvoor: 3
*/

int melody[] = {
  NOTE_E5, NOTE_D5, NOTE_FS4, NOTE_GS4, 
  NOTE_CS5, NOTE_B4, NOTE_D4, NOTE_E4, 
  NOTE_B4, NOTE_A4, NOTE_CS4, NOTE_E4,
  NOTE_A4
};

int durations[] = {
  8, 8, 4, 4,
  8, 8, 4, 4,
  8, 8, 4, 4,
  2
};

int melodyTetris[] = {
  NOTE_E5, NOTE_B4, NOTE_C5, NOTE_D5, NOTE_C5, NOTE_B4,
  NOTE_A4, NOTE_A4, NOTE_C5, NOTE_E5, NOTE_D5, NOTE_C5,
  NOTE_B4, NOTE_C5, NOTE_D5, NOTE_E5,
  NOTE_C5, NOTE_A4, NOTE_A4, NOTE_A4, NOTE_B4, NOTE_C5,
  
  NOTE_D5, NOTE_F5, NOTE_A5, NOTE_G5, NOTE_F5,
  NOTE_E5, NOTE_C5, NOTE_E5, NOTE_D5, NOTE_C5,
  NOTE_B4, NOTE_B4, NOTE_C5, NOTE_D5, NOTE_E5,
  NOTE_C5, NOTE_A4, NOTE_A4, REST, 
  
  NOTE_E5, NOTE_B4, NOTE_C5, NOTE_D5, NOTE_C5, NOTE_B4,
  NOTE_A4, NOTE_A4, NOTE_C5, NOTE_E5, NOTE_D5, NOTE_C5,
  NOTE_B4, NOTE_C5, NOTE_D5, NOTE_E5,
  NOTE_C5, NOTE_A4, NOTE_A4, NOTE_A4, NOTE_B4, NOTE_C5,
  
  NOTE_D5, NOTE_F5, NOTE_A5, NOTE_G5, NOTE_F5,
  NOTE_E5, NOTE_C5, NOTE_E5, NOTE_D5, NOTE_C5,
  NOTE_B4, NOTE_B4, NOTE_C5, NOTE_D5, NOTE_E5,
  NOTE_C5, NOTE_A4, NOTE_A4, REST, 
  
  NOTE_E5, NOTE_C5,
  NOTE_D5, NOTE_B4,
  NOTE_C5, NOTE_A4,
  NOTE_GS4, NOTE_B4, REST, 
  NOTE_E5, NOTE_C5,
  NOTE_D5, NOTE_B4,
  NOTE_C5, NOTE_E5, NOTE_A5,
  NOTE_GS5
};

int durationsTetris[] = {
  4, 8, 8, 4, 8, 8,
  4, 8, 8, 4, 8, 8,
  4, 8, 4, 4,
  4, 4, 8, 4, 8, 8,
  
  4, 8, 4, 8, 8,
  4, 8, 4, 8, 8,
  4, 8, 8, 4, 4,
  4, 4, 4, 4,
  
  4, 8, 8, 4, 8, 8,
  4, 8, 8, 4, 8, 8,
  4, 8, 4, 4,
  4, 4, 8, 4, 8, 8,
  
  4, 8, 4, 8, 8,
  4, 8, 4, 8, 8,
  4, 8, 8, 4, 4,
  4, 4, 4, 4,
  
  2, 2,
  2, 2,
  2, 2,
  2, 4, 8, 
  2, 2,
  2, 2,
  4, 4, 2,
  2
};

int colorValues[] = {0, 0, 0, 0, 0, 0}; // Lege array om de waardes van de waardes te lezen

const int MOTOR_SERVO = 9; // Pin voor gripper

const int MOTOR_A_1 = 3;          // Motor A1
const int MOTOR_A_2 = 5;          // Motor A2
const int MOTOR_B_1 = 6;          // Motor B1
const int MOTOR_B_2 = 11;         // Motor B2
const int MOTOR_B_Afwijking = 65; // Motor b afwijking

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
  pinMode(BUZZER_PIN, OUTPUT);

  for (int i = 0; i < 8; i++)
  {
    pinMode(LIGHT_SENSOR[i], INPUT); // Deze functie zorgt ervoor dat elke lichtsensor bij de juiste pinMode wordt gezet
  }

  pinMode(MOTOR_SERVO, OUTPUT);
  digitalWrite(MOTOR_SERVO, LOW);

  pixels.begin();

  lightsOff();

  motorForward(255);
  
  delay(5000);

  motorStop();
  
  int randomColorGreen = random(0, 255);
  int randomColorRed = random(0, 255);
  int randomColorBlue = random(0, 255);

  int size = sizeof(durationsTetris) / sizeof(int);

  int i = 0;

  for (int note = 0; note < size; note++)
  {
    int duration = 1000 / durationsTetris[note];
    tone(BUZZER_PIN, melodyTetris[note], duration);
    int pauseBetweenNotes = duration * 1.30;
    delay(pauseBetweenNotes);
    noTone(BUZZER_PIN);

    lightsOff();
    pixels.setPixelColor(i, pixels.Color(randomColorGreen, randomColorRed, randomColorBlue));
    pixels.show();
    i = i == 0 ? 3 : i - 1;
    if (i == 3)
    {
      randomColorGreen = random(0, 255);
      randomColorRed = random(0, 255);
      randomColorBlue = random(0, 255);
    }
  }
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
        pixels.setPixelColor(lightPlace, pixels.Color(0, 255, 0));
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
    motorForward(230);

    for (int i = 0; i < 6; i += 2) // Deze functie zorgt voor de calibratie bij de startsetup. Deze code wordt 3 keer uitgevoerd.
    {
      while (analogRead(LIGHT_SENSOR[1]) > COLOR_BLACK && analogRead(LIGHT_SENSOR[6]) > COLOR_BLACK)
      {
        delay(1);
      }

      colorValues[i] = analogRead(LIGHT_SENSOR[3]); // Hier worden de gemeten waardes van zwart op de juiste variabele gezet
      Serial.println(colorValues[i]);

      delay(100);
      motorForward(200);
      while (analogRead(LIGHT_SENSOR[1]) < COLOR_BLACK && analogRead(LIGHT_SENSOR[6]) < COLOR_BLACK)
      {
        delay(1);
      }

      colorValues[i + 1] = analogRead(LIGHT_SENSOR[3]); // Deze functie zorgt ervoor dat alle sensoren de waardes van zwart meekrijgen
      Serial.println(colorValues[i + 1]);

      delay(100);
      motorForward(200);
    }

    motorForward(210);

    while (analogRead(LIGHT_SENSOR[3]) < COLOR_BLACK)
    {
      delay(1);
    }
    while (analogRead(LIGHT_SENSOR[3]) > COLOR_BLACK)
    {
      delay(1);
    }

    COLOR_BLACK = average(colorValues, 6) + 250; // Deze functie zorgt ervoor dat er een duidelijke drempel tussen wit en zwart is

    servoState(GRIPPER_CLOSE);

    while (analogRead(LIGHT_SENSOR[2]) < COLOR_BLACK && analogRead(LIGHT_SENSOR[6]) < COLOR_BLACK) // Wacht tot de lichtsensoren zwart detecteren, door het tegengestelde in een while loop te zetten
    {
      delay(1);
    }

    while (analogRead(LIGHT_SENSOR[2]) > COLOR_BLACK && analogRead(LIGHT_SENSOR[6]) > COLOR_BLACK) // Wacht tot de lichtsensoren wit detecteren, door het tegengestelde in een while loop te zetten
    {
      delay(1);
    }

    motorStop();

    for (int i = 0; i < 100; i++) // Deze functie sluit de gripper
    {
      delay(1);
      servoState(GRIPPER_CLOSE);
    }

    motorLeft(220);

    delay(300);

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
    if (analogRead(LIGHT_SENSOR[3]) > COLOR_BLACK && analogRead(LIGHT_SENSOR[4]) > COLOR_BLACK && analogRead(LIGHT_SENSOR[5]) > COLOR_BLACK)
    { // Zodra Heinrich weer zwart ziet na 3 pulses stopt Heinrich met de code.
      motorStop();
      servoState(GRIPPER_OPEN); // De pion wordt losgelaten
      motorBackward(255);
      delay(3000);
      motorStop();
      while (true)
      { // Deze functie zorgt ervoor dat na de race Heinrich leuke verschillende kleuren laat zien

        int i = 0;

        int randomColorGreen = random(0, 255);
        int randomColorRed = random(0, 255);
        int randomColorBlue = random(0, 255);

        int size = sizeof(durations) / sizeof(int);

        for (int note = 0; note < size; note++)
        {
          int duration = 1000 / durations[note];
          tone(BUZZER_PIN, melody[note], duration);

          int pauseBetweenNotes = duration * 1.30;
          delay(pauseBetweenNotes);

          noTone(BUZZER_PIN);

          lightsOff();
          pixels.setPixelColor(i, pixels.Color(randomColorGreen, randomColorRed, randomColorBlue));
          pixels.show();

          i = i == 0 ? 3 : i - 1;
          if (i == 3)
          {
            randomColorGreen = random(0, 255);
            randomColorRed = random(0, 255);
            randomColorBlue = random(0, 255);
          }
        }

        delay(1000);
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
    motorForward(255); // Deze functie zorgt ervoor dat als Heinrich zwart ziet op de middelste (als de rechter sensoren geen zwart zien), dat hij door blijft rijden.
  }
  else if (analogRead(LIGHT_SENSOR[0]) < COLOR_BLACK && analogRead(LIGHT_SENSOR[1]) < COLOR_BLACK && analogRead(LIGHT_SENSOR[2]) < COLOR_BLACK && analogRead(LIGHT_SENSOR[3]) < COLOR_BLACK && analogRead(LIGHT_SENSOR[4]) < COLOR_BLACK && analogRead(LIGHT_SENSOR[5]) < COLOR_BLACK && analogRead(LIGHT_SENSOR[4]) < COLOR_BLACK && analogRead(LIGHT_SENSOR[3]) < COLOR_BLACK && analogRead(LIGHT_SENSOR[6]) < COLOR_BLACK && analogRead(LIGHT_SENSOR[7]) < COLOR_BLACK)
  {
    motorForwardWithPulses(200, 9); // Deze functie zorgt ervoor dat als Heinrich links zwart ziet (als de rechter en middelste sensoren geen zwart zien), dat hij linksaf gaat.
    motorLeft(245);
    while (true)
    {
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
    motorLeft(245);
    while (true)
    {
      leftLights();
      if (analogRead(LIGHT_SENSOR[6]) > COLOR_BLACK) // Heinrich gaat linksaf tot hij weer zwart ziet op
      {
        break;
      }
    }
  }
  else if (analogRead(LIGHT_SENSOR[2]) > COLOR_BLACK || analogRead(LIGHT_SENSOR[1]) > COLOR_BLACK || analogRead(LIGHT_SENSOR[0]) > COLOR_BLACK) // Controleert of Heinrich links op de lijn zit
  {
    motorRight(200);
    while (true)
    {
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
      motorLeft(245);
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

  while (!motor1Ready || !motor2Ready) // Terwijl de motoren nog niet klaar zijn, wordt dit stukje code herhaald tot alle rotaties zijn gedaan.
  {

    if (!motor1Ready) // Als de linkermotor niet zijn rotaties heeft gedaan, wordt er nog een rotatie gedaan.
    {
      analogWrite(MOTOR_A_1, motorSpeed);
      analogWrite(MOTOR_A_2, 0);
    }
    else // Als de linkermotor zijn rotaties heeft gedaan, worden er geen rotaties meer gedaan.
    {
      analogWrite(MOTOR_A_1, 0);
      analogWrite(MOTOR_A_2, 0);
    }

    if (!motor2Ready) // Als de rechtermotor niet zijn rotaties heeft gedaan, wordt er nog een rotatie gedaan.
    {
      analogWrite(MOTOR_B_1, 0);
      analogWrite(MOTOR_B_2, motorSpeed - MOTOR_B_Afwijking);
    }
    else // Als de rechtermotor zijn rotaties heeft gedaan, worden er geen rotaties meer gedaan.
    {
      analogWrite(MOTOR_B_1, 0);
      analogWrite(MOTOR_B_2, 0);
    }

    int pulseMotor1Readed = digitalRead(MOTOR_SENSOR_1);
    int pulseMotor2Readed = digitalRead(MOTOR_SENSOR_2);

    if (previous1Pulse != pulseMotor1Readed) // Wanneer de huidige sensor status is niet hetzelfde als de vorige status
    {
      motor1PulsesDone++;
      previous1Pulse = pulseMotor1Readed;
    }

    if (previous2Pulse != pulseMotor2Readed) // Wanneer de huidige sensor status is niet hetzelfde als de vorige status
    {
      motor2PulsesDone++;
      previous2Pulse = pulseMotor2Readed;
    }

    motor1Ready = (motor1PulsesDone >= pulsesToDo); // Wanneer de pulsen klaar zijn, hoeft de motor niet meer te rijden
    motor2Ready = (motor2PulsesDone >= pulsesToDo); // Wanneer de pulsen klaar zijn, hoeft de motor niet meer te rijden
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

  while (!motor1Ready || !motor2Ready) // Terwijl de motoren nog niet klaar zijn, wordt dit stukje code herhaald tot alle rotaties zijn gedaan.
  {

    if (!motor1Ready) // Als de linkermotor niet zijn rotaties heeft gedaan, wordt er nog een rotatie gedaan.
    {
      analogWrite(MOTOR_A_1, motorSpeed);
      analogWrite(MOTOR_A_2, 0);
    }
    else // Als de linkermotor zijn rotaties heeft gedaan, worden er geen rotaties meer gedaan.
    {
      analogWrite(MOTOR_A_1, 0);
      analogWrite(MOTOR_A_2, 0);
    }

    if (!motor2Ready) // Als de rechtermotor niet zijn rotaties heeft gedaan, wordt er nog een rotatie gedaan.
    {
      analogWrite(MOTOR_B_1, motorSpeed);
      analogWrite(MOTOR_B_2, 0);
    }
    else // Als de rechtermotor zijn rotaties heeft gedaan, worden er geen rotaties meer gedaan.
    {
      analogWrite(MOTOR_B_1, 0);
      analogWrite(MOTOR_B_2, 0);
    }

    int pulseMotor1Readed = digitalRead(MOTOR_SENSOR_1);
    int pulseMotor2Readed = digitalRead(MOTOR_SENSOR_2);

    if (previous1Pulse != pulseMotor1Readed) // Wanneer de huidige sensor status is niet hetzelfde als de vorige status
    {
      motor1PulsesDone++;
      previous1Pulse = pulseMotor1Readed;
    }

    if (previous2Pulse != pulseMotor2Readed) // Wanneer de huidige sensor status is niet hetzelfde als de vorige status
    {
      motor2PulsesDone++;
      previous2Pulse = pulseMotor2Readed;
    }

    motor1Ready = (motor1PulsesDone >= pulsesToDo); // Wanneer de pulsen klaar zijn, hoeft de motor niet meer te rijden
    motor2Ready = (motor2PulsesDone >= pulsesToDo); // Wanneer de pulsen klaar zijn, hoeft de motor niet meer te rijden
  }
  motorStop();
}

void motorTurnLeft(int motorSpeed) // Deze werkt hetzelfde als vorige functie, maar dan linksaf
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

void motorRight(int motorSpeed) // Stuurt robot naar rechts, met een flinke boost, om zeker te zijn dat deze draait
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

void motorLeft(int motorSpeed) // Stuurt robot naar links, met een flinke boost, om zeker te zijn dat deze draait
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

void motorBackward(int motorSpeed) // Robot rijdt naar achter
{

  analogWrite(MOTOR_A_1, 0);
  analogWrite(MOTOR_A_2, motorSpeed);
  analogWrite(MOTOR_B_1, motorSpeed - MOTOR_B_Afwijking);
  analogWrite(MOTOR_B_2, 0);
  reversingLight();
}

void motorStop() // Alle motoren stoppen
{
  brakeLights();
  analogWrite(MOTOR_A_1, 0);
  analogWrite(MOTOR_A_2, 0);
  analogWrite(MOTOR_B_1, 0);
  analogWrite(MOTOR_B_2, 0);
}

float getDistanceCM() // Krijg de afstand van de sensoren in CM
{
  digitalWrite(AFSTAND_TRIGGER, HIGH);
  delayMicroseconds(10);
  digitalWrite(AFSTAND_TRIGGER, LOW);
  duration_us = pulseIn(AFSTAND_ECHO, HIGH);
  Serial.println(0.017 * duration_us);
  return 0.017 * duration_us;
}

void brakeLights() // Remlichten
{
  pixels.setPixelColor(0, pixels.Color(0, 255, 0));
  pixels.setPixelColor(1, pixels.Color(0, 255, 0));
  pixels.setPixelColor(2, pixels.Color(50, 50, 50));
  pixels.setPixelColor(3, pixels.Color(50, 50, 50));
  pixels.show();
}

void frontLights() // Standaard lampen
{
  pixels.setPixelColor(0, pixels.Color(0, 50, 0));
  pixels.setPixelColor(1, pixels.Color(0, 50, 0));
  pixels.setPixelColor(2, pixels.Color(255, 255, 255));
  pixels.setPixelColor(3, pixels.Color(255, 255, 255));
  pixels.show();
}

void leftLights() // Knipperlicht links
{
  pixels.setPixelColor(0, pixels.Color(64, 255, 0));
  pixels.setPixelColor(1, pixels.Color(0, 50, 0));
  pixels.setPixelColor(2, pixels.Color(50, 50, 50));
  pixels.setPixelColor(3, pixels.Color(64, 255, 0));
  pixels.show();
}

void rightLights() // Knipperlicht rechts
{
  pixels.setPixelColor(0, pixels.Color(0, 50, 0));
  pixels.setPixelColor(1, pixels.Color(64, 255, 0));
  pixels.setPixelColor(2, pixels.Color(64, 255, 0));
  pixels.setPixelColor(3, pixels.Color(50, 50, 50));
  pixels.show();
}

void standardLight() // Basis lichten
{
  pixels.setPixelColor(0, pixels.Color(0, 50, 0));
  pixels.setPixelColor(1, pixels.Color(0, 50, 0));
  pixels.setPixelColor(2, pixels.Color(50, 50, 50));
  pixels.setPixelColor(3, pixels.Color(50, 50, 50));
  pixels.show();
}

void reversingLight() // Achterruitrijlampen
{
  pixels.setPixelColor(0, pixels.Color(0, 50, 0));
  pixels.setPixelColor(1, pixels.Color(255, 255, 255));
  pixels.setPixelColor(2, pixels.Color(50, 50, 50));
  pixels.setPixelColor(3, pixels.Color(50, 50, 50));
  pixels.show();
}

void setLightStartUpLight(int lightNR) // Opstart lampen
{
  pixels.setPixelColor(lightNR, pixels.Color(255, 0, 0));
  pixels.show();
}

void lightsOff() // Alle lampen uit
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
  for (int i = 0; i < size; i++) // Voor elke opgenomen waarde gaat die de loop langs
  {
    sum += numbers[i]; // De totale gemeten lichtwaarde wordt hier opgeteld
  }
  return (int)sum / (double)size; // Door de totale gemeten lichtwaarde te delen door de aantal meetpogingen krijg je de gemiddelde(calibratie)
}

void gripperToggle() // Toggle de gripper
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

void servoState(int pulse) // Zet de servo voor meegegeven waarde
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
