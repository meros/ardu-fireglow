#include <Adafruit_NeoPixel.h>

#define PIN            0
#define NUMPIXELS      24

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int delayval = 500;

class Color {
private:
  uint8_t r;
  uint8_t g;
  uint8_t b;
public:
  Color(uint8_t r, uint8_t g, uint8_t b) :r(r), g(g), b(b) {}
  
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

class Fade {
private:
  int startMillis;
  int durationMillis;
  
  Color from;
  Color to;
  
public:
  Fade(int startMillis, int durationMillis, Color from, Color to) :
  startMillis(startMillis),
  durationMillis(durationMillis),
  from(from),
  to(to) {}
  
  void render(int nowMillis, Adafruit_NeoPixel& pixels) {    
    uint32_t color = Color::mix(from, to, map(nowMillis, startMillis, startMillis+durationMillis, 0, 255)).getColor();
    
    for (int i = 0; i < pixels.numPixels(); i++) {
      pixels.setPixelColor(i, color);
    }
  }
  
  boolean done(int nowMillis) {
    return nowMillis > startMillis + durationMillis;
  }
};

Fade* currentFade = NULL;

void setup() {
  pixels.begin(); 
}

void loop() {
  int now = millis();
  
  if (currentFade == NULL || currentFade->done(now)) {
    delete(currentFade);
    currentFade = new Fade(millis(), 1000, Color(0,0,255), Color(255,0,0));
  }
  
  currentFade->render(now, pixels);
  pixels.show();
}
