#include <Adafruit_NeoPixel.h>

#include <FastLED.h>
#include <avr/power.h>

#define PIN            0
#define NUMPIXELS      24

#define TURN (PI*2)
#define DELAYVAL (10)

#define BUTTON_PIN 4

typedef struct SRGB {
  uint8_t r;
  uint8_t g;
  uint8_t b;
} SRGB;

const uint8_t sinval_data[] = {
  128,
  160,
  191,
  218,
  238,
  251,
  255,
  251,
  238,
  218,
  191,
  160,
  128,
  95,
  64,
  37,
  17,
  4,
  0,
  4,
  17,
  37,
  64,
  95};

uint8_t sinval(uint8_t i) {
  return sinval_data[i%NUMPIXELS];
}

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

uint32_t now = 0;

int state = 0;



void setRgb(Adafruit_NeoPixel& pixels, int i, SRGB& rgb) {
    pixels.setPixelColor(i, rgb.r, rgb.g, rgb.b);
}


void blendRGB(SRGB& destRGB, int i, int from, int to, SRGB& fromRGB, SRGB& toRGB) {
  destRGB.r = map(i, from, to, fromRGB.r, toRGB.r);
  destRGB.g = map(i, from, to, fromRGB.g, toRGB.g);
  destRGB.b = map(i, from, to, fromRGB.b, toRGB.b);
}


void renderFire(uint32_t now, Adafruit_NeoPixel& pixels) {
  SRGB red;
  red.r = 70;
  red.g = 30;
  red.b = 0;

  SRGB yellow;
  yellow.r = 30;
  yellow.g = 10;
  yellow.b = 0;

  SRGB black;
  black.r = 0;
  black.g = 0;
  black.b = 0;

  SRGB dest;
  for (int i = 0; i < NUMPIXELS; i++) {
      blendRGB(dest, constrain(inoise8((now)*10, i*100), 50, 200), 50, 200, red, yellow);
      blendRGB(dest, constrain(inoise8(now*50), 100, 170), 100, 255, dest, black);
    setRgb(pixels, i, dest);
  }
} 

int oldButtonState = 0;

void setup() {
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  pinMode(BUTTON_PIN, INPUT);
  pixels.begin(); 
}

void loop() { 
  bool idle = true;

  int buttonState =  digitalRead(BUTTON_PIN);
  if (!oldButtonState && buttonState) {
    state = random(0,0);
    now = 0;
  }   

  oldButtonState = buttonState;

  switch(state) {
  case 0:  
    renderFire(now, pixels);
    break;
  }

  pixels.show();

  delay(DELAYVAL);
  now++;
}






















