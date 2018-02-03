const int led_pin = 3;
const int ain_pin = A0;

void setup() {
  pinMode(led_pin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  int val = analogRead(ain_pin);
  int brightness = map(val, 0, 1023, 0, 255);

  Serial.println(brightness);
  analogWrite(led_pin, brightness);
  delay(100);
}
