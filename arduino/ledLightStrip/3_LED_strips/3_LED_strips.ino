
#include <Adafruit_NeoPixel.h>
#define LEDPIN1 13 //40 led matix
#define LEDPIN2 3  //2 x 8 pin strips
#define LEDPIN3 4  //90 pin strip
#define BUTTONPIN 7  //90 big red button
#define BUTTONLEDPIN 12  //90 pin strip
#define N_LEDS1 40
#define N_LEDS2 16
#define N_LEDS3 90
Adafruit_NeoPixel strip1 = Adafruit_NeoPixel(N_LEDS1, LEDPIN1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip2 = Adafruit_NeoPixel(N_LEDS2, LEDPIN2, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip3 = Adafruit_NeoPixel(N_LEDS3, LEDPIN3, NEO_GRB + NEO_KHZ800);

#define MAX_INT_16 32000
#define COLORS_BLOCK_LEN 10
#define SAME_COLORS_LEN 2
#define NUM_COLORS 9
  uint32_t colors[NUM_COLORS] = {
    strip1.Color(4, 4, 4),
    strip1.Color(12, 0, 0),
    strip1.Color(0, 12, 0),
    strip1.Color(0, 0, 12),
    strip1.Color(6, 6, 0),
    strip1.Color(0, 6, 6),
    strip1.Color(6, 0, 6),
    strip1.Color(8, 4, 0),
    strip1.Color(0, 4, 8)
  };
  uint32_t white = strip1.Color(2, 2, 2);
  uint32_t black = strip1.Color(0, 0, 0);
  
void setup() {
  pinMode(BUTTONLEDPIN, OUTPUT);
  pinMode(BUTTONPIN, INPUT);
  strip1.begin();
  strip2.begin();
  strip3.begin();
}
void loop() {
  chase();
}
static void chase() {
  for(uint16_t i=COLORS_BLOCK_LEN-1; i<MAX_INT_16; i++) {
    uint32_t highlight_color = black;
    uint32_t erase_color = black;
    choose_colors(i, highlight_color, erase_color);
    update_strip(strip1, i, highlight_color, erase_color);
    update_strip(strip2, i, highlight_color, erase_color);
    update_strip(strip3, i, highlight_color, erase_color);
    delay(25);
  }
}
static int choose_colors(uint16_t i, uint32_t &highlight_color, uint32_t &erase_color) {
    if (is_button_pressed()) {
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
static bool is_button_pressed(){
    int button_voltage = digitalRead(BUTTONPIN);
    if (HIGH == button_voltage) 
      return true;
    return false;
}
static int update_strip(Adafruit_NeoPixel &strip, int led_position, 
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
}
