
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


#define N_MAX_LEDS 300  // Total LED max = 560 based on memory usage

Adafruit_NeoPixel strip1 = Adafruit_NeoPixel(1, LEDSTRIP1, NEO_GRB + NEO_KHZ800); // 78
Adafruit_NeoPixel strip2 = Adafruit_NeoPixel(1, LEDSTRIP2, NEO_GRB + NEO_KHZ800); // 78
Adafruit_NeoPixel strip3 = Adafruit_NeoPixel(1, LEDSTRIP3 , NEO_GRB + NEO_KHZ800);// 78
Adafruit_NeoPixel strip5 = Adafruit_NeoPixel(1, LEDSTRIP5, NEO_GRB + NEO_KHZ800); // 3 x 78 = 234
Adafruit_NeoPixel strip6 = Adafruit_NeoPixel(1, LEDSTRIP6, NEO_GRB + NEO_KHZ800); // 5m = 300

#define STOP_LEDSTRIP_NOW 1
#define STOPPED_LEDSTRIP_ALREADY 0

//Defeine times as lnog datatype to ensure that time calculations do not exhibit rounding errors
#define SECOND_TO_MILLISECOND 1000L
#define LED123_EFFECT_DURATION 4L
#define LED6_EFFECT_DURATION 30L
#define LED5_EFFECT_DURATION 21L


#define COLORS_BLOCK_LEN 10
#define SAME_COLORS_LEN 2

#define NUM_COLORS 8
uint32_t white = strip2.Color(255, 255, 255);
uint32_t superwhite = strip2.Color(250, 250, 250);
uint32_t black = strip2.Color(0, 0, 0);
uint32_t colors[NUM_COLORS] = {
    strip2.Color(0, 0, 150),
    strip2.Color(0, 100, 50),
    strip2.Color(0, 150, 0),
    strip2.Color(100, 50, 0),
    strip2.Color(150, 0, 0),
    superwhite,
    strip2.Color(2, 2, 2),
    strip2.Color(50, 50, 50),/*
    white,
    superwhite,
    strip2.Color(255, 0, 0),
    strip2.Color(0, 255, 0),
    strip2.Color(0, 0, 255),
    strip2.Color(10, 10, 10),
    strip2.Color(20, 20, 20),
    strip2.Color(40, 40, 40),*/
};
int color_index = 0;

long current_time = 0;

byte                led_strip_color_index[NUM_BUTTONS+1] =  {0, 0, 0, 0, 0, 0, 0};
Adafruit_NeoPixel*  led_strips[NUM_BUTTONS+1] =             {nullptr ,nullptr ,nullptr ,nullptr ,nullptr ,nullptr ,nullptr};
short               led_strip_length[NUM_BUTTONS+1] =       {0, 78, 78, 78, 0, 234, 300}; // actual lengths are 0, 78, 78, 78, 0, 234, 300
byte                led_strip_location[NUM_BUTTONS+1] =     {0, LEDSTRIP1, LEDSTRIP2, LEDSTRIP3, LEDSTRIPNone, LEDSTRIP5, LEDSTRIP6};
long                led_stop_time[NUM_BUTTONS+1] =          {STOPPED_LEDSTRIP_ALREADY, STOPPED_LEDSTRIP_ALREADY, STOPPED_LEDSTRIP_ALREADY, 
                                                             STOPPED_LEDSTRIP_ALREADY, STOPPED_LEDSTRIP_ALREADY, STOPPED_LEDSTRIP_ALREADY, 
                                                             STOPPED_LEDSTRIP_ALREADY};
long                led_next_effect_time[NUM_BUTTONS+1] =   {STOPPED_LEDSTRIP_ALREADY, STOPPED_LEDSTRIP_ALREADY, STOPPED_LEDSTRIP_ALREADY, 
                                                             STOPPED_LEDSTRIP_ALREADY, STOPPED_LEDSTRIP_ALREADY, STOPPED_LEDSTRIP_ALREADY, 
                                                             STOPPED_LEDSTRIP_ALREADY};
  
void setup() {
  Serial.begin(9600);
  initialise_led_strip(1, LEDSTRIP1, BIGBUTTON1, strip1);
  initialise_led_strip(2, LEDSTRIP2, BIGBUTTON2, strip2);
  initialise_led_strip(3, LEDSTRIP3, BIGBUTTON3, strip3);
  initialise_led_strip(5, LEDSTRIP5, BIGBUTTON5, strip5);
  initialise_led_strip(6, LEDSTRIP6, BIGBUTTON6, strip6);
}

