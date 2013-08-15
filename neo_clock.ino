#include <Adafruit_NeoPixel.h>

#define PIN 4

// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_RGB     Pixels are wired for RGB bitstream
//   NEO_GRB     Pixels are wired for GRB bitstream
//   NEO_KHZ400  400 KHz bitstream (e.g. FLORA pixels)
//   NEO_KHZ800  800 KHz bitstream (e.g. High Density LED strip)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(16, PIN, NEO_GRB + NEO_KHZ800);
uint32_t milli_color  = strip.Color(60, 0,  0);
uint32_t second_color = strip.Color( 0, 0, 60);
uint32_t off_color = strip.Color(0, 0, 0);
uint8_t position = 0;
uint8_t pulse = 0;


/* TODO 0.0 to 1.0 percent between current and next value. for color fading */
/* or event based lerping? */

/* TODO gamut values from goggles */


/* CLOCK */
class ClockPositions
{
 public:
  uint8_t milli;
  uint8_t second;
  uint8_t minute;
  uint8_t hour;

  ClockPositions();
  void update();
};


ClockPositions::ClockPositions()
{
  milli = second = minute = hour = 0;
}


void ClockPositions::update()
{
  second = map((millis() % 60000), 0, 60000, 0, 15);
  milli = map((millis() %  1000), 0,  1000, 0, 16);
}



/* CLOCK VIEW */

class ClockSegments
{
 public:
  ClockPositions &positions;
  Adafruit_NeoPixel &strip;
  ClockSegments(Adafruit_NeoPixel &n_strip, ClockPositions &n_positions): strip(n_strip), positions(n_positions) {};
  void draw();
};


void ClockSegments::draw()
{

}



/* SIMPLE MIXER */
// add rgb and clamp





/* APP */
ClockPositions positions;
ClockSegments  segments(strip, positions);

void setup() {
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {
  positions.update();
  milli_looper();
}


void milli_looper()
{

  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, off_color);
  }


  strip.setPixelColor(positions.second, second_color);
  strip.setPixelColor(positions.milli, milli_color);

  strip.show();
}




// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint32_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}

