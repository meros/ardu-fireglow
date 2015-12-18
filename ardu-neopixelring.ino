#include <FastLED.h>
#include <avr/power.h>

#define PIN            0
#define NUMPIXELS      24

#define TURN (PI*2)
#define DELAYVAL (10)

uint32_t now = 0;
CRGB leds[NUMPIXELS];

void renderFire(uint32_t now, CRGB* leds, int numpixels) {
  CRGB yellow(70, 40, 0);
  CRGB red(20, 0, 0);

  for (int i = 0; i < numpixels; i++) {
    CRGB dest = blend(red, yellow, inoise8((now) * 20, i * 500));
    leds[i] = dest;
  }

  fadeToBlackBy(leds, numpixels, map(inoise8(now * 50), 0, 255, 0, 50));
}

void setup() {
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  FastLED.addLeds<NEOPIXEL, PIN>(leds, NUMPIXELS);
}

void loop() {
  renderFire(now, leds, NUMPIXELS);

  FastLED.show();
  delay(DELAYVAL);
  now++;
}






















