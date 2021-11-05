
#include <Adafruit_NeoPixel.h>

#define LEDSTRIP1 1 
#define LEDSTRIP2 2 
#define LEDSTRIP3 3 
#define LEDSTRIPNone 4
#define LEDSTRIP5 5 
#define LEDSTRIP6 6 

#define DIAGNOSTICBUTTON 7  //7
#define BIGBUTTON1 8 
#define BIGBUTTON2 9 
#define BIGBUTTON3 10 
#define BIGBUTTON5 11
#define BIGBUTTON6 12 
#define NUM_BUTTONS 6


#define N_MAX_LEDS 560  //Total LED max = 560

Adafruit_NeoPixel strip1 = Adafruit_NeoPixel(1, LEDSTRIP1, NEO_GRB + NEO_KHZ800);//78
Adafruit_NeoPixel strip2 = Adafruit_NeoPixel(1, LEDSTRIP2, NEO_GRB + NEO_KHZ800);//78
Adafruit_NeoPixel strip3 = Adafruit_NeoPixel(1, LEDSTRIP3 , NEO_GRB + NEO_KHZ800);//78
Adafruit_NeoPixel strip5 = Adafruit_NeoPixel(1, LEDSTRIP5, NEO_GRB + NEO_KHZ800);//1 x 78, 2 x 156, 3 x 234
Adafruit_NeoPixel strip6 = Adafruit_NeoPixel(1, LEDSTRIP6, NEO_GRB + NEO_KHZ800);//5m = 300

#define STOPPED_LEDSTRIP_ALREADY 0
#define STOP_LEDSTRIP_NOW 1

#define SECOND_TO_MILLISECOND 1000
#define LED_EFFECT_DURATION (5 * SECOND_TO_MILLISECOND) //30

#define COLORS_BLOCK_LEN 10
#define SAME_COLORS_LEN 2

#define NUM_COLORS 8
uint32_t white = strip2.Color(255, 255, 255);
uint32_t superwhite = strip2.Color(250, 250, 250);
uint32_t black = strip2.Color(0, 0, 0);
uint32_t colors[NUM_COLORS] = {
    black,
    strip2.Color(2, 2, 2),
    strip2.Color(2, 2, 2),
    strip2.Color(2, 2, 2),
    strip2.Color(2, 2, 2),
    strip2.Color(2, 2, 2),
    strip2.Color(2, 2, 2),
    strip2.Color(50, 50, 50),/*
    white,
    superwhite,
    strip2.Color(255, 0, 0),
    strip2.Color(0, 255, 0),
    strip2.Color(0, 0, 255),*/
};
int color_index = 0;

Adafruit_NeoPixel* led_strips[NUM_BUTTONS+1] = {nullptr ,nullptr ,nullptr ,nullptr ,nullptr ,nullptr ,nullptr};
uint16_t led_strip_length[NUM_BUTTONS+1] = {0, 78, 78, 78, 0, 234, 300}; //uint16_t led_strip_length[NUM_BUTTONS+1] = {0, 8, 8, 8, 0, 234, 3};
uint16_t led_strip_location[NUM_BUTTONS+1] = {0, LEDSTRIP1, LEDSTRIP2, LEDSTRIP3, LEDSTRIPNone, LEDSTRIP5, LEDSTRIP6};
int led_stop_time[NUM_BUTTONS+1] = {STOPPED_LEDSTRIP_ALREADY, STOPPED_LEDSTRIP_ALREADY, STOPPED_LEDSTRIP_ALREADY, 
        STOPPED_LEDSTRIP_ALREADY, STOPPED_LEDSTRIP_ALREADY, STOPPED_LEDSTRIP_ALREADY, STOPPED_LEDSTRIP_ALREADY};
  
void setup() {
  Serial.begin(9600);
  initilaise_led_strip(1, LEDSTRIP1, BIGBUTTON1, strip1);
  initilaise_led_strip(2, LEDSTRIP2, BIGBUTTON2, strip2);
  initilaise_led_strip(3, LEDSTRIP3, BIGBUTTON3, strip3);
  initilaise_led_strip(5, LEDSTRIP5, BIGBUTTON5, strip5);
  initilaise_led_strip(6, LEDSTRIP6, BIGBUTTON6, strip6);
}

void loop() {
  printFreeMemory(__LINE__);
  check_buttons();  
  for(byte strip=1; strip<=NUM_BUTTONS; strip++) {
    display_strip(strip);
  }  
  color_index = color_index + 1;
  color_index = color_index % NUM_COLORS;
  
  delay(250);
}

void check_buttons() {
  int current_time = millis();
  
  //only allow small strips to work if the big strips are stopped
  if (STOPPED_LEDSTRIP_ALREADY == led_stop_time[5] && STOPPED_LEDSTRIP_ALREADY == led_stop_time[6]) {
    if (is_button_pressed(BIGBUTTON1)) led_stop_time[1] = current_time + 1 * SECOND_TO_MILLISECOND;
    if (is_button_pressed(BIGBUTTON2)) led_stop_time[2] = current_time + 1 * SECOND_TO_MILLISECOND;
    if (is_button_pressed(BIGBUTTON3)) led_stop_time[3] = current_time + 1 * SECOND_TO_MILLISECOND;
  }
  
  if (is_button_pressed(BIGBUTTON5)) {
    led_stop_time[5] = current_time + LED_EFFECT_DURATION;
    stop_small_strips();
  }
  if (is_button_pressed(BIGBUTTON6)) {
    led_stop_time[6] = current_time + LED_EFFECT_DURATION;
    stop_small_strips();
  }
}