void loop() {
  current_time = millis();
  if (current_time % 100 == 0) printFreeMemory(__LINE__);
  check_buttons();  
  for(byte strip=1; strip<=NUM_BUTTONS; strip++) {
    display_strip(strip);
  }  
  color_index = tick_twice_per_second()  % NUM_COLORS;
  
  delay(10);
}

int tick_twice_per_second(){
  return (current_time * 2L / SECOND_TO_MILLISECOND);
}

void check_buttons() {  
  //only allow small strips to work if the big strips are stopped
  if (STOPPED_LEDSTRIP_ALREADY == led_stop_time[5] && STOPPED_LEDSTRIP_ALREADY == led_stop_time[6]) {
    if (is_button_pressed(BIGBUTTON1)) led_stop_time[1] = get_stop_time(LED123_EFFECT_DURATION);
    if (is_button_pressed(BIGBUTTON2)) led_stop_time[2] = get_stop_time(LED123_EFFECT_DURATION);
    if (is_button_pressed(BIGBUTTON3)) led_stop_time[3] = get_stop_time(LED123_EFFECT_DURATION);
  }
  
  if (is_button_pressed(BIGBUTTON5)) {
    led_stop_time[5] = get_stop_time(LED5_EFFECT_DURATION);
    stop_small_strips();
  }
  if (is_button_pressed(BIGBUTTON6)) {
    led_stop_time[6] = get_stop_time(LED6_EFFECT_DURATION);
    stop_small_strips();
  }
}

long get_stop_time(int seconds) {
  long stop_duration = seconds * SECOND_TO_MILLISECOND;
  return current_time + stop_duration;
}
void stop_small_strips(){
    stop_led_strip(1);
    stop_led_strip(2);
    stop_led_strip(3);
}
void stop_led_strip(byte strip_num){
  if (STOPPED_LEDSTRIP_ALREADY == led_stop_time[strip_num] ) return;
  led_stop_time[strip_num] = STOPPED_LEDSTRIP_ALREADY;
  led_strip_color_index[strip_num] = 0;
  Serial.print(F("Stopped strip ")); Serial.println(strip_num);
  if (nullptr != led_strips[strip_num]) {
    activate_strip(strip_num);
    led_strips[strip_num]->clear();
    led_strips[strip_num]->show();
    dectivate_strip(strip_num);
  }
}

void display_strip(byte strip) {
  if (nullptr == led_strips[strip]) return;
  if (current_time > led_stop_time[strip] && led_stop_time[strip] > 0){
    stop_led_strip(strip);
  } else {
    if (STOPPED_LEDSTRIP_ALREADY == led_stop_time[strip] ) return;
    activate_strip(strip);
    show_effects(strip);
    dectivate_strip(strip);
  }
}

void initialise_led_strip(byte strip_num, byte strip_pin, byte strip_button, Adafruit_NeoPixel &strip_ref) {
  pinMode(strip_pin, OUTPUT);
  if (is_big_button(strip_button)) {
    pinMode(strip_button, INPUT); //allow in button LEDs
    Serial.print(F("Big button initialised to INPUT =  ")); Serial.println(strip_button);
  }
  else {
    pinMode(strip_button,INPUT_PULLUP); // TODO rewire hardware buttons
    Serial.print(F("Little button initialised to PULLUP =  ")); Serial.println(strip_button);
  }
  strip_ref.begin();
  led_strips[strip_num] = &strip_ref;
  dectivate_strip(strip_num);
}
bool is_button_pressed(byte button){
  byte button_state = digitalRead(button);
  if (is_big_button(button) && HIGH == button_state) {
    Serial.print(F("Big button pressed =  ")); Serial.println(button);
    return true;
  }
  if (!is_big_button(button) && LOW == button_state) {
    Serial.print(F("Little button pressed =  ")); Serial.println(button);
    return true;
  }
  return false;
}

bool is_big_button(byte button){
  // Big buttons are shorted to ground and can have LEDs
  if (button <= BIGBUTTON3) {
    return true;
  }
  if (button != DIAGNOSTICBUTTON) {
    return true;
  }
  // Little buttons are "floating" when unpressed and connected to ground when pressed
  return false;
}


void show_effects(byte strip_num) {
  if (nullptr == led_strips[strip_num]) return;
  //show_effect_default(strip_num);
  if (strip_num<4) show_effect_1(strip_num);
  else if (strip_num == 5) show_effect_3(strip_num);
  else if (strip_num == 6) show_effect_2(strip_num);
  else show_effect_default(strip_num);
}

