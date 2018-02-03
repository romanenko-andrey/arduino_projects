const int btn_pin = 4;
const int debounce_delay = 50; //ms

int btn_prev = HIGH;
int counter = 0;
int btn_state = HIGH;
unsigned long last_debounce_time = millis();
 

void setup()
{
  pinMode(btn_pin, INPUT_PULLUP);
  Serial.begin(9600);
  last_debounce_time = millis();
}

void loop()
{
  int btn_read = digitalRead(btn_pin);

  if (btn_read != btn_prev){
    last_debounce_time = millis();
  }
  Serial.println(last_debounce_time);
  if ( millis() > (last_debounce_time + debounce_delay) ){ 
    if ( btn_read != btn_state )  {
      btn_state = btn_read;
      if (btn_state == LOW){
        counter++;
        Serial.println(counter);
      }  
    }
  }  
  btn_prev = btn_read;  
   
}  
