/*Creation crate memory game
Match the pattern of the LEDs
Each time you get the pattern right, it gets longer and faster
The game resets when you make a mistake

Step 1: Create contant variables
Constant variables have static values.  Once you set their values, you can't change them */

//These values are the pins on the UNO R3 that the buttons connect to
const int buttonYellow = 2;
const int buttonGreen = 3;
const int buttonRed = 4;
const int buttonBlue = 5;
const int numberOfButtons = 4;

//these values are the pins on the Uno R3 that the LEDs connect to
const int ledAll = 0;
const int ledYellow = 7;
const int ledGreen = 8;
const int ledRed = 9;
const int ledBlue = 10;

//This is the buzzer to UNO R3 connection
const int buzzer = 12;

//These are the tone used for the buzzer unig Hertzs (Hz).
const int tones[] = {1900, 1600, 1300, 1000, 3200};

//step2: create user define variables
int buttonState[] = {0,0,0,0};
int lastButtonState[] = {0,0,0,0};
int buttonCounter[] = {0,0,0,0};
int gameOn = 0; //A new game or level starts when  GameOn is 0
int wait = 0; //this is used to tell the computer to wait until the user has pressed a button
int currentLevel = 1; //This id the current game level and the number of button presses to make it to the next level
int delayInterval = 500; //This is the ammount of time to wait for the next button press  (0.5 seconds)
int ledTime = 500; //This is the amount of time each LED will flash when a corresponding button is pressed(0.5 seconds) 
int nLevels = 10; //This is the number of levels until the game is won - at which poibnt the the game will get faster 
int pinAndTone = 0; //this is the variable that will be used to determine which LED to turn on and it's corresponding buzzer tone 
int correctAnswer = 0; //this value must become 1 to get to the next level. 
int speedFactor = 5; //This is the speed of the game.  It increases every time a level is beaten 
int ledDelay = 200; //This is the amount of time taken before the next LED pattern lights up (0.2 seconds).  This will decrease every time a level is beaten.  

//Step 3: create a funtion to play tones on the buzzer 
 
 void playTone(int tone, int duration) {
     for (long i=0; i<duration*1000L; i+=tone*2)
     {
         digitalWrite(buzzer, HIGH);//Turns the buzzer on
         delayMicroseconds(tone); //Creates the tone on the buzzer 
         digitalWrite(buzzer, LOW); //turns the buzzer off
         delayMicroseconds(tone);
     }
 } //end of playTune

 void victorySong() {
    int notes[] = {2,2,2,2,0,1,2,1,2};
    int note=0;
    int tempo[]={200,200,200,400,400,400,200,200,600};
    int breaks[]={100,100,100,200,200,200,300,100,200};
    for (int i=0; i<9; i=i+1){
        note=notes[i];
        brightBuzz(ledYellow+note, tones[note],tempo[i]);
        delay(breaks[i]);
    }
 } //end of victorySong

 void brightBuzz(int led, int tone, int duration) {
     if (led==ledAll) {
         //Turn all LEDs on
        for (int i=0; i<numberOfButtons; i=i+1){
            digitalWrite(ledYellow+i, HIGH);
        }
        playTone(tone,duration);
        for (int i=0; i<numberOfButtons; i=i+1){
            digitalWrite(ledYellow+i, LOW);
        }
     } else {
         //Turn nominated LED on
        digitalWrite(led, HIGH);
        playTone(tone,duration);
        digitalWrite(led, LOW);

     }
 } //end of brightBuzz

 void showMistake() {
    //These loops make the LEDs blink twice and the buzzer beep twice when the user makes a mistake and resets the game 
    brightBuzz(ledAll, tones[4], ledTime);
    delay(200);
    brightBuzz(ledAll, tones[4], ledTime);
    delay(500);
 } //end of showMistake

 void showPattern(int *pattern) {
    for (int i=0; i<currentLevel; i=i+1) { //this for loop shows the user the curent state of the game 
        ledDelay = ledTime/(1+(speedFactor/nLevels) * (currentLevel-1));
        pinAndTone = pattern[i];
        brightBuzz(ledYellow+pinAndTone, tones[pinAndTone], ledDelay);
        delay( 100/speedFactor);
    }
 } //end of showPattern

 int getButtonPress() {
    while (true){
        for (int i=0; i<numberOfButtons; i=i+1) {
            if (digitalRead(i+buttonYellow) != 0) {
                return (i);
            }
        } 
    }
 } //end of getButtonPress

 int initLevel(int *gamePattern, int *userPattern) {
    for (int i=0; i<nLevels; i=i+1){
        //saves the number in nArray to generate a random pattern
        userPattern[i] = 0;
        gamePattern[i] = random(0,4);
    }
 } //end of initLevel

 //step 4: Initalise the inputs and outputs
 //Here, We're telling the UNO that buttons are inputs and LEDs and the buzzer are outputs
 void setup() {
     randomSeed(analogRead(0)); //used to generate random numbers

     //initialise inputs 
     pinMode(buttonYellow, INPUT);
     pinMode(buttonGreen, INPUT);
     pinMode(buttonRed, INPUT);
     pinMode(buttonBlue, INPUT);

     //initialise outputs 
     pinMode(ledYellow, OUTPUT);
     pinMode(ledGreen, OUTPUT);
     pinMode(ledRed, OUTPUT);
     pinMode(ledBlue, OUTPUT);
     pinMode(buzzer, OUTPUT);
} //end of setup 

//Step 5: Create the game loop
//This is where the game starts! 
void loop() {
    int nArray[nLevels]; //nArray will store the randomised game pattern 
    int uArray[nLevels]; //uArray will store the pattern input byu the user 

    if (gameOn == 0) {//Only triggers if it's a new game or level 
        initLevel(nArray, uArray);
        gameOn = 1; //this tells the game to run
    }    
    if (wait == 0) { //triggers if no action is required from the user 
        delay(200);
        showPattern(nArray);
        wait = 1; //This puts the game on hold until the user enteres a pattern
    }

    int buttonPressed=0; 
    int patternPosition=0; //'patternPosition' is the current position in the pattern
    while (patternPosition<currentLevel)  {
        buttonPressed=getButtonPress();
        //this turns on the corresponding LED to the button pressed, and calls the function "playTone" to  play the corresponding sound on the buzzer 
        brightBuzz(ledYellow+buttonPressed, tones[buttonPressed], ledTime);
        wait=0;
        uArray[patternPosition]=buttonPressed; //This stores the users input to be matched against the game pattern
                
        //this section checks if the button pressed by the user matches the game pattern
        if (uArray[patternPosition]==nArray[patternPosition]){
            correctAnswer=1;
            patternPosition++;
        } else {
            //if user makes a mistake, these variables will be reset so that the game starts over.
            correctAnswer=0;
            delay(300);
            gameOn = 0;
            currentLevel = 1;
            showMistake;
            break;
        }
    }

    //if the user gets the sequence right the game goes up one level.
    if (correctAnswer==1){
        currentLevel++;
    }

    //This section play the victory song if the game is beaten 
    if (currentLevel == nLevels){
        delay(500);
        victorySong();

        gameOn=0;
        currentLevel=1;
        nLevels=nLevels+2; //this adds two more levels to the game.
        speedFactor +=1; //This increases the speed factor by 1.
    }
}//end of void loop 