void show_effect_default(byte strip_num) {
  Adafruit_NeoPixel *strip = led_strips[strip_num];
  for(uint16_t i=0; i<N_MAX_LEDS; i++) {
    if (color_index == superwhite) {      
      if (i%10 == 0) {
        strip->setPixelColor(i, colors[color_index]);      
      } else {
        strip->setPixelColor(i, black);
      }
    } else {
      strip->setPixelColor(i, colors[color_index]);
    }
  }
  strip->show();
}

void show_effect_1(byte strip_num) {
  Adafruit_NeoPixel *strip = led_strips[strip_num];
  uint16_t new_color = black;
  int time_reamining = led_stop_time[strip_num] - current_time;
  int fade_time = 2 * SECOND_TO_MILLISECOND;
  byte max_intensity = 150;
  byte main_intensity = 100;
  byte secondary_intensity = max_intensity - main_intensity;
  
   if ( time_reamining < fade_time) {
    // Fade down.
    int intensity_factor = fade_time / max_intensity;
    byte color_intensity = byte(time_reamining / intensity_factor);
    new_color = strip->Color(color_intensity, color_intensity, color_intensity); 
  } else if ( led_strip_color_index[strip_num] < 20) {
    // Fade up for 20 cycles.
    byte color_index = led_strip_color_index[strip_num]++;
    byte color_intensity = color_index * 5;
    new_color = strip->Color(color_intensity, color_intensity, color_intensity); 
  } else if (led_next_effect_time[strip_num] < current_time) {
    // Random colors of a similar hue.
    led_next_effect_time[strip_num] = current_time + 200; //200 miliseconds to cycle through colors
    byte random_num = current_time % secondary_intensity;
    if (strip_num % 3 == 0)
      new_color = strip->Color(main_intensity, random_num, secondary_intensity-random_num);
    else if (strip_num % 3 == 1)
      new_color = strip->Color(random_num, main_intensity, secondary_intensity-random_num);
    else 
      new_color = strip->Color(random_num, secondary_intensity-random_num, main_intensity);
  } else return;
  strip->fill(new_color); 
  strip->show();
}

