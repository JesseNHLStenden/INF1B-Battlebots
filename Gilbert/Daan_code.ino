#include <Servo.h>

Servo myservo;

const int trigPin = A4;
const int echoPin = A5;
#define MOTOR_LEFT_PIN1 11
#define MOTOR_LEFT_PIN2 6
#define MOTOR_RIGHT_PIN1 5
#define MOTOR_RIGHT_PIN2 10
#define MOTOR_ROTATION_PIN_R1 12
#define MOTOR_ROTATION_PIN_R2 13

long duration;
int distance;

void setup() {
  Serial.begin(9600);
  myservo.attach(9); // Servo op pin 9
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(MOTOR_LEFT_PIN1, OUTPUT);
  pinMode(MOTOR_LEFT_PIN2, OUTPUT);
  pinMode(MOTOR_RIGHT_PIN1, OUTPUT);
  pinMode(MOTOR_RIGHT_PIN2, OUTPUT);
  pinMode(MOTOR_ROTATION_PIN_R1, INPUT);
  pinMode(MOTOR_ROTATION_PIN_R2, INPUT);
}

void moveForward(int leftSpeed, int rightSpeed) {
  digitalWrite(MOTOR_LEFT_PIN1, LOW);
  analogWrite(MOTOR_LEFT_PIN2, leftSpeed);
  digitalWrite(MOTOR_RIGHT_PIN1, LOW);
  analogWrite(MOTOR_RIGHT_PIN2, rightSpeed);
}

void moveBackward(int leftSpeed, int rightSpeed) {
   analogWrite(MOTOR_LEFT_PIN1, leftSpeed);
  digitalWrite(MOTOR_LEFT_PIN2, LOW);
  analogWrite(MOTOR_RIGHT_PIN1, rightSpeed);
  digitalWrite(MOTOR_RIGHT_PIN2, LOW);
}

void moveLeft(int leftSpeed, int rightSpeed) {
  analogWrite(MOTOR_LEFT_PIN1, leftSpeed);
  digitalWrite(MOTOR_LEFT_PIN2, LOW);
  digitalWrite(MOTOR_RIGHT_PIN1, LOW);
  analogWrite(MOTOR_RIGHT_PIN2, rightSpeed);
}

void moveRight(int leftSpeed, int rightSpeed) {
  digitalWrite(MOTOR_LEFT_PIN1, LOW);
  analogWrite(MOTOR_LEFT_PIN2, leftSpeed);
  analogWrite(MOTOR_RIGHT_PIN1, rightSpeed);
  digitalWrite(MOTOR_RIGHT_PIN2, LOW);
}

void stopMoving() {
  digitalWrite(MOTOR_LEFT_PIN1, LOW);
  digitalWrite(MOTOR_LEFT_PIN2, LOW);
  digitalWrite(MOTOR_RIGHT_PIN1, LOW);
  digitalWrite(MOTOR_RIGHT_PIN2, LOW);
}

void scanArea() {
    motorForward(); 
    delay(350);
    sendPulse();
    motorRight(); 
    delay(350);
    sendPulse();
}

void motorLeft(){
  myservo.write(180);
}

void motorRight(){
  myservo.write(0);
}

void motorForward(){
  myservo.write(90);
}
void sendPulse() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;

  Serial.print("Angle: ");
  Serial.print(myservo.read());
  Serial.print(" - Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
}

void loop() {
  motorForward(); 
  sendPulse();
  if (distance < 10) {
    stopMoving();
    motorRight(); 
    delay(500);
    sendPulse();
    delay(200);
     if (distance < 25) {
        motorLeft(); 
        delay(500);
        sendPulse();
        delay(200);
        if (distance < 25) 
        {
          while (distance < 25) {
          moveBackward(255, 255);
          delay(1000);
          motorRight();
          delay(500);
          sendPulse();
        if (distance > 25) {
          moveRight(255, 255);
          delay(445);
        }
        moveBackward(255, 255);
        delay(1000);
        motorLeft();
        delay(500);
        sendPulse();
        if (distance > 25) {
          moveLeft(255, 255);
          delay(445);
        }
      }
        } else 
        {
          moveLeft(255,255);
          delay(445);
        }
        
    } else if (distance > 25) {
        // Draai naar rechts
        moveRight(255, 255);
        delay(445);
    }
  } else {
    // Ga naar voren
    moveForward(243.5, 255);
  }
}
