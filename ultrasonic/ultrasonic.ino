const int trig_pin = 4;    //PRIG pin
const int pw_pin = 7;      //ECHO or PW pin
const int trig_delay = 25; //microsec

const int speaker_pin = 11;   //buzer

void setup() {
  pinMode(speaker_pin, OUTPUT);
  Serial.begin(9600);
}

void loop() {

   float dist = measureDistance(); //in cm

   float freq = 14.003 * dist - 15.01;
   tone(speaker_pin, freq);
   
   Serial.print(dist);
   Serial.println(" cm");

   delay(100);
}


float measureDistance(){
   long duration;
   //отправим одиночный импульс на ножку TRIG
   pinMode(trig_pin, OUTPUT);
   digitalWrite(trig_pin, LOW);
   delayMicroseconds(10);
   digitalWrite(trig_pin, HIGH);
   delayMicroseconds(trig_delay);
   digitalWrite(trig_pin, LOW);

   //измерим время ответа на импульс
   pinMode(pw_pin, INPUT);
   duration = pulseIn(pw_pin, HIGH);

   //convert to distance
   return  duration / 58.8;
}
