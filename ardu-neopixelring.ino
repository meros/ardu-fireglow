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
  
  int getEndMillis() {
    return startMillis + durationMillis;
  }
};

struct Fade *redToGreen(int fromMillis) {
  return new Fade(fromMillis, 1000, Color(0,0,255), Color(255,0,0));
}

struct Fade *greenToRed(int fromMillis) {
  return new Fade(fromMillis, 500, Color(255,0,0), Color(0,0,255));
}

typedef Fade* (*FadeFactory)(int fromMillis);

class Loop {
private:
  FadeFactory *factories;
  int numFactories;
  
  int state;
  
  Fade* current;
  
public:
  Loop(FadeFactory factories[], int numFactories) : 
    factories(factories),
    numFactories(numFactories),
    state(0),
    current(NULL) {
  }
  
  ~Loop() {
    delete[] factories;
  }
  
  void render(int nowMillis, Adafruit_NeoPixel& pixels) {    
    int newStartMillis = nowMillis;
    if (current && current->done(nowMillis)) {
      newStartMillis = current->getEndMillis();
    }
    
    if (!current || current->done(nowMillis)) {
      delete(current);
      state = (state + 1) % numFactories;
      current = factories[state](newStartMillis);
    }
    
    current->render(nowMillis, pixels);
  }
};

Loop *looper;

void setup() {
  pixels.begin(); 
  
  FadeFactory *factories = new FadeFactory[2];
  factories[0] = redToGreen;
  factories[1] = greenToRed;
  
  looper = new Loop(factories, 2);
}

void loop() {
  int now = millis();

  looper->render(now, pixels);
  pixels.show();
}
