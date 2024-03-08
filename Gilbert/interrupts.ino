// Motor pins
const int A1_MOTOR_PIN = 6;
const int A2_MOTOR_PIN = 11;

const int B1_MOTOR_PIN = 5;
const int B2_MOTOR_PIN = 10;

int R1_ROTATION_PIN = 3;
int R2_ROTATION_PIN = 2;

const int ledPin = 13;

unsigned int r1Rotations = 0;
unsigned int r2Rotations = 0;

double wheelOffset = 1.0;

unsigned long lastMillis = 0;

int millisInterval = 100;

void setup() {
  Serial.begin(9600);
  pinMode(12, OUTPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(R1_ROTATION_PIN, INPUT);
  pinMode(R2_ROTATION_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(R1_ROTATION_PIN), rotateR1, CHANGE);
  attachInterrupt(digitalPinToInterrupt(R2_ROTATION_PIN), rotateR2, CHANGE);
}

void rotateR1() {
  r1Rotations++;
}

void rotateR2() {
  r2Rotations++;
}

void loop() { 
  // moveForward(220.0, 220.0);

  if(r2Rotations < 37.5){
    analogWrite(B1_MOTOR_PIN, 0);
    analogWrite(B2_MOTOR_PIN, 200);
    Serial.println(r2Rotations);
  }
  else{
    analogWrite(B1_MOTOR_PIN, 0);
    analogWrite(B2_MOTOR_PIN, 0);
    delay(2000);
    Serial.println(r2Rotations);
    r2Rotations = 0;
  }
  Serial.println(r2Rotations);
  
  //unsigned long currentMillis = millis();  
  //if (currentMillis - lastMillis >= millisInterval) {
    //lastMillis = currentMillis; // save the last executed time
    //moveForward(200, 200 );
  //}  
}

// calculateWheelOffset();
// moveForward(220.0, 220.0 * wheelOffset);

void moveForward(int leftSpeed, int rightSpeed) {
  int pulseDifference = r1Rotations - r2Rotations;
  float speedModifier = 1;
  
  leftSpeed = leftSpeed - (speedModifier * pulseDifference);
  rightSpeed = rightSpeed + (speedModifier * pulseDifference);  
    
  int c_leftSpeed = constrain(leftSpeed, 0 , 255);
  int c_rightSpeed = constrain(rightSpeed, 0 , 255);

  analogWrite(A1_MOTOR_PIN, 0);
  analogWrite(A2_MOTOR_PIN, c_leftSpeed);
  analogWrite(B1_MOTOR_PIN, 0);
  analogWrite(B2_MOTOR_PIN, c_rightSpeed);

  Serial.print("Pulse difference:");
  Serial.println(pulseDifference);
  Serial.print("Pulses left motor: ");
  Serial.println(r1Rotations);
  Serial.print("Pulses right motor: ");
  Serial.println(r2Rotations);
  Serial.print("Speed left motor: ");
  Serial.println(leftSpeed);
  Serial.print("Speed right motor: ");
  Serial.println(rightSpeed);
}
