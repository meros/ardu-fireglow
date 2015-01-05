#include <Adafruit_NeoPixel.h>

#define PIN            0
#define NUMPIXELS      24

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int delayval = 500;

void setup() {
  pixels.begin(); 
}

void loop() {
  for(int i=0;i<NUMPIXELS;i++){
    pixels.setPixelColor(i, 
      pixels.Color(
        map(i, 0, NUMPIXELS, 0, 255),
        map(i, 0, NUMPIXELS, 255, 0),
        0));
    pixels.show(); 
    delay(delayval); 
  }
}
