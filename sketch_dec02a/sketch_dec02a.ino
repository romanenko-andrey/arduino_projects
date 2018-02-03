const int data_pin = 2;
const int clock_pin = 3;
const int latch_pin = 4;

void setup()
{
  pinMode(data_pin, OUTPUT);
  pinMode(clock_pin, OUTPUT);
  pinMode(latch_pin, OUTPUT);  
}

void loop()
{
  shiftDisplay(B11010001);
}

void shiftDisplay(byte data){
  digitalWrite(latch_pin, LOW);
  shiftOut(data_pin, clock_pin, LSBFIRST, data);
  digitalWrite(latch_pin, HIGH);
}
