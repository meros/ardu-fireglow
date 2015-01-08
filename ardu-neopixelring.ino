#include <Adafruit_NeoPixel.h>

#include <avr/power.h>

#define PIN            0
#define NUMPIXELS      24

#define TURN (PI*2)
#define DELAYVAL (500)

#define BUTTON_PIN 4

uint8_t threeturns[] = {
  128,
  218,
  255,
  218,
  128,
  37,
  0,
  37,
  128,
  218,
  255,
  218,
  128,
  37,
  0,
  37,
  128,
  218,
  255,
  218,
  128,
  37,
  0,
  37};

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int now = 0;

int state = 0;

void renderIdle(int now, Adafruit_NeoPixel& pixels) {
  for (int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, 0, 0, threeturns[(i+now)%NUMPIXELS]/3+10);
  }
} 

void renderTest(int now, Adafruit_NeoPixel& pixels) {
  for (int i = 0; i < pixels.numPixels(); i++) {
    pixels.setPixelColor(i, 255,0,0);
  }
}

void setup() {
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  pinMode(BUTTON_PIN, INPUT);
  pixels.begin(); 
}

void loop() { 
  if (now % 2 > 0) {
    digitalWrite(1, HIGH);
  } 
  else {
    digitalWrite(1, LOW);
  }

  bool idle = true;

  int buttonState =  digitalRead(BUTTON_PIN);
  if (state == 0 && buttonState) {
    state = 1;
    now = 0;
  }   
  
  if (state > 0 && !buttonState) {
    state = 0;
    now = 0;
  }

  switch(state) {
  case 0:  
    renderIdle(now, pixels);
    break;
  case 1:
    renderTest(now, pixels);
    break;
  }

  pixels.show();

  delay(DELAYVAL);
  now++;
}












