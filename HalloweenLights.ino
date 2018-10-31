// #define FASTLED_ESP8266_NODEMCU_PIN_ORDER 
#define FASTLED_ESP8266_RAW_PIN_ORDER 

#include <FastLED.h>

FASTLED_USING_NAMESPACE

// FastLED "100-lines-of-code" demo reel, showing just a few 
// of the kinds of animation patterns you can quickly and easily 
// compose using FastLED.  
//
// This example also shows one easy way to define multiple 
// animations patterns and have them automatically rotate.
//
// -Mark Kriegsman, December 2014



#if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001000)
#warning "Requires FastLED 3.1 or later; check github for latest code."
#endif

#define DATA_PIN    13
#define LED_TYPE    WS2811
#define COLOR_ORDER RGB
#define NUM_LEDS    50
CRGB leds[NUM_LEDS];

#define BRIGHTNESS         230
#define FRAMES_PER_SECOND  120




DEFINE_GRADIENT_PALETTE( LowellFall_No3_gp ) {
    0,   173,  53,   1,
   43,     0,   0,   0,
   73,    37,   1,  80,
  102,    13,   1,  31,
  137,     0,   0,   0,
  183,    13,  60,   6,
  232,     0,   0,   0,
  255,   173,  53,   1
};


// # http://angrytools.com/gradient/?0_c87d02,20_000000,38_7A16A1,48_510D6E,61_000000,72_36631E,90_000000,100_c87d02&0_100,100_100&l_321
DEFINE_GRADIENT_PALETTE( LowellFall_No5_gp ) {
   0,    135,  53,   1,
   51,     0,   0,   0,
   96,    37,   1,  80,
  122,    12,   1,  31,
  155,     0,   0,   0,
  183,     4,  31,   1,
  229,     0,   0,   0,
  255,   135,  53,   1
};


// http://angrytools.com/gradient/?0_c87d02,20_000000,22_7A16A1,40_510D6E,54_052455,58_000000,100_c87d02,88_69853D,92_000000,34_7a16a1,34_7a16a1,14_B57002,19_000000,82_3A6E2D,65_438034,95_c87d02&0_100,100_100&l_321
DEFINE_GRADIENT_PALETTE( LowellFall_No6_gp ) {
    0,   135,  53,   1,
   35,   104,  41,   1,
   48,     0,   0,   0,
   51,     0,   0,   0,
   56,    37,   1,  80,
   86,    37,   1,  80,
  102,    12,   1,  31,
  137,     1,   3,  16,
  147,     0,   0,   0,
  165,     7,  55,   4,
  209,     5,  40,   3,
  224,    25,  60,   7,
  234,     0,   0,   0,
  242,   135,  53,   1,
  255,   135,  53,   1
};


DEFINE_GRADIENT_PALETTE( Leaf1_gp ){
    0,    12, 255,   1,
   48,   255, 255,   0,
   94,   188, 162,   8,
  181,   255,   0,   0,
  244,   222,  70,   1,
  255,    58,  22,   1
};

// More inspiration https://i.redd.it/6a0slunyg6xy.jpg


CRGBPalette16 palette = LowellFall_No6_gp;
// CRGBPalette16 palette = Leaf1_gp;



void setup() {

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(2000); // 2 second delay for recovery
  
  digitalWrite(2, LOW);
  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection( Typical8mmPixel );
  //FastLED.addLeds<LED_TYPE,DATA_PIN,CLK_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

  FastLED.setTemperature( OvercastSky );
 
  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);
}


// List of patterns to cycle through.  Each is defined as a separate function below.
typedef void (*SimplePatternList[])();
// SimplePatternList gPatterns = { rainbow, rainbowWithGlitter, /*sinelon, */juggle /*, bpm */, jungleWithGlitter};

SimplePatternList gPatterns = { rainbow, juggle};


uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t gHue = 0; // rotating "base color" used by many of the patterns

uint8_t last = 0;

void loop()
{
  // Call the current pattern function once, updating the 'leds' array
  gPatterns[gCurrentPatternNumber]();

  // send the 'leds' array out to the actual LED strip
  FastLED.show();  
  // insert a delay to keep the framerate modest
  FastLED.delay(1000/FRAMES_PER_SECOND); 

  // do some periodic updates
  EVERY_N_MILLISECONDS( 25 ) { gHue++; } // slowly cycle the "base color" through the rainbow
  EVERY_N_SECONDS( 30 ) { nextPattern(); } // change patterns periodically

  EVERY_N_SECONDS( 5) { if(last==0) { digitalWrite(LED_BUILTIN, HIGH); last=1; } else { digitalWrite(LED_BUILTIN, LOW); last=0; } }
  
}

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

void nextPattern()
{
  // add one to the current pattern number, and wrap around at the end
  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE( gPatterns);
}

void rainbow() 
{
  // FastLED's built-in rainbow generator
  //fill_rainbow( leds, NUM_LEDS, gHue, 7);
  //CRGBPalette16 palette = LowellFall_No3_gp;
  
    // fill_palette(leds, NUM_LEDS, gHue, 7, palette, 255, LINEARBLEND);
    fill_palette(leds, NUM_LEDS, gHue, 3, palette, 255, LINEARBLEND);
  
}

void rainbowWithGlitter() 
{
  // built-in FastLED rainbow, plus some random sparkly glitter
  rainbow();
  addGlitter(30);
  // addGlitter(80);
}

void addGlitter( fract8 chanceOfGlitter) 
{
  if( random8() < chanceOfGlitter) {
    leds[ random16(NUM_LEDS) ] += CRGB::White;
  }
}

void confetti() 
{
  
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  //leds[pos] += CHSV( gHue + random8(64), 200, 255);

    leds[pos] = ColorFromPalette(palette, gHue+random8(64));
}

void sinelon()
{    
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, NUM_LEDS, 20);
  int pos = beatsin16( 13, 0, NUM_LEDS-1 );
  // leds[pos] += CHSV( gHue, 255, 192);
  leds[pos] = ColorFromPalette(palette, gHue);
}

void bpm()
{
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62;
  //CRGBPalette16 palette = LowellFall_No3_gp;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for( int i = 0; i < NUM_LEDS; i++) { //9948
    leds[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
  }
}

void juggle() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, NUM_LEDS, 20);
  byte dots = 10;
  byte dothue = 0;
  for( int i = 0; i < 8; i++) {
    leds[beatsin16( i+(dots-1), 0, NUM_LEDS-1 )] |= ColorFromPalette(palette, dothue, 255);

    dothue += (4*dots);
  }
}

void jungleWithGlitter(){
  juggle();
  addGlitter(54);

}
