/*
 * IRremote: IRrecvDemo - demonstrates receiving IR codes with IRrecv
 * An IR detector/demodulator must be connected to the input RECV_PIN.
 * Version 0.1 July, 2009
 * Copyright 2009 Ken Shirriff
 * http://arcfn.com
 */

#include <IRremote.h>

const int RECV_PIN = 11;
const int RELAY_PIN = 13;

IRrecv irrecv(RECV_PIN);

decode_results results;

void setup()
{
  Serial.begin(9600);
  // In case the interrupt driver crashes on setup, give a clue
  // to the user what's going on.
  Serial.println("Enabling IRin");
  pinMode(RELAY_PIN, OUTPUT);
  irrecv.enableIRIn(); // Start the receiver
  Serial.println("Enabled IRin");
}

int on = 0;
unsigned long last = millis();

void loop() {
  if (irrecv.decode(&results)) {
    Serial.print(results.value, HEX);
    Serial.print(" (");
    Serial.print(results.bits, DEC);
    Serial.println(" bits)");

    // If it's been at least 1/4 second since the last
    // IR received, toggle the relay
    if (millis() - last > 250) {
      on = !on;
      digitalWrite(RELAY_PIN, on ? HIGH : LOW);
      Serial.println(on);
    }
    last = millis();      
    
    irrecv.resume(); // Receive the next value
  }
  delay(100);
}
