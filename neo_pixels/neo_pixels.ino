#include <Adafruit_NeoPixel.h>

// Which pin on the Arduino is connected to the NeoPixels?
const int data_pin = 6;
// How many NeoPixels are attached to the Arduino?
const int num_pixels = 5;

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(num_pixels, data_pin, NEO_GRB + NEO_KHZ800);



void setup() {
  pixels.begin(); // This initializes the NeoPixel library.
  pixels.setPixelColor(0, pixels.Color(255,0,0)); // set led#0 in red color
  pixels.setPixelColor(1, pixels.Color(255,127,0)); // set led#1 in yellow color
  pixels.setPixelColor(2, pixels.Color(0,255,0)); // set led#2 in green color
  pixels.setPixelColor(3, pixels.Color(0,0,255)); // set led#1 in blue color
  pixels.setPixelColor(4, pixels.Color(127,0,255)); // set led#4 in violet color
  pixels.show(); // This sends the updated pixel color to the hardware.
}

void loop() {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< pixels.numPixels(); i++) {
      pixels.setPixelColor(i, Wheel(((i * 256 / pixels.numPixels()) + j) & 255));
    }
    pixels.show();
    delay(20);
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return pixels.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return pixels.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return pixels.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}
