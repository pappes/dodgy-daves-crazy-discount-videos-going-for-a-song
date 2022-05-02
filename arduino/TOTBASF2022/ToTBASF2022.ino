//#include "..\LEDEffects.hpp"
#include "C:\Users\dave\Documents\git\dodgy\arduino\LEDEffects.hpp"

#define LED_PIN     10
#define BRIGHTNESS_PERCENT  100
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB


#define LED_SPEED (10 * SECONDS)
#define START_LED 1
#define LED_1M_LENGTH 78
//#define LED_1M_LENGTH 25
#define END_LED (LED_1M_LENGTH*2)
#define NUM_LEDS    (END_LED - START_LED +1)


CRGB leds[NUM_LEDS];

#define UPDATES_PER_SECOND 1000

#define SECONDS 1000L
#define MINUTES (SECONDS*60L)
#define CYCLE_1_TIME (2L * SECONDS)
#define CYCLE_2_TIME (4L * SECONDS)
#define CYCLE_3_TIME (6L * SECONDS)
#define CYCLE_4_TIME (8L * SECONDS)
/*#define CYCLE_2_TIME (CYCLE_1_TIME + (60L * SECONDS))
#define CYCLE_3_TIME (CYCLE_2_TIME + (3L * MINUTES)) 
#define CYCLE_4_TIME (CYCLE_3_TIME + (10L * MINUTES))*/


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
  Serial.print(F("starting"));
  
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
    FastLED.setBrightness(  percentToInt8(BRIGHTNESS_PERCENT) );
    
    currentPalette = RainbowColors_p;
    currentBlending = LINEARBLEND;
    
}





void loop()
{
    clock_ticks=millis();
  Serial.print(F("cycle="));
  Serial.print(cycle);
  Serial.print(F(" clock_ticks "));
  Serial.println(clock_ticks);
    printFreeMemory(__LINE__);

    fadeEntireStrip(clock_ticks, LED_SPEED, leds, START_LED, END_LED);
    
    //cyclePalettes();
    /*if (clock_ticks<CYCLE_1_TIME && cycle <= 0) {
      showCycle1();
      cycle = 1;
    } else if (clock_ticks<CYCLE_2_TIME && clock_ticks>=CYCLE_1_TIME && cycle <= 1) {
      showCycle2();
      cycle = 2;
    } else if (clock_ticks<CYCLE_3_TIME && clock_ticks>=CYCLE_2_TIME && cycle <= 3) {
      showCycle3();
      cycle = 3;
    } else if (clock_ticks>=CYCLE_3_TIME && cycle <= 4) {
      showCycle4();
      cycle = 4;
    }*/
    
    FastLED.delay(SECONDS / UPDATES_PER_SECOND);
}

//only last led white
void showCycle1()
{
    printFreeMemory(__LINE__);
    fill_solid( leds, NUM_LEDS, CRGB::Black );
    FastLED.show();
}
//Last 10 leds white
void showCycle2()
{
    printFreeMemory(__LINE__);
    fill_solid( leds, NUM_LEDS, CRGB::White );
    FastLED.show();
}
//Spit out white
void showCycle3()
{
    printFreeMemory(__LINE__);
    unsigned long pulse_delay = 0L;
    unsigned long cycle_3_elaped_milliseconds = clock_ticks - CYCLE_2_TIME;
    
    // Divide the strip up into 3 segments.  Pulse travel time through each segment is 1 second
    const short segment_sizes[4] = {10, 20, 50};
    const short segment_start[4] = {0, 10, 30};
    short segment = cycle_3_elaped_milliseconds/ (pulse_delay+1L) / SECONDS % 3 ;
    short segment_progress_percent = (cycle_3_elaped_milliseconds/ (pulse_delay+1L) % SECONDS) * 100 / SECONDS;
    short pulse_start = segment_start[segment] + segment_sizes[segment] * segment_progress_percent / 100;

  
  Serial.print(F("segment="));
  Serial.print(segment);
  Serial.print(F(" segment_progress_percent "));
  Serial.println(segment_progress_percent);

  
  Serial.print(F("pulse_start="));
  Serial.print(pulse_start);
  Serial.print(F(" cycle_3_elaped_milliseconds "));
  Serial.println(cycle_3_elaped_milliseconds);
    fill_solid( leds, NUM_LEDS /*number of leds*/, CRGB::Black );
    if (pulse_start + LED_BLOCK_SIZE > NUM_LEDS){
        pulse_start = NUM_LEDS-LED_BLOCK_SIZE-1; 
    }
    fill_solid( &(leds[pulse_start]), LED_BLOCK_SIZE /*number of leds*/, CRGB::White );
    if (segment<2 || segment_progress_percent > 90) {
      fill_solid( &(leds[NUM_LEDS-LED_BLOCK_SIZE-1]), LED_BLOCK_SIZE /*number of leds*/, CRGB::White );
    }
    FastLED.show();
}
//Fade up/down white
void showCycle4()
{
    printFreeMemory(__LINE__);
   static short brightness = 0;
   static short fadeAmount = 5;
   
    fill_solid( leds, NUM_LEDS /*number of leds*/, CRGB(brightness, brightness, brightness) );
  FastLED.show();
  brightness = brightness + fadeAmount;
  // reverse the direction of the fading at the ends of the fade: 
  if(brightness <= 0 || brightness >= 255)
  {
    fadeAmount = -fadeAmount ; 
    brightness = brightness + fadeAmount;
  }    
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
