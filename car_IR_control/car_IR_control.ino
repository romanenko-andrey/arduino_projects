#include <IRremote.h>

const unsigned long FORWARD_COMMAND    = 0xFF00FF;//0xFF18E7; //2
const unsigned long BACKWARD_COMMAND   = 0xFF807F;//0xFF4AB5; //8
const unsigned long LEFT_COMMAND       = 0xFFC03F;//0xFF10EF; //4
const unsigned long RIGHT_COMMAND      = 0xFF40BF;//0xFF5AA5; //6
const unsigned long FORWARD_RIGHT_COMMAND  = 0xFF02FD;//0xFF7A85; //3
const unsigned long FORWARD_LEFT_COMMAND   = 0xFF38C7;//0xFF30CF; //1
const unsigned long BACKWARD_RIGHT_COMMAND = 0xFFF00F;//0xFF52AD; //9
const unsigned long BACKWARD_LEFT_COMMAND  = 0xFFD827;//0xFF42BD; //7
const unsigned long MINUS_SPEED_COMMAND = 0xFF12ED;//0xFFE01F; //-
const unsigned long PLUS_SPEED_COMMAND  = 0xFF1AE5;//0xFFA857; //+
const unsigned long AUTO_REG_COMMAND = 0xFFF807;//oK
const unsigned long CONTINUE_COMMAND    = 0xFFFFFFFF;
const unsigned long STOP_COMMAND        = 0x000000;

const int  RUN  = 1;
const int  STOP = 0;
const int  AUTO = 1;
const int  MANUAL = 0;

const int RECV_PIN  = 3;
const int RELAY_PIN = 13;

const int pwm_M1_pin = 5;
const int pwm_M2_pin = 10;

const int dir_1a_pin = 2;
const int dir_2a_pin = 4;

const int dir_3a_pin = 7;
const int dir_4a_pin = 8;

const int led_left_pin = 12;
const int led_right_pin = 13;

const int MAX_SPEED = 255;
const int MIN_SPEED = 100;
const int DELTA_SPEED = 50;

const int ULTRASONIC_TRIG_PIN = 9;    //PRIG pin
const int ULTRASONIC_ECHO_PIN = 11;    //ECHO or PW pin
const int ULTRASONIC_TRIG_DELAY = 25; //microsec
const int MINIMUM_DISTANCE = 150; //минимально допустимое расстояние до препятствия
const int BACKWARD_TIME = 1000;
const int ROTATION_90_TIME = 500;


IRrecv irrecv(RECV_PIN);
decode_results results;
int car_speed = 255;

void setup()
{
  Serial.begin(9600);
  Serial.println("Enabling IRin");
  pinMode(RELAY_PIN, OUTPUT);
  irrecv.enableIRIn(); // Start the receiver
  Serial.println("Enabled IRin");

  pinMode(pwm_M1_pin, OUTPUT);
  pinMode(dir_1a_pin, OUTPUT);
  pinMode(dir_2a_pin, OUTPUT);
  pinMode(pwm_M2_pin, OUTPUT);
  pinMode(dir_3a_pin, OUTPUT);
  pinMode(dir_4a_pin, OUTPUT);
  pinMode(led_left_pin, OUTPUT);  
  pinMode(led_right_pin, OUTPUT);
}

int on = STOP;
int reg = MANUAL;
unsigned long last = millis();
unsigned long last_command = STOP_COMMAND;
float dist = 1000;

void loop() {
  if (irrecv.decode(&results)) {
    Serial.println(results.value, HEX);
    analyse_command(); 
    last = millis(); // save last signal time     
    irrecv.resume(); // Receive the next value
  }
 if (millis() - last > 120) {
    // no signal at least 120 millisecond since the last
    // IR received - stop mooving
    on = STOP;
  }

  if (reg == MANUAL) {
    manual_running();
  } else {
    if (dist < MINIMUM_DISTANCE){     
      Serial.print(dist, DEC);
      Serial.println(' mm - Препятствие! Объезжаем.');
      backward(car_speed);
      delay(BACKWARD_TIME);
      turnLeft(car_speed);
      delay(ROTATION_90_TIME);  
    }
    forward(car_speed);
   };
  delay(50);
  dist = measureDistance(); 

} //loop

void analyse_command(){
  switch (results.value) {
    case AUTO_REG_COMMAND:
      reg = reg == AUTO ? MANUAL : AUTO;
    case FORWARD_COMMAND:
    case BACKWARD_COMMAND:
    case LEFT_COMMAND:
    case RIGHT_COMMAND:
    case MINUS_SPEED_COMMAND:
    case PLUS_SPEED_COMMAND:
    case FORWARD_RIGHT_COMMAND:
    case FORWARD_LEFT_COMMAND:
    case BACKWARD_RIGHT_COMMAND:
    case BACKWARD_LEFT_COMMAND:
       last_command = results.value;
    case CONTINUE_COMMAND:
      on = RUN;
      break;
    default:
      on = STOP;
  }
} //analyse_command function

