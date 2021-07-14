// Arduino Nano Every
#include "FastLED.h"

#define NUM_LEDS 29
#define DATA_PIN 2
//or 6 for nano every

#define COLOR_ORDER RGB

// Adalight sends a "Magic Word" (defined in /etc/boblight.conf) before sending the pixel data
uint8_t prefix[] = {'A', 'd', 'a'}, hi, lo, chk, i;
 
// Baudrate, higher rate allows faster refresh rate and more LEDs (defined in /etc/boblight.conf)
#define serialRate 115200
 
// Define the array of leds
CRGB leds[NUM_LEDS];
 
void setup() {
        FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS).setCorrection(0xBCFF79);
     
        // initial RGB flash
        //LEDS.showColor(CRGB(188, 255, 202));//GRB
        LEDS.showColor(CRGB(255, 255, 255));//GRB
        //LEDS.showColor(CRGB(200, 255, 180));//GRB
        Serial.begin(serialRate);
        Serial.print("Ada\n"); // Send "Magic Word" string to host
}
 
void loop() {
  // wait for first byte of Magic Word
  for(i = 0; i < sizeof prefix; ++i) {
    waitLoop: while (!Serial.available()) ;;
    // Check next byte in Magic Word
    if(prefix[i] == Serial.read()) continue;
    // otherwise, start over
    i = 0;
    goto waitLoop;
  }
 
  // Hi, Lo, Checksum
 
  while (!Serial.available()) ;;
  hi=Serial.read();
  while (!Serial.available()) ;;
  lo=Serial.read();
  while (!Serial.available()) ;;
  chk=Serial.read();
 
  // if checksum does not match go back to wait
  if (chk != (hi ^ lo ^ 0x55))
  {
    i=0;
    goto waitLoop;
  }
 
  memset(leds, 0, NUM_LEDS * sizeof(struct CRGB));
  // read the transmission data and set LED values
  for (uint16_t i = 0; i < NUM_LEDS; i++) {
    byte r, g, b;    
    while(!Serial.available());
    r = Serial.read();
    while(!Serial.available());
    g = Serial.read();
    while(!Serial.available());
    b = Serial.read();
    leds[i].r = r;
    leds[i].g = g;
    leds[i].b = b;
  }
  // shows new values
 FastLED.show();
}
