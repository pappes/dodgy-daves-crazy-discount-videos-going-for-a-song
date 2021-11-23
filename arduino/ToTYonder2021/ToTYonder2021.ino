#include <FastLED.h>

#define CLOCK_PIN     11
#define LED_PIN     10
#define NUM_LEDS    78
#define BRIGHTNESS  64
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

#define UPDATES_PER_SECOND 100

#define SECONDS_PER_MILLI 1000L
#define MINUTES (SECONDS_PER_MILLI*60L)
#define CYCLE_1_TIME (10 * SECONDS_PER_MILLI) / SECONDS_PER_MILLI
#define CYCLE_2_TIME CYCLE_1_TIME + (60L * SECONDS_PER_MILLI) / SECONDS_PER_MILLI
#define CYCLE_3_TIME CYCLE_2_TIME + (3L * MINUTES)
#define CYCLE_4_TIME CYCLE_3_TIME + (10L * MINUTES)


#define LED_BLOCK_SIZE 10

// This example shows several ways to set up and use 'palettes' of colors
// with FastLED.
//
// These compact palettes provide an easy way to re-colorize your
// animation on the fly, quickly, easily, and with low overhead.
//
// USING palettes is MUCH simpler in practice than in theory, so first just
// run this sketch, and watch the pretty lights as you then read through
// the code.  Although this sketch has eight (or more) different color schemes,
// the entire sketch compiles down to about 6.5K on AVR.
//
// FastLED provides a few pre-configured color palettes, and makes it
// extremely easy to make up your own color schemes with palettes.
//
// Some notes on the more abstract 'theory and practice' of
// FastLED compact palettes are at the bottom of this file.



CRGBPalette16 currentPalette;
TBlendType    currentBlending;
unsigned long clock_ticks;
short         cycle = 0;

extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;


void setup() {
  Serial.begin(9600);
  
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
    FastLED.setBrightness(  BRIGHTNESS );
    
    currentPalette = RainbowColors_p;
    currentBlending = LINEARBLEND;
    
}


void loop()
{
    clock_ticks=millis();
    printFreeMemory(__LINE__);
    //cyclePalettes();
    if (clock_ticks<CYCLE_1_TIME && cycle <= 0) {
      showCycle1();
      cycle = 1;
    } else if (clock_ticks<CYCLE_2_TIME && clock_ticks>=CYCLE_1_TIME && cycle <= 1) {
      showCycle2();
      cycle = 2;
    } else if (clock_ticks<CYCLE_3_TIME && clock_ticks>=CYCLE_2_TIME && cycle <= 3) {
      showCycle3();
      cycle = 3;
    } else if (clock_ticks<CYCLE_4_TIME && clock_ticks>=CYCLE_3_TIME && cycle <= 3) {
      showCycle4();
      cycle = 4;
    }
    
    FastLED.delay(1000 / UPDATES_PER_SECOND);
}

//only last led white
void showCycle1()
{
    fill_solid( leds, NUM_LEDS, CRGB::Black );
    leds[NUM_LEDS-1] = CRGB::White;
    printFreeMemory(__LINE__);
    FastLED.show();
}
//Last 10 leds white
void showCycle2()
{
    fill_solid( &(leds[NUM_LEDS-LED_BLOCK_SIZE-1]), LED_BLOCK_SIZE /*number of leds*/, CRGB::White );
    printFreeMemory(__LINE__);
    FastLED.show();
}
//Spit out white
void showCycle3()
{
    unsigned int cycle_3_elaped_milliseconds = clock_ticks - CYCLE_2_TIME;
    // Divide the strip up into 3 segments.  Pulse travel time through each segment is 1 second
    const short segment_sizes[4] = {10, 20, 50};
    const short segment_start[4] = {0, 10, 30};
    short segment = cycle_3_elaped_milliseconds/ 2 / SECONDS_PER_MILLI % 3 ;
    short segment_progress_percent = (cycle_3_elaped_milliseconds/2 % SECONDS_PER_MILLI) * 90 / SECONDS_PER_MILLI;
    short pulse_start = segment_start[segment] + segment_sizes[segment] * segment_progress_percent / 100;

    
    printFreeMemory(__LINE__);
  Serial.print(F("segment="));
  Serial.print(segment);
  Serial.print(F(" segment_progress_percent="));
  Serial.print(segment_progress_percent);
  Serial.print(F(" pulse_start="));
  Serial.println(pulse_start);

  
    fill_solid( leds, NUM_LEDS /*number of leds*/, CRGB::Black );
    fill_solid( &(leds[pulse_start]), LED_BLOCK_SIZE /*number of leds*/, CRGB::White );
    fill_solid( &(leds[NUM_LEDS-LED_BLOCK_SIZE-1]), LED_BLOCK_SIZE /*number of leds*/, CRGB::White );
    printFreeMemory(__LINE__);
    FastLED.show();
}
//Fade up/down white
void showCycle4()
{
  return;
}

void cyclePalettes()
{
    ChangePalettePeriodically();
    
    static uint8_t startIndex = 0;
    startIndex = startIndex + 1; /* motion speed */
    
    FillLEDsFromPaletteColors( startIndex);
  
}

void FillLEDsFromPaletteColors( uint8_t colorIndex)
{
    uint8_t brightness = 255;
    
    for( int i = NUM_LEDS-1; i >= 0 ; --i) {
        leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
        colorIndex += 3;
    }
}


