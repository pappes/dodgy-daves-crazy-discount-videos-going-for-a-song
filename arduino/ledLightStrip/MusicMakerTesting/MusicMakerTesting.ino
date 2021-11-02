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

// define the pins used for the musicmaker shield
#define SHIELD_RESET  8      // VS1053 reset pin (unused!)
#define SHIELD_CS     6      // VS1053 chip select pin (output)
#define SHIELD_DCS    7      // VS1053 Data/command select pin (output)
#define CARDCS        9     // Card chip select pin
// DREQ should be an Int pin, see http://arduino.cc/en/Reference/attachInterrupt
#define DREQ          2       // VS1053 Data request, ideally an Interrupt pin

// music file locations
#define DIRECTORY_MELLOW String("/NORMAL~1")
#define DIRECTORY_PARTY  String("/DANCEM~1")
#define SONG_1 "/NORMAL~1/01AIR-~1.MP3"
#define SONG_2  "/DANCEM~1/02UTAH~1.MP3"

#define LIGHT_THRESHOLD  10          // 10 is quite dark
#define LIGHT_CHANGE_THRESHOLD  5000 // 5 seconds

String musicMode = DIRECTORY_MELLOW;
int currentMellowTrack = 1;
int currentPartyTrack = 1;
int qtyMellowTracks = 0;
int qtyPartyTracks = 0;
bool moodChanged = true;
unsigned long moodChangedTime = millis();


Adafruit_VS1053_FilePlayer musicPlayer = 
  // create breakout-example object!
  //Adafruit_VS1053_FilePlayer(BREAKOUT_RESET, BREAKOUT_CS, BREAKOUT_DCS, DREQ, CARDCS);
  // create shield-example object!
  Adafruit_VS1053_FilePlayer(SHIELD_RESET, SHIELD_CS, SHIELD_DCS, DREQ, CARDCS);
  
void setup() {
  Serial.begin(9600);
  Serial.println("Adafruit VS1053 Simple Test");

  if (musicMakerInit()) {
  
    // list files
    printDirectory(SD.open(DIRECTORY_MELLOW), 0);
    printDirectory(SD.open(DIRECTORY_PARTY), 0);
    logString("Music maker init success");
  } else {
    logString("Music maker init failed");
  }
  

  // Play one file, don't return until complete
  Serial.println(F("Playing track 001"));
 // musicPlayer.playFullFile("/track001.mp3");
  // Play another file in the background, REQUIRES interrupts!
  Serial.println(F("Playing track 002"));
  //musicPlayer.startPlayingFile(SONG_2);
}

void loop() {

  
  checkLightSensor();

  
  // File is playing in the background
  if (moodChanged || musicPlayer.stopped()) {
    // Play another file in the background, REQUIRES interrupts
    String song = getNextSong();
    if (song.length() > 0) {
      musicPlayer.startPlayingFile(song.c_str());
    } else {   
      logString(String("Cant find any music for ") + musicMode);
    }
    moodChanged = false;
  }
  if (Serial.available()) {
    char c = Serial.read();
    
    // if we get an 's' on the serial console, stop!
    if (c == 's') {
      musicPlayer.stopPlaying();
    }
    
    // if we get an 'p' on the serial console, pause/unpause!
    if (c == 'p') {
      if (! musicPlayer.paused()) {
        logString("Paused");
        musicPlayer.pausePlaying(true);
      } else { 
        logString("Resumed");
        musicPlayer.pausePlaying(false);
      }
    }
  }

  delay(1000);
}

String getNextSong() {
  logString(String("getNextSong for mood ") + musicMode);
  int trackNumber;
  if (DIRECTORY_MELLOW==musicMode) trackNumber = currentMellowTrack++;
  else trackNumber = currentPartyTrack++;        
  
  logString(String("searching for track number ") + trackNumber);
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
     logString("Couldn't find VS1053, do you have the right pins defined?");
     return false;
  }
  logString("VS1053 found");
  
  if (!SD.begin(CARDCS)) {
    logString("SD failed, or not present");
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
  logString(String("light intensity = ") + analogValue);
  
  String mode;
  if (analogValue<LIGHT_THRESHOLD)  mode = DIRECTORY_MELLOW;
  else mode = DIRECTORY_PARTY;
  if (mode != musicMode) {
    logString(String("switching mood to  ") + mode);
    moodChanged = true;    
    moodChangedTime = millis();
    musicMode = mode;
  }
}



/// File listing helper
void printDirectory(File dir, int numTabs) {
  logString(String("Scanning directory ") + dir.name());
   while(true) {
     
     File entry =  dir.openNextFile();
     if (! entry) {
       logString("Directory scan completed");
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
  logString(String("querying") + dir);
  String trackName = "";
  File directory(SD.open(dir));
  logString(String("opened directory") + dir);
  while(true) {   
    File entry =  directory.openNextFile();
    if (! entry) {
      logString(String("no more files to play from directory") + dir);
      trackName = "";
      break;
    }
    trackName = String(entry.name());
    entry.close();
    trackNumber = trackNumber -1;
    if (0 == trackNumber) break;
    
    logString(String("skipping track ") + trackName + " (" + trackNumber + ")");
  }   
  directory.close();
  logString("returning track " + trackName);
  return trackName;
}


void logString(String text){
    Serial.println(text);
    return;
}
