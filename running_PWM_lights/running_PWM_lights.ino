const int pins_count = 6;
int pins[pins_count] = {3, 5, 6, 9, 10, 11};
int led[pins_count];
int center = 0;
int direction = 1;
int step_power = 25; 
  
void setup()
{
  Serial.begin(9600);
  Serial.println("==============");
  for (int i=0; i < pins_count; i++){
    pinMode(pins[i], OUTPUT);
  }
}

void loop()
{
  int step = 1;
  led[center] = 255;
  for (int i = center + 1; i < pins_count; i++){
    led[i] = 255 - step * step_power;
    step++;
  }
  step = 1;
  for (int i = center - 1; i >= 0; i--){
    led[i] = 255 - step * step_power;
    step++;
  }

  for (int i=0; i < pins_count; i++){
    Serial.print(led[i]);
    if (i != pins_count-1) {
      Serial.print('-');
    }
  }
  Serial.println();
  
  center += direction; 
  
  if ((direction == 1) && (center >= pins_count)){
    direction = -1;
    center = pins_count - 1;
  } else if ((direction == -1) && (center < 0)){
    direction = 1;
    center = 0;
  }
   
  delay(1500);
}