// There are several different palettes of colors demonstrated here.
//
// FastLED provides several 'preset' palettes: RainbowColors_p, RainbowStripeColors_p,
// OceanColors_p, CloudColors_p, LavaColors_p, ForestColors_p, and PartyColors_p.
//
// Additionally, you can manually define your own color palettes, or you can write
// code that creates color palettes on the fly.  All are shown here.

void ChangePalettePeriodically()
{
    uint8_t secondHand = (millis() / 1000) % 60;
    static uint8_t lastSecond = 99;
    
    if( lastSecond != secondHand) {
        lastSecond = secondHand;
        if( secondHand ==  0)  { currentPalette = RainbowColors_p;         currentBlending = LINEARBLEND; }
        if( secondHand == 10)  { currentPalette = RainbowStripeColors_p;   currentBlending = NOBLEND;  }
        if( secondHand == 15)  { currentPalette = RainbowStripeColors_p;   currentBlending = LINEARBLEND; }
        if( secondHand == 20)  { SetupPurpleAndGreenPalette();             currentBlending = LINEARBLEND; }
        if( secondHand == 25)  { SetupTotallyRandomPalette();              currentBlending = LINEARBLEND; }
        if( secondHand == 30)  { SetupBlackAndWhiteStripedPalette();       currentBlending = NOBLEND; }
        if( secondHand == 35)  { SetupBlackAndWhiteStripedPalette();       currentBlending = LINEARBLEND; }
        if( secondHand == 40)  { currentPalette = CloudColors_p;           currentBlending = LINEARBLEND; }
        if( secondHand == 45)  { currentPalette = PartyColors_p;           currentBlending = LINEARBLEND; }
        if( secondHand == 50)  { currentPalette = myRedWhiteBluePalette_p; currentBlending = NOBLEND;  }
        if( secondHand == 55)  { currentPalette = myRedWhiteBluePalette_p; currentBlending = LINEARBLEND; }
    }
}

// This function fills the palette with totally random colors.
void SetupTotallyRandomPalette()
{
    for( int i = 0; i < 16; ++i) {
        currentPalette[i] = CHSV( random8(), 255, random8());
    }
}

// This function sets up a palette of black and white stripes,
// using code.  Since the palette is effectively an array of
// sixteen CRGB colors, the various fill_* functions can be used
// to set them up.
void SetupBlackAndWhiteStripedPalette()
{
    // 'black out' all 16 palette entries...
    fill_solid( currentPalette, 16, CRGB::Black);
    // and set every fourth one to white.
    currentPalette[0] = CRGB::White;
    currentPalette[4] = CRGB::White;
    currentPalette[8] = CRGB::White;
    currentPalette[12] = CRGB::White;
    
}

// This function sets up a palette of purple and green stripes.
void SetupPurpleAndGreenPalette()
{
    CRGB purple = CHSV( HUE_PURPLE, 255, 255);
    CRGB green  = CHSV( HUE_GREEN, 255, 255);
    CRGB black  = CRGB::Black;
    
    currentPalette = CRGBPalette16(
                                   green,  green,  black,  black,
                                   purple, purple, black,  black,
                                   green,  green,  black,  black,
                                   purple, purple, black,  black );
}


// This example shows how to set up a static color palette
// which is stored in PROGMEM (flash), which is almost always more
// plentiful than RAM.  A static PROGMEM palette like this
// takes up 64 bytes of flash.
const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM =
{
    CRGB::Red,
    CRGB::Gray, // 'white' is too bright compared to red and blue
    CRGB::Blue,
    CRGB::Black,
    
    CRGB::Red,
    CRGB::Gray,
    CRGB::Blue,
    CRGB::Black,
    
    CRGB::Red,
    CRGB::Red,
    CRGB::Gray,
    CRGB::Gray,
    CRGB::Blue,
    CRGB::Blue,
    CRGB::Black,
    CRGB::Black
};




void printFreeMemory(int line){
  
  Serial.print(F("free memory="));
  Serial.print(freeMemory());
  Serial.print(F(" at line "));
  Serial.println(line);
}

#ifdef __arm__
// should use uinstd.h to define sbrk but Due causes a conflict
extern "C" char* sbrk(int incr);
#else  // __ARM__
extern char *__brkval;
#endif  // __arm__

int freeMemory() {
  char top;
#ifdef __arm__
  return &top - reinterpret_cast<char*>(sbrk(0));
#elif defined(CORE_TEENSY) || (ARDUINO > 103 && ARDUINO != 151)
  return &top - __brkval;
#else  // __arm__
  return __brkval ? &top - __brkval : &top - __malloc_heap_start;
#endif  // __arm__
}

// Additional notes on FastLED compact palettes:
//
// Normally, in computer graphics, the palette (or "color lookup table")
// has 256 entries, each containing a specific 24-bit RGB color.  You can then
// index into the color palette using a simple 8-bit (one byte) value.
// A 256-entry color palette takes up 768 bytes of RAM, which on Arduino
// is quite possibly "too many" bytes.
//
// FastLED does offer traditional 256-element palettes, for setups that
// can afford the 768-byte cost in RAM.
//
// However, FastLED also offers a compact alternative.  FastLED offers
// palettes that store 16 distinct entries, but can be accessed AS IF
// they actually have 256 entries; this is accomplished by interpolating
// between the 16 explicit entries to create fifteen intermediate palette
// entries between each pair.
//
// So for example, if you set the first two explicit entries of a compact 
// palette to Green (0,255,0) and Blue (0,0,255), and then retrieved 
// the first sixteen entries from the virtual palette (of 256), you'd get
// Green, followed by a smooth gradient from green-to-blue, and then Blue.