void manual_running(){
  if (on == RUN) {
    switch (last_command) {
      case FORWARD_COMMAND:
        Serial.println(" Идем вперед");
        forward(car_speed);
        break;
      case BACKWARD_COMMAND:
        Serial.println(" Идем назад");
        backward(car_speed);
        break;
      case LEFT_COMMAND:
        Serial.println(" Поворот налево");
        turnLeft(car_speed);
        break;
      case RIGHT_COMMAND:
        Serial.println(" Поворот направо");
        turnRight(car_speed);
        break;
      case FORWARD_RIGHT_COMMAND:
        Serial.println(" Прямо и направо");
        turnForwardRight(car_speed);
        break;      
      case FORWARD_LEFT_COMMAND:
        Serial.println(" Прямо и налево направо");
        turnForwardLeft(car_speed);
        break; 
      case BACKWARD_RIGHT_COMMAND:
        Serial.println(" Назад и направо");
        turnBackwardRight(car_speed);
        break; 
      case BACKWARD_LEFT_COMMAND:
        Serial.println(" Назад и налево");
        turnBackwardLeft(car_speed);
        break; 
      default:
        Serial.println(" Не знаю что делать ....");
    } //switch
  } else {
    switch (last_command) {
      case STOP_COMMAND:
        break;
      case MINUS_SPEED_COMMAND:
        if (car_speed - DELTA_SPEED >= MIN_SPEED){
          car_speed -= DELTA_SPEED;
        }
        Serial.print("Уменьшаем скорость до ");
        Serial.println(car_speed);
        break;
      case PLUS_SPEED_COMMAND:
        if (car_speed + DELTA_SPEED < MAX_SPEED){
          car_speed += DELTA_SPEED;
        } else {
          car_speed = MAX_SPEED;
        }
        Serial.print("Увеличиваем скорость до ");
        Serial.println(car_speed);
        break;
      default: 
        stopMotor();
        Serial.println(" СТОП машина!");
    } //switch
    last_command = STOP_COMMAND;
  } // if
} //running function


float measureDistance(){
   long duration;
   //отправим одиночный импульс на ножку TRIG
   pinMode(ULTRASONIC_TRIG_PIN, OUTPUT);
   digitalWrite(ULTRASONIC_TRIG_PIN, LOW);
   delayMicroseconds(10);
   digitalWrite(ULTRASONIC_TRIG_PIN, HIGH);
   delayMicroseconds(ULTRASONIC_TRIG_DELAY);
   digitalWrite(ULTRASONIC_TRIG_PIN, LOW);

   //измерим время ответа на импульс
   pinMode(ULTRASONIC_ECHO_PIN, INPUT);
   duration = pulseIn(ULTRASONIC_ECHO_PIN, HIGH);

   //convert to distance in mm
   return  duration / 5.88;
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
  digitalWrite(dir_4a_pin, HIGH);
  analogWrite(pwm_M1_pin, speed);
  analogWrite(pwm_M2_pin, speed);
}

void turnRight(int speed){
  digitalWrite(led_right_pin, HIGH);
  digitalWrite(led_left_pin, LOW);
  digitalWrite(dir_1a_pin, LOW);
  digitalWrite(dir_2a_pin, HIGH);
  digitalWrite(dir_3a_pin, HIGH);
  digitalWrite(dir_4a_pin, LOW);
  analogWrite(pwm_M1_pin, speed);
  analogWrite(pwm_M2_pin, speed);
}

void turnForwardRight(unsigned int speed){
  digitalWrite(led_right_pin, HIGH);
  digitalWrite(led_left_pin, LOW);
  digitalWrite(dir_1a_pin, HIGH);
  digitalWrite(dir_2a_pin, LOW);
  digitalWrite(dir_3a_pin, HIGH);
  digitalWrite(dir_4a_pin, LOW);
  analogWrite(pwm_M1_pin, speed >> 1);
  analogWrite(pwm_M2_pin, speed);
}
void turnForwardLeft(unsigned int speed){
  digitalWrite(led_right_pin, LOW);
  digitalWrite(led_left_pin, HIGH);
  digitalWrite(dir_1a_pin, HIGH);
  digitalWrite(dir_2a_pin, LOW);
  digitalWrite(dir_3a_pin, HIGH);
  digitalWrite(dir_4a_pin, LOW);
  analogWrite(pwm_M1_pin, speed);
  analogWrite(pwm_M2_pin, speed >> 1);
}
void turnBackwardRight(unsigned int speed){
  digitalWrite(led_right_pin, HIGH);
  digitalWrite(led_left_pin, LOW);
  digitalWrite(dir_1a_pin, LOW);
  digitalWrite(dir_2a_pin, HIGH);
  digitalWrite(dir_3a_pin, LOW);
  digitalWrite(dir_4a_pin, HIGH);
  analogWrite(pwm_M1_pin, speed >> 1);
  analogWrite(pwm_M2_pin, speed);
}
void turnBackwardLeft(unsigned int speed){
  digitalWrite(led_right_pin, LOW);
  digitalWrite(led_left_pin, HIGH);
  digitalWrite(dir_1a_pin, LOW);
  digitalWrite(dir_2a_pin, HIGH);
  digitalWrite(dir_3a_pin, LOW);
  digitalWrite(dir_4a_pin, HIGH);
  analogWrite(pwm_M1_pin, speed);
  analogWrite(pwm_M2_pin, speed >> 1);
}

