//More information about VS1053 - https://www.elecrow.com/wiki/index.php?title=VS1053_MP3_Shield

// include SPI, MP3 and SD libraries
#include <SPI.h>
#include <Adafruit_VS1053.h>
#include <SD.h>

// define the pins used
// These are the pins used for the music maker shield
#define SHIELD_RESET  8      // VS1053 reset pin (unused!)
#define SHIELD_CS     6      // VS1053 chip select pin (output)
#define SHIELD_DCS    7      // VS1053 Data/command select pin (output)
// These are common pins between breakout and shield
#define CARDCS        9     // Card chip select pin
// DREQ should be an Int pin, see http://arduino.cc/en/Reference/attachInterrupt
#define DREQ          2       // VS1053 Data request, ideally an Interrupt pin

// music file locations
#define DIRECTORY_MELLOW String("/NORMAL~1")
#define DIRECTORY_PARTY  String("/DANCEM~1")

#define LIGHT_THRESHOLD  50

Adafruit_VS1053_FilePlayer musicPlayer = Adafruit_VS1053_FilePlayer(SHIELD_RESET, SHIELD_CS, SHIELD_DCS, DREQ, CARDCS);
String musicMode = DIRECTORY_MELLOW;
int mellowTrack = 1;
int partyTrack = 1;
bool moodChanged = true;
  
void setup() {
  Serial.begin(9600);
  logString("Initialising Music Maker");

  if (musicMakerInit()) {
    // list files
    printDirectoryListing(DIRECTORY_MELLOW);
    printDirectoryListing(DIRECTORY_PARTY);
    
    logString("Music maker init success");
  } else {
    logString("Music maker init failed");
  }
}

void loop() {
  // File is playing in the background
  checkLightSensor();
  if (moodChanged || musicPlayer.stopped()) {
    logString("Changing track");
    String song;
    String directory;
    logString("Searching");
    song = getNextSong(musicMode);
    logString(String("Now playing ") + song);
    musicPlayer.startPlayingFile(song.c_str());
    moodChanged=false;
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

  delay(100);
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


void printDirectoryListing(String dir) {
  File entry = SD.open(dir);
  printDirectory(entry,0);
  //entry.close();
}

/// File listing helper
void printDirectory(File dir, int numTabs) {
  logString(String("Scanning directory ") + dir.name());
  while(true) {
     
     File entry =  dir.openNextFile();
     if (! entry) {
       // no more files
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
}


String checkLightSensor() {
  logString("  light");
  int analogValue = 40;//analogRead(A0);
  logString("  light read");
  
  logString("  light = " + analogValue);
  String mode = DIRECTORY_PARTY;
  if (analogValue<LIGHT_THRESHOLD)  mode = DIRECTORY_MELLOW;
  if (mode != musicMode) {
    moodChanged = true;
    musicMode = mode;
  }
}


String getNextSong(String dir) {
      logString(String("getNextSong ") + dir);
  int trackNumber = 0;
  if (DIRECTORY_MELLOW==musicMode){
    trackNumber = mellowTrack++;
  } else {
    trackNumber = partyTrack++;        
  }
  
      logString(String("track number ") + trackNumber);
  
  String song = getSong(musicMode, trackNumber);  
  if (song.length()>0) return musicMode + "/" + song;
  
  if (DIRECTORY_MELLOW==musicMode){
    mellowTrack = 1;
  } else {
    partyTrack = 1;        
  }
  return musicMode + "/" + getSong(musicMode, 1);
}

/// File listing helper
String getSong(String dir, int trackNumber) {
  String trackName = "";
      logString(String("querying") + dir);
  File directory(SD.open(dir));
      logString(String("opened directory") + dir);
  while(true) {
   
      logString("looping");
    File entry =  directory.openNextFile();
    if (! entry) {
      // no more files
      logString(String("no more files to play from directory") + dir);
      break;
    }
    trackName = entry.name();
    entry.close();
    trackNumber = trackNumber -1;
    if (0 == trackNumber) break;
    
    logString("skipping track " + trackName + " (" + trackNumber + ")");
  }   
  //directory.close();
  logString("returning track " + trackName);
  return trackName;
}

void logString(String text){
    //Serial.println(F(text.c_str()));
    Serial.println(text);
    return;
}
