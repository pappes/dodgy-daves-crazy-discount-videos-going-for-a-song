/*************************************************** 
  This is an example for the Adafruit VS1053 Codec Breakout
  Designed specifically to work with the Adafruit VS1053 Codec Breakout 
  ----> https://www.adafruit.com/products/1381
  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!
  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ****************************************************/

// include SPI, MP3 and SD libraries
#include <SPI.h>
#include <Adafruit_VS1053.h>
#include <SD.h>
#include <Adafruit_NeoPixel.h>

// define the pins used for the musicmaker shield
#define SHIELD_RESET  8      // VS1053 reset pin (unused!)
#define SHIELD_CS     6      // VS1053 chip select pin (output)
#define SHIELD_DCS    7      // VS1053 Data/command select pin (output)
#define CARDCS        9     // Card chip select pin
// DREQ should be an Int pin, see http://arduino.cc/en/Reference/attachInterrupt
#define DREQ          2       // VS1053 Data request, ideally an Interrupt pin


#define LEDSTRIPPIN 4
#define LEDSHIELDPIN 5

// music file locations
#define DIRECTORY_MELLOW String("/NORMAL~1")
#define DIRECTORY_PARTY  String("/DANCEM~1")
#define SONG_1 "/NORMAL~1/01AIR-~1.MP3"
#define SONG_2  "/DANCEM~1/02UTAH~1.MP3"

#define LIGHT_THRESHOLD  20          // 10 is quite dark
#define LIGHT_CHANGE_THRESHOLD  5    //000 // 5 seconds
#define SECOND_TO_MILLISECOND 1000L
#define SAME_COLORS_LEN 4

bool musicModeIsParty = false;
String musicMode = DIRECTORY_MELLOW;
int currentMellowTrack = 1;
int currentPartyTrack = 1;
int qtyMellowTracks = 0;
int qtyPartyTracks = 0;
bool moodChanged = true;
unsigned long moodChangedTime = millis();

long current_time = 0;
short current_cycle_strip_1 = 0;
short current_cycle_strip_2 = 0;