#define PULSE_5A 6
#define PULSE_5B 5
#define PULSE_5C 4
#define PULSE_5D 6
#define PULSE_5E 5
#define PULSE_5F 4
void show_effect_2(byte strip_num) {
  // Light strip has 3 segments |0------1------2------3|
  // Pulse A from 3 to 2 in 6 seconds
  // Pulse B from 1 to 2 in 5 seconds
  // Pulse C from 0 to 1 in 4 seconds
  // Pulse D from 3 to 1 in 6 seconds
  // Pulse E from 0 to 2 in 5 seconds
  // Pulse F from 0 to 3 in 4 seconds
  Adafruit_NeoPixel *strip = led_strips[strip_num];
  long time_remaining = led_stop_time[strip_num] - current_time;
  byte seconds_remaining = time_remaining/SECOND_TO_MILLISECOND;
  byte max_rgb_intensity = 300/3;
  byte main_rgb_intensity = 15/3;
  byte section1_len = strip->numPixels()/3;
  byte section2_len = section1_len;
  byte section3_len = section1_len;
  uint32_t pulse_color = strip->Color(max_rgb_intensity, max_rgb_intensity/2, max_rgb_intensity/3);

  
  strip->fill(strip->Color(main_rgb_intensity, main_rgb_intensity, main_rgb_intensity)); 
  byte  section_pecent;
  int   section_remaining_time;
  short pulse_location;

  if (seconds_remaining >= PULSE_5B + PULSE_5C + PULSE_5D + PULSE_5E + PULSE_5F) {
    //Pulse A from section 3 to section 2
    section_remaining_time = time_remaining - (PULSE_5B + PULSE_5C + PULSE_5D + PULSE_5E + PULSE_5F) * SECOND_TO_MILLISECOND;
    section_pecent = 100 - section_remaining_time*100 / (PULSE_5A*SECOND_TO_MILLISECOND);
    pulse_location = (section3_len * section_pecent / 100);
    pulse_color = strip->Color(max_rgb_intensity, max_rgb_intensity/2, max_rgb_intensity/30);
    
  } else if (seconds_remaining >= PULSE_5C + PULSE_5D + PULSE_5E + PULSE_5F) {
    //Pulse B from section 1 to section 2
    section_remaining_time = time_remaining - (PULSE_5C + PULSE_5D + PULSE_5E + PULSE_5F) * SECOND_TO_MILLISECOND;
    section_pecent = 100 - section_remaining_time*100 / (PULSE_5B*SECOND_TO_MILLISECOND);
    pulse_location = section3_len + section2_len - (section2_len * section_pecent / 100);
    pulse_color = strip->Color(max_rgb_intensity/2, max_rgb_intensity, max_rgb_intensity/30);
    
  } else if (seconds_remaining >= PULSE_5D + PULSE_5E + PULSE_5F) {
    //Pulse C from section 2 to section 1
    section_remaining_time = time_remaining - (PULSE_5D + PULSE_5E + PULSE_5F) * SECOND_TO_MILLISECOND;
    section_pecent = 100 - section_remaining_time*100 / (PULSE_5C*SECOND_TO_MILLISECOND);
    pulse_location = section3_len + section2_len + (section1_len * section_pecent / 100);
    pulse_color = strip->Color(max_rgb_intensity/2, max_rgb_intensity/30, max_rgb_intensity);
    
  } else if (seconds_remaining >= PULSE_5E + PULSE_5F) {
    //Pulse D from section 3 to section 1
    section_remaining_time = time_remaining - (PULSE_5E + PULSE_5F) * SECOND_TO_MILLISECOND;
    byte section_pecent = 100 - section_remaining_time*100 / (PULSE_5D*SECOND_TO_MILLISECOND);
    pulse_location = (section3_len + section2_len) * section_pecent / 100;
    pulse_color = strip->Color(max_rgb_intensity/30, max_rgb_intensity/2, max_rgb_intensity);
    
  } else if (seconds_remaining >= PULSE_5F) {
    //Pulse E from section 0 to section 2
    section_remaining_time = time_remaining - PULSE_5F * SECOND_TO_MILLISECOND;
    section_pecent = 100 - section_remaining_time*100 / (PULSE_5E*SECOND_TO_MILLISECOND);
    pulse_location = section3_len + section2_len + section1_len - ((section2_len + section1_len) * section_pecent / 100);
    pulse_color = strip->Color(max_rgb_intensity/30, max_rgb_intensity, max_rgb_intensity/2);
    
  } else {
    //Pulse F from section 3 to section 0
    section_remaining_time = time_remaining;
    section_pecent = 100 - section_remaining_time*100 / (PULSE_5F*SECOND_TO_MILLISECOND);
    pulse_location = (section3_len + section2_len + section1_len) * section_pecent / 100;
    if (section_pecent<30)
      pulse_color = strip->Color(max_rgb_intensity*2, 0, 0);
    else if (section_pecent<60)
      pulse_color = strip->Color(0, max_rgb_intensity*2, max_rgb_intensity/3);
    else 
      pulse_color = strip->Color(0, 0, max_rgb_intensity*2);
    
  } 
  for(short i=pulse_location; i<pulse_location+COLORS_BLOCK_LEN; i++) {
    strip->setPixelColor(i, pulse_color); 
  }

  printFreeMemory(__LINE__);
  strip->show();
}


