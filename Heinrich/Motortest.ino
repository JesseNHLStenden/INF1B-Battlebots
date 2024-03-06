const int motorB = 2;
const int motorA = 3;

void setup(){
  pinMode(motorB, OUTPUT);
  pinMode(motorA, OUTPUT);
}
void loop(){
  digitalWrite(motorA, HIGH);
  digitalWrite(motorB, HIGH);

}
