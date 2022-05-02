#include <FastLED.h>
#include <limits.h>
#include <Arduino.h>
extern char *__brkval;

#define MAX_CHANNEL INT8_MAX
#define AGE_RANGE 200
#define MIN_BRIGHTNESS 3

long percentToInt8(long percent)
{
  
  /*Serial.print(F("percentToInt8 percent="));
  Serial.print(percent);
  Serial.print(F(" Int8= "));
  Serial.println(percent * MAX_CHANNEL / 100);*/
    return percent * MAX_CHANNEL / 100;
}
void scaleCRGBByPercent(CRGB& led, unsigned short percent)
{
  /*Serial.print(F("scaleCRGBByPercent percent="));
  Serial.print(percent);
  Serial.print(F(" led.red= "));
  Serial.print(led.red);
  Serial.print(F(" scaled red= "));
  Serial.print(led.red * percent/100);*/
    led.red = led.red * percent/100;
    led.green = led.green * percent/100;
    led.blue = led.blue * percent/100;
    //if (led.green< MIN_BRIGHTNESS) led.green =0;
   // if (led.blue< MIN_BRIGHTNESS) led.blue =0;
  /*Serial.print(F(" actual red= "));
  Serial.println(led.red);*/
}
unsigned short favourLowIntensity(unsigned short percent)
{
    return (unsigned long) percent * percent * percent / 100 / 100;
}
void AdjustLowLight(CRGB& led)
{
 /* Serial.println(F("---"));
  Serial.print(F("AdjustLowLight red="));
  Serial.print(red);
  Serial.print(F(" speed= "));
  Serial.print(speed);
  Serial.print(F(" age= "));
  Serial.println(age);*/
    if (led.red == 5) {
      led.blue = 5;
      led.green = 5;
      //led.red = 0;
    }
    if (led.red == 4) {
      led.blue = 5;
      led.green = 5;
      //led.red = 0;
    }
    if (led.red == 3) {
      led.blue = 4;
      led.green = 4;
      //led.red = 0;
    }
    if (led.red == 2) {
      led.blue = 3;
      led.green = 3;
      //led.red = 0;
    }
    if (led.red == 1) {
      led.blue = 2;
      led.green = 2;
      led.red = 2;
    }
}
unsigned short ageToFadePercent(unsigned long age)
{
    short range = age % AGE_RANGE; // fade up from 1 to 100%, fade down from 100% to 0 
    short activity = range - 99; // -99 to 100
    unsigned short fade = abs(activity); //100 -> 0 -> 100
  /*Serial.print(F("ageToFadePercent age="));
  Serial.print(age);
  Serial.print(F(" fade= "));
  Serial.println(fade);*/
    return fade; // * brightness / 100;
}
void fadeEntireStrip(unsigned long clock_ticks, unsigned short speed, CRGB* leds, unsigned short start_led, unsigned short end_led)
{
    unsigned long age = (double)clock_ticks * 100 / speed; // convert clock tick to a percentage
 /* Serial.println(F("---"));
  Serial.print(F("fadeEntireStrip clock_ticks="));
  Serial.print(clock_ticks);
  Serial.print(F(" speed= "));
  Serial.print(speed);
  Serial.print(F(" age= "));
  Serial.println(age);*/
    CRGB* array_start = leds + (start_led-1) * sizeof(CRGB);
    unsigned short num_leds = end_led - start_led + 1;

    CRGB sample;
    sample = CRGB::White;
    scaleCRGBByPercent(sample, favourLowIntensity(ageToFadePercent(age)));
    AdjustLowLight(sample); 

    fill_solid(array_start, num_leds, sample );
    FastLED.show();
}



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