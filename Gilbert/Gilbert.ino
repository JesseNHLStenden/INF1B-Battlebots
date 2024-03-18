
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

  moveServo(90);
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
    // scan for distance
    getDistanceFromPulse();
    // Calculate distance if pulses
    pulsesToMove = distance * ONE_CM_IN_ROTATIONS;
    // Drive forward based on pulses
    moveForward();
  }
  else if (r1Rotations >= pulsesToMove)
  {
    stopMoving();
    Serial.println("Approx CM moved:");
    Serial.print(r1Rotations / ONE_CM_IN_ROTATIONS);
    pulsesToMove = 0;
    r1Rotations = 0;
    delay(5000);
  }

  // Check if pulses are over their limit.

  // If not continue moving forward

  // If so stop moving and pulse forward/left

  // Based on feedback do whatever actions need to be done
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

int getDistanceFromPulse()
{
  int avaragePulse = 0;

  for (int i = 0; i < 5; i++)
  {
    digitalWrite(TRIGGER_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIGGER_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIGGER_PIN, LOW);

    duration = pulseIn(ECHO_PIN, HIGH);
    distance = duration * 0.034 / 2;

    Serial.print("Distance ping ");
    Serial.print(i);
    Serial.print(" Approx ");
    Serial.print(distance);
    Serial.println("CM");
    avaragePulse += distance;
    delay(1000);
  }

  avaragePulse /= 5;

  Serial.print("Approx distance able to travel: ");
  Serial.print(avaragePulse);
  Serial.println(" cm");

  return avaragePulse;
}

void moveForward()
{
  analogWrite(A1_MOTOR_PIN, 0);
  analogWrite(A2_MOTOR_PIN, 220);
  analogWrite(B1_MOTOR_PIN, 0);
  analogWrite(B2_MOTOR_PIN, 220);
}

void stopMoving()
{
  analogWrite(A1_MOTOR_PIN, 0);
  analogWrite(A2_MOTOR_PIN, 0);
  analogWrite(B1_MOTOR_PIN, 0);
  analogWrite(B2_MOTOR_PIN, 0);
}
