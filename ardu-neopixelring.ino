#include <Adafruit_NeoPixel.h>

#include <avr/power.h>

#define PIN            0
#define NUMPIXELS      24

#define TURN (PI*2)

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int delayval = 500;

class Color {
private:
  uint8_t r;
  uint8_t g;
  uint8_t b;
public:
  Color(uint8_t r, uint8_t g, uint8_t b) :
  r(r), g(g), b(b) {
  }

  static Color mix(Color& a, Color& b, uint8_t scale) {
    return Color(
    map(scale, 0, 255, a.r, b.r),
    map(scale, 0, 255, a.g, b.g),
    map(scale, 0, 255, a.b, b.b));
  }

  uint32_t getColor() {
    return Adafruit_NeoPixel::Color(r, g, b);
  }
};

class Animation {
private:
  uint32_t start;
  uint32_t end;

public:
  Animation() :
  start(0),
  end(0) {
  }

  void extend(uint32_t now, uint32_t duration) {
    uint32_t from = now;

    if (start > 0 && end < now) {
      // We are past the end of this an
      from = end;
    }

    start = from;
    end = start + duration;
  }

  void renderFade(uint32_t now, Adafruit_NeoPixel& pixels, Color from, Color to) {    
    uint32_t color = Color::mix(from, to, map(now, start, end, 0, 255)).getColor();
      

    for (int i = 0; i < pixels.numPixels(); i++) {
      pixels.setPixelColor(i, color);
    }
  }

  void renderIdle(int now, Adafruit_NeoPixel& pixels) {
    Color high = Color(0,20,50);
    Color low = Color(20,0,20);
      
    for (int i = 0; i < pixels.numPixels(); i++) {
      float s = sin(((float)(i+now)/(float)pixels.numPixels())*TURN*5)*1000;
      
      uint32_t color = Color::mix(high, low, map(s, -1000, 1000, 0, 255)).getColor();
      
      pixels.setPixelColor(i, color);
    }
  }

  boolean done(uint32_t now) {
    return now > end;
  }  
};

Animation animation;

void setup() {
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);

  pixels.begin(); 
}

int now = 0;

void loop() { 
  if (now % 2 > 0) {
  digitalWrite(1, HIGH);
  } else {
    digitalWrite(1, LOW);
  }
  
  bool idle = true;

  if (idle) {
    if (animation.done(now)) {
      animation.extend(now, 100);    
    }

    animation.renderIdle(now, pixels);
    pixels.show();
  }
  
  delay(100);
  
  now++;
}


