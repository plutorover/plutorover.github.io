#include <Servo.h>

int throttleRxPin = 7;
int servosRxPin = 8;
bool manualControl = false;

int pinRightMotorDirection = 2;
int pinRightMotorSpeed = 3;

int pinLeftMotorDirection = 4;
int pinLeftMotorSpeed = 5;

int pinRightFrontServo = 6;
int pinLeftFrontServo = 10;
int pinRightBackServo = 11;
int pinLeftBackServo = 9;
Servo rightFront;
Servo leftFront;
Servo rightBack;
Servo leftBack;

int servos = 0;
int throttle = 0;

void setup() {
  Serial.begin(9600);
  rightFront.attach(pinRightFrontServo);
  rightBack.attach(pinRightBackServo);
  leftFront.attach(pinLeftFrontServo);
  leftBack.attach(pinLeftBackServo);

  if (pulseIn(throttleRxPin, HIGH) > 0) {
    manualControl = true;
  }
}

void throttleWrite(int throttle) {
  if (throttle < 0) {
    digitalWrite(pinRightMotorDirection, HIGH);
    digitalWrite(pinLeftMotorDirection, HIGH);
  } else {
    digitalWrite(pinRightMotorDirection, LOW);
    digitalWrite(pinLeftMotorDirection, LOW);
  }
  analogWrite(pinRightMotorSpeed, abs(throttle));
  analogWrite(pinLeftMotorSpeed, abs(throttle));
  
  Serial.println(throttle);
}


void servosWrite(int servos) {
  if (servos < 90) {
    rightFront.write(servos-10);
    rightBack.write(abs(180-(servos-10)));
    leftFront.write(servos+10);
    leftBack.write(abs(180-(servos+10)));
  } else if (servos > 90){
    rightFront.write(servos+10);
    rightBack.write(abs(180-(servos+10)));
    leftFront.write(servos-10);
    leftBack.write(abs(180-(servos-10)));
  } else {
    rightFront.write(servos);
    rightBack.write(servos);
    leftFront.write(servos);
    leftBack.write(servos);
  }
}

int manualThrottle() {
  int throttleVector = 0; //set default to stationary. Negative is reverse, pos is forward -255 to 255
  int throttleVal = pulseIn(throttleRxPin, HIGH) - 900;
  Serial.println(throttleVal);
  if ((throttleVal > 500 && throttleVal < 600) ||  throttleVal < 0) {
    throttleVector = 0;
  } else if (throttleVal < 500) {
    throttleVector = map(throttleVal, 0, 500, -255, 0);
  } else if (throttleVal > 600) {
    throttleVector = map(throttleVal-600, 0, 400, 0, 255);
  }
  return(throttleVector);
}


int manualServos() {
  int servosVector = 0; //set default to stationary. Negative is left, pos is right -255 to 255
  int servosVal = pulseIn(servosRxPin, HIGH) - 900;
  Serial.println(servosVal);
  if ((servosVal > 500 && servosVal < 600) ||  servosVal < 0) {
    servosVector = 90;
  } else if (servosVal < 500) {
    servosVector = map(servosVal, 0, 500, 10, 90);
  } else if (servosVal > 600) {
    servosVector = map(servosVal-600, 0, 400, 90, 170);
  }
  return(servosVector);
}


void loop() {
  if (manualControl) {
    throttle = manualThrottle();
    servos = manualServos();
  }

  throttleWrite(throttle);
  servosWrite(servos);
}
