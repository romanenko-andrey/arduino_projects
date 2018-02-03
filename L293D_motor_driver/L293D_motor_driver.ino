const int pwm_M1_pin = 3;
const int pwm_M2_pin = 5;

const int dir_1a_pin = 2;
const int dir_2a_pin = 4;

const int dir_3a_pin = 7;
const int dir_4a_pin = 8;

const int led_left_pin = 12;
const int led_right_pin = 13;

const int MAX_SPEED = 255;
const int HALF_SPEED = 127;

void setup() {
  pinMode(pwm_M1_pin, OUTPUT);
  pinMode(dir_1a_pin, OUTPUT);
  pinMode(dir_2a_pin, OUTPUT);
  pinMode(pwm_M2_pin, OUTPUT);
  pinMode(dir_3a_pin, OUTPUT);
  pinMode(dir_4a_pin, OUTPUT);
  pinMode(led_left_pin, OUTPUT);  
  pinMode(led_right_pin, OUTPUT);
  
}

void forward(int speed){
  digitalWrite(led_left_pin, HIGH);
  digitalWrite(led_right_pin, HIGH);
  digitalWrite(dir_1a_pin, HIGH);
  digitalWrite(dir_2a_pin, LOW);
  digitalWrite(dir_3a_pin, HIGH);
  digitalWrite(dir_4a_pin, LOW);
  analogWrite(pwm_M1_pin, speed);
  analogWrite(pwm_M2_pin, speed);
}

void backward(int speed){
  digitalWrite(led_left_pin, LOW);
  digitalWrite(led_right_pin, LOW);
  
  digitalWrite(dir_1a_pin, LOW);
  digitalWrite(dir_2a_pin, HIGH);
  digitalWrite(dir_3a_pin, LOW);
  digitalWrite(dir_4a_pin, HIGH);
  analogWrite(pwm_M1_pin, speed);
  analogWrite(pwm_M2_pin, speed);
}

void stopMotor(){
  digitalWrite(led_left_pin, LOW);
  digitalWrite(led_right_pin, LOW);
  digitalWrite(dir_1a_pin, LOW);
  digitalWrite(dir_2a_pin, LOW);
  digitalWrite(dir_3a_pin, LOW);
  digitalWrite(dir_4a_pin, LOW);
  analogWrite(pwm_M1_pin, 0);
  analogWrite(pwm_M2_pin, 0);
}

void turnLeft(int speed){
  digitalWrite(led_left_pin, HIGH);
  digitalWrite(led_right_pin, LOW);
  digitalWrite(dir_1a_pin, HIGH);
  digitalWrite(dir_2a_pin, LOW);
  digitalWrite(dir_3a_pin, LOW);
  digitalWrite(dir_4a_pin, LOW);
  analogWrite(pwm_M1_pin, speed);
  analogWrite(pwm_M2_pin, 0);
}

void turnRight(int speed){
  digitalWrite(led_right_pin, HIGH);
  digitalWrite(led_left_pin, LOW);
  digitalWrite(dir_1a_pin, LOW);
  digitalWrite(dir_2a_pin, LOW);
  digitalWrite(dir_3a_pin, HIGH);
  digitalWrite(dir_4a_pin, LOW);
  analogWrite(pwm_M1_pin, 0);
  analogWrite(pwm_M2_pin, speed);
}

void loop() {
  forward(MAX_SPEED);
  delay(1000);
  turnLeft(HALF_SPEED);
  delay(1000);
  turnRight(HALF_SPEED); 
  delay(1000);
  backward(MAX_SPEED);
  delay(1000);
  stopMotor();
  delay(3000);
}