Adafruit_NeoPixel strip1 = Adafruit_NeoPixel(72, LEDSTRIPPIN,  NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip2 = Adafruit_NeoPixel(40, LEDSHIELDPIN, NEO_GRB + NEO_KHZ800);



Adafruit_VS1053_FilePlayer musicPlayer = 
  // create breakout-example object!
  //Adafruit_VS1053_FilePlayer(BREAKOUT_RESET, BREAKOUT_CS, BREAKOUT_DCS, DREQ, CARDCS);
  // create shield-example object!
  Adafruit_VS1053_FilePlayer(SHIELD_RESET, SHIELD_CS, SHIELD_DCS, DREQ, CARDCS);
  
void setup() {
  Serial.begin(9600);
  pinMode(LEDSTRIPPIN, OUTPUT);
  pinMode(LEDSHIELDPIN, OUTPUT);
  strip1.begin();
  strip2.begin();
  
  
  Serial.println(F("Adafruit VS1053 Simple Test"));

  if (musicMakerInit()) {
  
    // list files
    printDirectory(SD.open(DIRECTORY_MELLOW), 0);
    printDirectory(SD.open(DIRECTORY_PARTY), 0);
    Serial.println(F("Music maker init success"));
  } else {
    Serial.println(F("Music maker init failed"));
  }
  

  // Play one file, don't return until complete
 // musicPlayer.playFullFile("/track001.mp3");
  // Play another file in the background, REQUIRES interrupts!
  //musicPlayer.startPlayingFile(SONG_2);  
}

void loop() {
  current_time = millis();
  //if (current_time % 10 == 0) printFreeMemory(__LINE__);
  printFreeMemory(__LINE__);
  dance_floor_effects();
  dance_room_effects();

  
  checkLightSensor();
  
  // File is playing in the background
  if (moodChanged || musicPlayer.stopped()) {
    // Play another file in the background, REQUIRES interrupts
    String song = getNextSong();
    if (!musicPlayer.stopped())  musicPlayer.stopPlaying();
    if (song.length() > 0) {
      musicPlayer.startPlayingFile(song.c_str());
    } else {   
      Serial.print(F("Cant find any music for ")); Serial.println(musicMode);
    }
    moodChanged = false;
  }
  if (Serial.available()) {
    char c = Serial.read();
    
    // if we get an 's' on the serial console, stop!
    if (c == 's') {
      musicPlayer.stopPlaying();
    }
    // if we get an 'm' on the serial console, swith mode
    if (c == 'm') {
      
      String mode;
      if (!musicModeIsParty)  mode = DIRECTORY_PARTY;
      else mode = DIRECTORY_MELLOW;
      musicModeIsParty = ! musicModeIsParty;
      moodChanged = true;    
      moodChangedTime = millis();
      musicMode = mode;
    }
    
    // if we get an 'p' on the serial console, pause/unpause!
    if (c == 'p') {
      if (! musicPlayer.paused()) {
        Serial.println(F("Paused"));
        musicPlayer.pausePlaying(true);
      } else { 
        Serial.println(F("Resumed"));
        musicPlayer.pausePlaying(false);
      }
    }
  }

  delay(10);
}

String getNextSong() {
  Serial.print(F("getNextSong for mood ")); Serial.println(musicMode);
  int trackNumber;
  if (DIRECTORY_MELLOW==musicMode) trackNumber = currentMellowTrack++;
  else trackNumber = currentPartyTrack++;        
  
  Serial.print(F("searching for track number ")); Serial.println(trackNumber);
  String song = getSong(musicMode, trackNumber);  
  if (song.length()>0) return musicMode + "/" + song;
  
  // no track returned
  if (1 == trackNumber) return "";
  if (DIRECTORY_MELLOW==musicMode) currentMellowTrack = 1;
  else currentPartyTrack = 1;   
  return getNextSong();
}



bool musicMakerInit() {

  if (! musicPlayer.begin()) { // initialise the music player
     Serial.println(F("Couldn't find VS1053, do you have the right pins defined?"));
     return false;
  }
  Serial.println(F("VS1053 found"));
  
  if (!SD.begin(CARDCS)) {
    Serial.println(F("SD failed, or not present"));
    return false;
  }
  
  // Set volume for left, right channels. lower numbers == louder volume!
  musicPlayer.setVolume(20,20);

  // If DREQ is on an interrupt pin (on uno, #2 or #3) we can do background audio playing
  musicPlayer.useInterrupt(VS1053_FILEPLAYER_PIN_INT);  // DREQ int
  return true;
}


void checkLightSensor() {
  if (millis() < moodChangedTime+LIGHT_CHANGE_THRESHOLD) return;
  int analogValue = analogRead(A0);  
  Serial.print(F("light intensity = ")); Serial.println(analogValue);
  
  String mode;
  if (analogValue<LIGHT_THRESHOLD)  mode = DIRECTORY_MELLOW;
  else mode = DIRECTORY_PARTY;
  if (mode != musicMode) {
    Serial.print(F("switching mood to ")); Serial.println(mode);
    moodChanged = true;    
    moodChangedTime = millis();
    musicMode = mode;
  }
}



/// File listing helper
void printDirectory(File dir, int numTabs) {
  Serial.print(F("Scanning directory ")); Serial.println(dir.name());
   while(true) {
     
     File entry =  dir.openNextFile();
     if (! entry) {
       Serial.print(F("Directory scan completed"));
       break;
     }
     for (uint8_t i=0; i<numTabs; i++) {
       Serial.print('\t');
     }
     Serial.print(entry.name());
     if (entry.isDirectory()) {
       Serial.println("/");
       printDirectory(entry, numTabs+1);
     } else {
       // files have sizes, directories do not
       Serial.print("\t\t");
       Serial.println(entry.size(), DEC);
     }
     entry.close();
   }
   if (0 == numTabs) dir.close();
}

/// File listing helper
String getSong(String dir, int trackNumber) {
  Serial.print(F("querying ")); Serial.println(dir);
  String trackName = "";
  File directory(SD.open(dir));
  Serial.print(F("opened directory ")); Serial.println(dir);
  while(true) {   
    File entry =  directory.openNextFile();
    if (! entry) {
      Serial.print(F("no more files to play from directory")); Serial.println(dir);
      trackName = "";
      break;
    }
    trackName = String(entry.name());
    entry.close();
    trackNumber = trackNumber -1;
    if (0 == trackNumber) break;
    
    Serial.print(F("skipping track ")); Serial.print(trackName);Serial.print(F("(")); Serial.print(trackNumber);Serial.println(F(")"));
  }   
  directory.close();
  Serial.print(F("returning track ")); Serial.println(trackName);
  return trackName;
}





uint32_t white = strip2.Color(25, 25, 25);
uint32_t superwhite = strip2.Color(250, 250, 250);
uint32_t black = strip2.Color(0, 0, 0);
uint32_t green = strip2.Color(0, 25, 0);
uint32_t red = strip2.Color(25, 0, 0);
uint32_t blue = strip2.Color(0, 0, 25);

#define NUM_COLORS_STRIP_1 3
uint32_t strip_1_colors[NUM_COLORS_STRIP_1] = {
    red,
    green,
    blue,
};

static void dance_room_effects() {
 
  short new_cycle = current_time/(SECOND_TO_MILLISECOND/5);
  if (current_cycle_strip_1 != new_cycle){
    current_cycle_strip_1 = new_cycle;
    Serial.println(new_cycle);
    for(short i=0; i<strip1.numPixels(); i++) {
      byte color_index = (new_cycle+i)/SAME_COLORS_LEN%NUM_COLORS_STRIP_1;
      strip1.setPixelColor(i , strip_1_colors[color_index]); // Draw new pixel
    }
    strip1.show();
  }
}

#define NUM_COLORS_STRIP_2 7
uint32_t strip_2_colors[NUM_COLORS_STRIP_2] = {
    green,
    red,
    green,
    red,
    blue,
    green,
    red,
};

static void dance_floor_effects() {
 short new_cycle = current_time/SECOND_TO_MILLISECOND;
  if (current_cycle_strip_2 != new_cycle){
    current_cycle_strip_2 = new_cycle;
    Serial.println(new_cycle);
    for(short i=0; i<strip2.numPixels(); i++) {
      byte color_index = (new_cycle+i/* + i/8*/)%NUM_COLORS_STRIP_2 ;
      strip2.setPixelColor(i , strip_2_colors[color_index]); // Draw new pixel
    }
    strip2.show();
  }
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