void stop_small_strips(){
    stop_led_strip(1);
    stop_led_strip(2);
    stop_led_strip(3);
}
void stop_led_strip(byte strip){
  if (STOPPED_LEDSTRIP_ALREADY == led_stop_time[strip] ) return;
  led_stop_time[strip] = STOPPED_LEDSTRIP_ALREADY;
  Serial.print(F("Stopped strip ")); Serial.println(strip);
  if (nullptr != led_strips[strip]) {
    activate_strip(strip);
    led_strips[strip]->clear();
    led_strips[strip]->show();
    dectivate_strip(strip);
  }
}

void display_strip(byte strip) {
  if (nullptr == led_strips[strip]) return;
  int current_time = millis();
  if (current_time > led_stop_time[strip]){
    stop_led_strip(strip);
  } else {
    if (STOPPED_LEDSTRIP_ALREADY == led_stop_time[strip] ) return;
    printFreeMemory(led_stop_time[strip]);
    activate_strip(strip);
    show_effects(*led_strips[strip]);
    dectivate_strip(strip);
  }
}

void initilaise_led_strip(byte strip_num, byte strip_pin, byte strip_button, Adafruit_NeoPixel &strip_ref) {
  pinMode(strip_pin, OUTPUT);
  pinMode(strip_button,INPUT_PULLUP); // TODO rewire hardware buttons
  //pinMode(strip_button, INPUT);
  strip1.begin();
  led_strips[strip_num] = &strip_ref;
  dectivate_strip(strip_num);
}

void activate_strip(byte strip) {
  led_strips[strip]->setPin(led_strip_location[strip]);
  if (led_strips[strip]->numPixels() != led_strip_length[strip]) {
    led_strips[strip]->updateLength(led_strip_length[strip]);
  }  
}
void dectivate_strip(byte strip) {
  led_strips[strip]->setPin(LEDSTRIPNone);
  if (led_strips[strip]->numPixels() >1) {
    led_strips[strip]->updateLength(1);
  }  
}


static int show_effects(Adafruit_NeoPixel &strip) {
  for(uint16_t i=0; i<N_MAX_LEDS; i++) {
    if (color_index == superwhite) {      
      if (i%10 == 0) {
        strip.setPixelColor(i, colors[color_index]);      
      } else {
        strip.setPixelColor(i, black);
      }
    } else {
      strip.setPixelColor(i, colors[color_index]);
    }
  }
  strip.show();
}


/*static void chase() {
    uint32_t highlight_color = black;
    choose_colors(highlight_color);
    update_strip(strip1, highlight_color);
    update_strip(strip2, highlight_color);
    update_strip(strip3, highlight_color);
    update_strip(strip5, highlight_color);
    update_strip(strip6, highlight_color);
  delay(250);
}
static int choose_colors(uint32_t &highlight_color) {
    if (is_button_pressed(DIAGNOSTICBUTTON)) {
      color_index = color_index + 1;
      color_index = color_index % NUM_COLORS;
    } 
    highlight_color = colors[color_index];
}*/
static bool is_button_pressed(byte button){
    if (LOW == digitalRead(button)) {
      Serial.print(F("Button pressed =  ")); Serial.println(button);
      return true;
    }
    return false;
}
static int update_strip(Adafruit_NeoPixel &strip, uint32_t highlight_color) {
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
}

/*
static void old_chase() {
  for(uint16_t i=COLORS_BLOCK_LEN-1; i< INT16_MAX; i++) {
    uint32_t highlight_color = black;
    uint32_t erase_color = black;
    old_choose_colors(i, highlight_color, erase_color);
    old_update_strip(strip1, i, highlight_color, erase_color);
    old_update_strip(strip2, i, highlight_color, erase_color);
    old_update_strip(strip3, i, highlight_color, erase_color);
    delay(25);
  }
}
static int old_choose_colors(uint16_t i, uint32_t &highlight_color, uint32_t &erase_color) {
    if (is_button_pressed(BIGBUTTON1)) {
      int next_color;
      int color_index;
      next_color = i/SAME_COLORS_LEN;
      color_index = (next_color)%NUM_COLORS;
      highlight_color = colors[color_index];
      erase_color = white;
    } else {
      highlight_color = black;
      erase_color = black;
    }
}
static int old_update_strip(Adafruit_NeoPixel &strip, int led_position, 
                        uint32_t highlight_color, uint32_t erase_color) {
  int front = front_pixel(led_position, strip.numPixels());
  int back = back_pixel(led_position, strip.numPixels());
  strip.setPixelColor(front, highlight_color); // Draw new pixel
  strip.setPixelColor(back, erase_color); // Erase pixel a few steps back
  strip.show();
}
static int front_pixel(int iterator, int max_front) {
  return(iterator%max_front);
}
static int back_pixel(int iterator, int max_front) {
  return front_pixel(iterator-COLORS_BLOCK_LEN, max_front);
}*/


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
