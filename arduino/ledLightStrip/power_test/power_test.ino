
#include <Adafruit_NeoPixel.h>
#define LEDPIN1 13 //40 led matix
#define BUTTONPIN 7  //90 big red button
#define BUTTONLEDPIN 12  //90 pin strip
#define N_MAX_LEDS 560  //Total LED max = 560
//Adafruit_NeoPixel strip1 = Adafruit_NeoPixel(10, 13, NEO_GRB + NEO_KHZ800);//40
Adafruit_NeoPixel strip2 = Adafruit_NeoPixel(10, 2, NEO_GRB + NEO_KHZ800);//78
Adafruit_NeoPixel strip3 = Adafruit_NeoPixel(10, 3, NEO_GRB + NEO_KHZ800);//78
Adafruit_NeoPixel strip4 = Adafruit_NeoPixel(10, 1 , NEO_GRB + NEO_KHZ800);//78
Adafruit_NeoPixel strip5 = Adafruit_NeoPixel(234, 5, NEO_GRB + NEO_KHZ800);//156
Adafruit_NeoPixel strip6 = Adafruit_NeoPixel(270, 6, NEO_GRB + NEO_KHZ800);//300
//1 x 78, 2 x 156, 3 x 234
//5m = 300

#define MAX_INT_16 32000
#define COLORS_BLOCK_LEN 10
#define SAME_COLORS_LEN 2
#define NUM_COLORS 8
  uint32_t white = strip2.Color(255, 255, 255);
  uint32_t superwhite = strip2.Color(250, 250, 250);
  uint32_t black = strip2.Color(0, 0, 0);
  uint32_t colors[NUM_COLORS] = {
    black,
    strip2.Color(2, 2, 2),
    strip2.Color(50, 50, 50),
    white,
    superwhite,
    strip2.Color(255, 0, 0),
    strip2.Color(0, 255, 0),
    strip2.Color(0, 0, 255),
  };
  int color_index = 0;
  
void setup() {
  Serial.begin(9600);
  pinMode(BUTTONLEDPIN, OUTPUT);
  pinMode(BUTTONPIN, INPUT);
  //strip1.begin();
  strip2.begin();
  strip3.begin();
  strip4.begin();
  strip5.begin();
  strip6.begin();
}
void loop() {
  printFreeMemory(__LINE__);
  chase();
}
static void chase() {
    uint32_t highlight_color = black;
    choose_colors(highlight_color);
    //update_strip(strip1, highlight_color);
    update_strip(strip2, highlight_color);
    update_strip(strip3, highlight_color);
    update_strip(strip4, highlight_color);
    update_strip(strip5, highlight_color);
    update_strip(strip6, highlight_color);
  delay(250);
}
static int choose_colors(uint32_t &highlight_color) {
    if (is_button_pressed()) {
      color_index = color_index + 1;
      color_index = color_index % NUM_COLORS;
    } 
    highlight_color = colors[color_index];
}
static bool is_button_pressed(){
    int button_voltage = digitalRead(BUTTONPIN);
    if (HIGH == button_voltage) 
      return true;
    return false;
}
static int update_strip(Adafruit_NeoPixel &strip, uint32_t highlight_color) {
  //strip.updateLength (300);
  for(uint16_t i=0; i<N_MAX_LEDS; i++) {
    if (highlight_color == superwhite) {      
      if (i%10 == 0) {
        strip.setPixelColor(i, highlight_color);      
      } else {
        strip.setPixelColor(i, black);
      }
    } else {
      strip.setPixelColor(i, highlight_color);
      if (i%2000 == 0) strip.show();
    }
  }
  strip.show();
  //strip.updateLength (3);
}


void printFreeMemory(int line){
  
  Serial.print(F("free memory="));
  Serial.print(freeMemory());
  Serial.print(F(" at aline "));
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