#define PULSE_6A 5
#define PULSE_6B 2
#define PULSE_6C 3
#define PULSE_6D 3
#define PULSE_6E 8
void show_effect_3(byte strip_num) {
  // Light strip has 3 segments |0       2
  //                              \     / \
  //                               \   /   \
  //                                \ /     \
  //                                 1       3
  //
  // Pulse A from 3 to 2 in 5 seconds with solid color
  // Pulse B from 1 to 0 in 4 seconds with solid color
  // Pulse C from 1 to 2 in 3 seconds with solid color
  // Pulse D from (1 to 0) and (1 to 2) in 4 seconds with ? color
  // Pulse E from (1 to 0) and (1 to 2) and (3 to 2) in 8 seconds with pattern color
  Adafruit_NeoPixel *strip = led_strips[strip_num];
  long time_remaining = led_stop_time[strip_num] - current_time;
  byte seconds_remaining = time_remaining/SECOND_TO_MILLISECOND;
  byte max_rgb_intensity = 300/3;
  byte main_rgb_intensity = 15/3;
  byte section1_len = strip->numPixels()/3;
  byte section2_len = section1_len;
  byte section3_len = section1_len;
  uint32_t pulse_color = strip->Color(max_rgb_intensity, max_rgb_intensity/2, max_rgb_intensity/3);
  
  strip->fill(strip->Color(main_rgb_intensity, main_rgb_intensity, main_rgb_intensity)); 
  byte  section_pecent;
  long   section_remaining_time;
  short pulse_location;
  short pulse_location_2=0;
  short pulse_location_3=0;

  if (seconds_remaining >= PULSE_6B + PULSE_6C + PULSE_6D + PULSE_6E) {
    //Pulse A from 3 to 2 in 5 seconds with solid color
    section_remaining_time = time_remaining - (PULSE_6B + PULSE_6C + PULSE_6D + PULSE_6E) * SECOND_TO_MILLISECOND;
    section_pecent = 100 - section_remaining_time*100 / (PULSE_6A*SECOND_TO_MILLISECOND);
    pulse_location = (section3_len * section_pecent / 100);
    pulse_color = strip->Color(max_rgb_intensity/30, max_rgb_intensity/2, max_rgb_intensity);
    
  } else if (seconds_remaining >= PULSE_6C + PULSE_6D + PULSE_6E) {
    //Pulse B from 1 to 0 in 4 seconds with solid color
    section_remaining_time = time_remaining - (PULSE_6C + PULSE_6D + PULSE_6E) * SECOND_TO_MILLISECOND;
    section_pecent = 100 - section_remaining_time*100 / (PULSE_6B*SECOND_TO_MILLISECOND);
    pulse_location = section3_len + section2_len + (section2_len * section_pecent / 100);
    pulse_color = strip->Color(max_rgb_intensity/2, max_rgb_intensity/30, max_rgb_intensity);
    
  } else if (seconds_remaining >= PULSE_6D + PULSE_6E) {
    //C from 1 to 2 in 3 seconds with solid color
    section_remaining_time = time_remaining - (PULSE_6D + PULSE_6E) * SECOND_TO_MILLISECOND;
    section_pecent = 100 - section_remaining_time*100 / (PULSE_6C*SECOND_TO_MILLISECOND);
    pulse_location = section3_len + section2_len - (section1_len * section_pecent / 100);
    pulse_color = strip->Color(max_rgb_intensity/10, max_rgb_intensity/10, max_rgb_intensity);
    
  } else if (seconds_remaining >= PULSE_6E) {
    //Pulse D from (1 to 0) and (1 to 2)) and (3 to 2) in 4 seconds with bright color (multiple fast pulses)
    section_remaining_time = time_remaining - (PULSE_6E) * SECOND_TO_MILLISECOND;
    byte section_pecent = 100 - section_remaining_time*100 / (PULSE_6D*SECOND_TO_MILLISECOND);
    section_pecent = (section_pecent * 5) % 100; // go 5 times faster
    pulse_location = section3_len + section2_len + (section2_len * section_pecent / 100);    
    pulse_location_2 = section3_len + section2_len - (section1_len * section_pecent / 100); 
    pulse_location_2 = section3_len + section2_len - (section1_len * section_pecent / 100);
    pulse_location_3 = (section3_len * section_pecent / 100);
    pulse_color = strip->Color(max_rgb_intensity/5, max_rgb_intensity/5, max_rgb_intensity/5);
    
  } else {
    //Pulse E from (1 to 0) and (1 to 2) and (3 to 2) in 8 seconds with pattern color
    section_remaining_time = time_remaining;
    section_pecent = 100 - section_remaining_time*100 / (PULSE_6E*SECOND_TO_MILLISECOND);
    pulse_location = section3_len + section2_len + (section2_len * section_pecent / 100);
    pulse_location_2 = section3_len + section2_len - (section1_len * section_pecent / 100);
    pulse_location_3 = (section3_len * section_pecent / 100);
    pulse_color =0;    
  } 

  if (pulse_color == 0 ){    
    for(short i=0; i<COLORS_BLOCK_LEN; i++) {
      pulse_color = colors[i/SAME_COLORS_LEN % NUM_COLORS];
      strip->setPixelColor(pulse_location_3 + i, pulse_color); 
      strip->setPixelColor(pulse_location_2 - i + (COLORS_BLOCK_LEN/2), pulse_color); //a little ahead of the other colors
      strip->setPixelColor(pulse_location + i, pulse_color); 
    }
  } else {
    for(short i=0; i<+COLORS_BLOCK_LEN; i++) {
      strip->setPixelColor(pulse_location + i, pulse_color); 
      if (pulse_location_2 >0 )  strip->setPixelColor(pulse_location_2 + i, pulse_color); 
      if (pulse_location_3 >0 )  strip->setPixelColor(pulse_location_3 + i, pulse_color); 
    }  
  }
  strip->show();
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



void dectivate_strip(byte strip) {
  led_strips[strip]->setPin(LEDSTRIPNone);
  if (led_strips[strip]->numPixels() >1) {
    led_strips[strip]->updateLength(1);
  }  
}
void activate_strip(byte strip) {
  if (led_strips[strip]->numPixels() != led_strip_length[strip]) {
    led_strips[strip]->updateLength(led_strip_length[strip]);
  }  
  led_strips[strip]->setPin(led_strip_location[strip]);
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
