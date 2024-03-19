
// Motor pins
const int A1_MOTOR_PIN = 6;
const int A2_MOTOR_PIN = 11;
const int B1_MOTOR_PIN = 5;
const int B2_MOTOR_PIN = 10;
const int R1_ROTATION_PIN = 3;
const int R2_ROTATION_PIN = 2;

// Servo pins
const int SERVO_PIN = 9;
const int TRIGGER_PIN = A4;
const int ECHO_PIN = A3;

// Rotation intergers
int r1Rotations = 0;
int r2Rotations = 0;

// Approx*
const int ONE_CM_IN_ROTATIONS = 2;

// echo sensor variables
long duration;
int distance;

// Variables for determining pulse movement
int pulsesToMove;

void setup()
{
  // Serial
  Serial.begin(9600);

  // Setup rotation pins and attach routine
  pinMode(R1_ROTATION_PIN, INPUT);
  pinMode(R2_ROTATION_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(R1_ROTATION_PIN), rotateR1, CHANGE);
  attachInterrupt(digitalPinToInterrupt(R2_ROTATION_PIN), rotateR2, CHANGE);

  // Echo pins
  pinMode(SERVO_PIN, OUTPUT);
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // Motor pins
  pinMode(A1_MOTOR_PIN, OUTPUT);
  pinMode(A2_MOTOR_PIN, OUTPUT);
  pinMode(B1_MOTOR_PIN, OUTPUT);
  pinMode(B2_MOTOR_PIN, OUTPUT);
}

void rotateR1()
{
  r1Rotations++;
}

void rotateR2()
{
  r2Rotations++;
}

void loop()
{
  if (pulsesToMove == 0)
  {
    // Move the servo forward
    moveServo(96);
    delay(250);
    // scan for distance
    int cmToMove = getDistanceFromPulse();
    // Calculate distance if pulses

    if (cmToMove < 12)
    {
      moveServo(180);
      delay(250);
      sendPulse();

      if (distance >= 30)
      {
        stopMoving();
        delay(150);
        turnLeft();
      }
      else if (distance < 25) 
      {
        stopMoving();
        delay(150);
        turnLeft();
        delay(500);
      }
    }

    pulsesToMove = cmToMove * ONE_CM_IN_ROTATIONS;

    moveForward(255, 255);

    moveServo(0); // Servo naar rechts draaien
    delay(250);
  }
  else if (r1Rotations >= pulsesToMove)
  {
    stopMoving();
    Serial.println("Approx CM moved:");
    Serial.print(r1Rotations / ONE_CM_IN_ROTATIONS);
    pulsesToMove = 0;
    r1Rotations = 0;
  }
  else
  {
    sendPulse(); // Puls versturen
    adjustDirection();
  }
}

void turnLeft()
{
  moveBackward(200,200);
  delay(250);
  stopMoving();
  delay(100);

  r2Rotations = 0;

  while(r2Rotations < 30)
  { 
    moveLeft(0,255);
    Serial.println(r2Rotations);
  }
  stopMoving();

  r2Rotations = 0;

  delay(250);

  while (r2Rotations < 30)
  {
    moveBackward(200, 200);
    Serial.println(r2Rotations);
  }

  stopMoving();
  
  pulsesToMove = 0;
  r1Rotations = 0;
  r2Rotations = 0;
}

void turnRight()
{
  r1Rotations = 0;

  while(r1Rotations < 35)
  { 
    moveRight(255,0);
    Serial.println(r1Rotations);
  }

  stopMoving();

  r1Rotations = 0;

  delay(250);

  while (r1Rotations < 30)
  {
    Serial.println(r1Rotations);
    moveBackward(200, 200);
  }

  stopMoving();

  pulsesToMove = 0;
  r1Rotations = 0;
  r2Rotations = 0;
}

void moveRight(int leftSpeed, int rightSpeed)
{
  analogWrite(A1_MOTOR_PIN, 0);
  analogWrite(A2_MOTOR_PIN, leftSpeed);
  analogWrite(B1_MOTOR_PIN, rightSpeed);
  analogWrite(B2_MOTOR_PIN, 0);
}

void moveLeft(int leftSpeed, int rightSpeed)
{
  analogWrite(A1_MOTOR_PIN, leftSpeed);
  analogWrite(A2_MOTOR_PIN, 0);
  analogWrite(B1_MOTOR_PIN, 0);
  analogWrite(B2_MOTOR_PIN, rightSpeed);
}

void moveServo(int angle)
{
  for (int i = 0; i < 10; i++)
  {
    int pulseWidth = map(angle, 0, 180, 544, 2400);

    digitalWrite(SERVO_PIN, HIGH);
    delayMicroseconds(pulseWidth);
    digitalWrite(SERVO_PIN, LOW);
  }
}

void sendPulse()
{
  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);

  duration = pulseIn(ECHO_PIN, HIGH);
  distance = duration * 0.034 / 2;

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
}

int getDistanceFromPulse()
{
  int avaragePulse = 0;

  for (int i = 0; i < 5; i++)
  {
    sendPulse();

    avaragePulse += distance;
    delay(10);
  }

  avaragePulse /= 5;

  Serial.print("Approx distance able to travel: ");
  Serial.print(avaragePulse);
  Serial.println(" cm");

  return avaragePulse;
}

void moveForward(int leftSpeed, int rightSpeed)
{
  analogWrite(A1_MOTOR_PIN, 0);
  analogWrite(A2_MOTOR_PIN, leftSpeed);
  analogWrite(B1_MOTOR_PIN, 0);
  analogWrite(B2_MOTOR_PIN, rightSpeed);
}

void moveBackward(int leftSpeed, int rightSpeed)
{
  analogWrite(A1_MOTOR_PIN, leftSpeed);
  analogWrite(A2_MOTOR_PIN, 0);
  analogWrite(B1_MOTOR_PIN, rightSpeed);
  analogWrite(B2_MOTOR_PIN, 0);
}

void stopMoving()
{
  analogWrite(A1_MOTOR_PIN, 0);
  analogWrite(A2_MOTOR_PIN, 0);
  analogWrite(B1_MOTOR_PIN, 0);
  analogWrite(B2_MOTOR_PIN, 0);
}

void adjustDirection()
{
  int forwardCreepRotations = r1Rotations + 20;

  if (distance > 13 && distance <= 25)  
  {
    moveForward(255, 255); // Go straight
  }
  else if (distance < 13)
  {
    moveForward(200, 255); // Turn slightly left
  }

  if (distance > 25)
  { 
    int creepForwardRotations = r1Rotations + 36;

    while (r1Rotations < creepForwardRotations)
    {
      moveForward(200,200);
      Serial.println(r1Rotations);
    }

    stopMoving();
    turnRight();    
  }
}
