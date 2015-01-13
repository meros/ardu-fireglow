#include <Adafruit_NeoPixel.h>

#include <avr/power.h>

#define PIN            0
#define NUMPIXELS      24

#define TURN (PI*2)
#define DELAYVAL (10)

#define BUTTON_PIN 4

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

void renderIdle(uint32_t now, Adafruit_NeoPixel& pixels) {
  for (int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, 0, 0, sinval(map(i, 0, 24, 0, 24*3)+now));
  }
} 

void renderRainbow(uint32_t now, Adafruit_NeoPixel& pixels) {
  uint8_t color = (now*5)/256 % 3;
  uint8_t f = (now*5) % 256;
  uint8_t fi = 255 - f;

  for (int i = 0; i < NUMPIXELS; i++) {
    switch(color) {
    case 0:
      pixels.setPixelColor(i, fi, f, 0);
      break;
    case 1:
      pixels.setPixelColor(i, 0, fi, f);
      break;
    case 2:
      pixels.setPixelColor(i, f, 0, fi);
      break;    
    }
  }
}

void renderWhite(uint32_t now, Adafruit_NeoPixel& pixels) {
  uint8_t part = (now*5)/256 % 5;
  uint8_t f = (now*5) % 256;
  uint8_t fi = 255 - f;

  for (int i = 0; i < NUMPIXELS; i++) {
    switch(part) {
    case 0: 
      {
        uint8_t brightness = map(sinval(i), 0, 255, 0, f);
        pixels.setPixelColor(i, brightness, brightness, brightness);
        break;
      }
    case 1: 
      {
        uint8_t brightness = sinval(i);

        if (brightness > 256/2) {
          brightness = min(255, brightness + f);
        }

        if (brightness < 256/2) {
          brightness = max(0, brightness - f);
        }

        pixels.setPixelColor(i, brightness, brightness, brightness);
        break;
      }
    case 2: 
      {
        uint8_t brightness = sinval(i + map(f, 0, 255, 0, 12));

        if (brightness > 256/2) {
          brightness = 255;
        }

        if (brightness < 256/2) {
          brightness = 0;
        }


        pixels.setPixelColor(i, brightness, brightness, brightness);
        break;    
      }
    case 3:
      {
        uint8_t brightness = sinval(i+12);

        if (brightness > 256/2) {
          brightness = min(255, brightness + fi);
        }

        if (brightness < 256/2) {
          brightness = max(0, brightness - fi);
        }

        pixels.setPixelColor(i, brightness, brightness, brightness);
        break;
      }   
    case 4: 
      {
        uint8_t brightness = map(sinval(i+12), 0, 255, 0, fi);
        pixels.setPixelColor(i, brightness, brightness, brightness);
        break;
      }
    }
  }
}

void renderChecker(uint32_t now, Adafruit_NeoPixel& pixels) {
  for (int i = 0; i < NUMPIXELS; i++) {
    if ((i+(now/50)) % 2 ) {
      pixels.setPixelColor(i, 255, 0, 0);

    } 
    else {
      pixels.setPixelColor(i, 0, 255, 0);
    }
  }
}

int oldButtonState = 0;

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
  if (!oldButtonState && buttonState) {
    state = random(0,4);
    now = 0;
  }   

  oldButtonState = buttonState;

  switch(state) {
  case 0:  
    renderIdle(now, pixels);
    break;
  case 1:
    renderWhite(now, pixels);
    break;
  case 2:
    renderRainbow(now, pixels);
    break;
  case 3:
    renderChecker(now, pixels);
    break;

  }

  pixels.show();

  delay(DELAYVAL);
  now++;
}






















