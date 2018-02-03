#include "Wire.h"
#include "I2Cdev.h"
#include "MPU6050.h"
#include "Servo.h"
 
MPU6050 mpu;
 
int16_t ax, ay, az;
int16_t gx, gy, gz;
const int led_alarm = 11;
 
Servo myservo;
 
int val;
int prevVal;
int pos = 0;
int avr_size = 10;
int avr_y[10];
int status = 0;
const int sensitive = 12000;
 
void setup() 
{
    pinMode(led_alarm, OUTPUT);
    
    Wire.begin();
    Serial.begin(115200);
 
    Serial.println("Initialize MPU");
    mpu.initialize();
    Serial.println(mpu.testConnection() ? "Connected" : "Connection failed");
    myservo.attach(9);

    for (int i = 0; i < avr_size; i = i + 1) {
       mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
       avr_y[i] = map_y(ay);
    }
}


int alarm(int gx, int gy, int gz){
  if ((abs(gx) > sensitive) || (abs(gy) > sensitive) || (abs(gz) > sensitive)){
    return 1;
  } else {
    return 0;
  }
}

void loop() 
{
    mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    
    Serial.print("a/g:\t");
    Serial.print(ax); Serial.print("\t");
    Serial.print(ay); Serial.print("\t");
    Serial.print(az); Serial.print("\t");
    Serial.print(gx); Serial.print("\t");
    Serial.print(gy); Serial.print("\t");
    Serial.println(gz);
   
    servo_move(ay);

    if (alarm(gx, gy, gz)){
      digitalWrite(led_alarm, HIGH);
      delay(1000);
      digitalWrite(led_alarm, LOW);
    }
 
    delay(50);
}


int map_y(int y){
  return map(y, 17000, -17000, 0, 179);
}

void servo_move(int ay){
   avr_y[pos] = map_y(ay);
    val = 0;
    for (int i = 0; i < avr_size; i = i + 1) {
      val += avr_y[i];
    }
    val /= avr_size;
    
    pos += 1;
    if (pos == avr_size) pos = 0;

    if (val != prevVal)
    {
        myservo.write(val);
        prevVal = val;
    }
}

