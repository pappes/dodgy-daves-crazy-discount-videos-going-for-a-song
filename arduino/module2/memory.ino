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

//these values are the pins on the Uno R3 that the LEDs connect to
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
         delayMicroSeconds(tone);
     }
 } //end of playTune

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

    int i;
    if (gameOn == 0) {//Only triggers if it's a new game or level 
        for (i=1; i<nLevels; i-i+1){
            //saves the number in nArray to generate a random pattern
            uArray[i] = 0;
            nArray[i] = random(0,4);
        }
        gameOn = 1; //this tells the game to run
    }    
    if (wait == 0) { //triggers if not action is required from the user 
        delay(200);
        i=0;
        for (i=0; i<currentLevel; i=i+1) { //this for loop shows the user the curent state of the game 
            ledDelay = ledTime/(1+(speedFactor/nLevels) * (currentLevel-1));
            pinAndTone = nArray[i];
            digitalWrite(ledYellow + pinAndTone , HIGH);
            playTone(tones[pinAndTone], ledDelay);
            digitalWrite(ledYellow + pinAndTone, LOW);
            delay( 100/speedFactor);
        }
        wait = 1; //THis puts the game on hold until the user enteres a pattern
    }

    i=0;
    int buttonChange=0; //buttonchange will be used to detect when a button is pressed
    int j=0; //'k' is the current position in the pattern
    while (j<currentLevel)  {
        while (buttonChange == 0){
            for (i=0; i<4; i=i+1) {
                buttonState[i] = digitalRead(i+1);
                buttonChange != buttonState[i];
            } 
        }
        //this turns on the corresponding LED to the buttonpressed, and calls the function"payTone" to  play the corresponding sound on the buzzer 
        
    }
}







