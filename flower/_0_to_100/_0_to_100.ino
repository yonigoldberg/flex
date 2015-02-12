#include <Adafruit_NeoPixel.h>
#include <avr/power.h>

#define PIN 6
#define RINSIZE 16
#define DEBUG false

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(RINSIZE, PIN, NEO_GRB + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

void setup() {
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
#if defined (__AVR_ATtiny85__)
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif
  // End of trinket special code

  if (DEBUG) Serial.begin(9600); 
  strip.begin();
  strip.setBrightness(20);
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {
  // read the input on analog pin 0:
  int sensorValue = analogRead(A0);  
  if (DEBUG) Serial.println(sensorValue);
  float actualPercent = (float)sensorValue / 4;
  if (DEBUG) Serial.println(actualPercent);
  lightPercent(int(actualPercent + 0.5));
}

void doTheLoop() {  
  for (int i = 0; i < 100; i++){
    lightPercent(i);
    delay(100);
  }
}

void lightPercent(int percent) {
  if (DEBUG) Serial.println("New percent: " + String(percent));
  int litColor = strip.Color(127, 127, 127);
  int offColor = strip.Color(0, 0, 0);
  
  //figure out till what number to light the strip
  float actualPercent = (float)percent / 100;

  //made it 1 indexed (so 0 pixel is off completely)
  int lastLitPin = ((float)strip.numPixels() * actualPercent) +  0.5;
  if (DEBUG) Serial.println("New lastLitPin is :" + String(lastLitPin));
  for (int i=0; i < strip.numPixels(); i++) {
    if (i <= lastLitPin - 1 ) {
      strip.setPixelColor(i, litColor);
      if (DEBUG) Serial.println("Set pixel " + String(i) + " to on");
    } else {
      strip.setPixelColor(i, offColor);
      if (DEBUG) Serial.println("Set pixel " + String(i) + " to off");
    }
  }
  if (DEBUG) {
    for (int j = 0; j < strip.numPixels(); j++) {
    Serial.println("The color for pixel #" + String(j) + " is " + String(strip.getPixelColor(j)));
    }
  }
  strip.show();
}

