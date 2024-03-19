const int SERVO_PIN = 9;
const int trigPin = A4;
const int echoPin = A3;
const int MOTOR_LEFT_PIN1 = 11;
const int MOTOR_LEFT_PIN2 = 6;
const int MOTOR_RIGHT_PIN1 = 5;
const int MOTOR_RIGHT_PIN2 = 10;
const int MOTOR_ROTATION_PIN_R1 = 12;
const int MOTOR_ROTATION_PIN_R2 = 13;

const float WIEL_RADIUS_CM = 20.0; // Radius van het wiel in cm
const int PULSEN_PER_VOLLEDIGE_DRAAI = 44; // Aantal pulsen per volledige draai

long duration;
int distance;
int pulsesToMove; // Variabele om het aantal pulsen voor beweging op te slaan

void setup() {
  Serial.begin(9600);
  pinMode(SERVO_PIN, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(MOTOR_LEFT_PIN1, OUTPUT);
  pinMode(MOTOR_LEFT_PIN2, OUTPUT);
  pinMode(MOTOR_RIGHT_PIN1, OUTPUT);
  pinMode(MOTOR_RIGHT_PIN2, OUTPUT);
  pinMode(MOTOR_ROTATION_PIN_R1, INPUT);
  pinMode(MOTOR_ROTATION_PIN_R2, INPUT);
}

void moveServo(int angle) {
  int pulseWidth = map(angle, 0, 180, 544, 2400);
  
  digitalWrite(SERVO_PIN, HIGH);
  delayMicroseconds(pulseWidth);
  digitalWrite(SERVO_PIN, LOW);
  delay(20);
}

void moveForward(int leftSpeed, int rightSpeed) {
  analogWrite(MOTOR_LEFT_PIN1, leftSpeed);
  analogWrite(MOTOR_LEFT_PIN2, 0);
  analogWrite(MOTOR_RIGHT_PIN1, 0);
  analogWrite(MOTOR_RIGHT_PIN2, rightSpeed);
}
void moveBackward(int leftSpeed, int rightSpeed) {
  analogWrite(MOTOR_LEFT_PIN1, 0);
  analogWrite(MOTOR_LEFT_PIN2, leftSpeed);
  analogWrite(MOTOR_RIGHT_PIN1, rightSpeed);
  analogWrite(MOTOR_RIGHT_PIN2, 0);
}

void stopMoving() {
  analogWrite(MOTOR_LEFT_PIN1, 0);
  analogWrite(MOTOR_LEFT_PIN2, 0);
  analogWrite(MOTOR_RIGHT_PIN1, 0);
  analogWrite(MOTOR_RIGHT_PIN2, 0);
}

void moveRight(int leftSpeed, int rightSpeed) {
  analogWrite(MOTOR_LEFT_PIN1, leftSpeed);
  analogWrite(MOTOR_LEFT_PIN2, 0);
  analogWrite(MOTOR_RIGHT_PIN1, rightSpeed);
  analogWrite(MOTOR_RIGHT_PIN2, 0);
}

void moveLeft(int leftSpeed, int rightSpeed) {
  analogWrite(MOTOR_LEFT_PIN1, 0);
  analogWrite(MOTOR_LEFT_PIN2, leftSpeed);
  analogWrite(MOTOR_RIGHT_PIN1, 0);
  analogWrite(MOTOR_RIGHT_PIN2, rightSpeed);
}

void sendPulse() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;
  
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
}


void loop() {
  moveServo(90); 
  sendPulse();
  
  if (distance < 10) {
    stopMoving();
    moveServo(0); 
    delay(500);
    sendPulse();
    delay(200);
    
    if (distance < 25) {
      moveServo(180); 
      delay(500);
      sendPulse();
      delay(200);

      if (distance < 25) {
          moveServo(180); 
          delay(500);
          while (distance < 25) {
          moveServo(180);
          moveBackward(255, 255); // Deze functie moet gedefinieerd zijn en correct werken
          sendPulse();
        
          if (distance > 25) {
            moveServo(90);
            moveLeft(255, 255); // Deze functie moet gedefinieerd zijn en correct werken
            delay(400);
        }
        }
      }
    } else if (distance > 25) {
      moveServo(90);
      moveRight(255, 255); // Deze functie moet gedefinieerd zijn en correct werken
      delay(450);
    }
  } else {
    // Ga naar voren
    moveForward(245, 255); // Deze functie moet gedefinieerd zijn en correct werken
  }
}
