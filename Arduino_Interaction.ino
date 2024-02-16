const int upPin = 2; // Button Up Joystick 2 pin
const int downPin = 3; // Button Down Joystick 3 pin
const int leftPin = 4; // Button Left Joystick 4 pin 
const int rightPin = 5; // Button Right Joystick 5 pin
const int selectPin = 6; // Button Select 6 pin
const int startPin = 7; // Button Start 7 pin
const int instructionPin = 8; // Button Instructions 8 pin
const int rRGB = 9; // Red part of RBG LED 9 pin
const int gRGB = 10; // Green part of RBG LED 10 pin
const int bRGB = 11; // Blue part of RBG LED 11 pin

const unsigned int MAX_MESSAGE_LENGTH = 2; // The lenght is only 2 since the first space is to singal the joystick movement and the second space is for the rest of the buttons
static char message[MAX_MESSAGE_LENGTH] = {'0','0'}; // Create a place to send the message with the states from the buttons

//Initilizating all the State integers
int upState;
int downState;
int leftState;
int rightState;
int startState;
int selectState;
int instructionState;
int redLevel;
int greenLevel;

void setup() {
  // Setting up all the pin numbers as an input reference
  pinMode(upPin, INPUT); 
  pinMode(downPin, INPUT);
  pinMode(leftPin, INPUT);
  pinMode(rightPin, INPUT);
  pinMode(selectPin, INPUT);
  pinMode(startPin, INPUT);
  pinMode(instructionPin, INPUT);
  pinMode(rRGB, OUTPUT); // Set up the red part of RGB LED pin to be an output:
  pinMode(gRGB, OUTPUT); // Set up the green part of RGB LED  pin to be an output:
  pinMode(bRGB, OUTPUT); // Set up the blue part of RGB LED pin to be an output:
  Serial.begin(9600); // Starts the serial screen
  // Start the RGB Color in a middle mix between red and green
  redLevel = 135; 
  greenLevel = 135;
  analogWrite(rRGB, redLevel); //Send the color level to the RGB LED
  analogWrite(gRGB, greenLevel);
}

void loop() {
  // Reading the signal and turning that into a LOW or HIGH int 
  upState = digitalRead(upPin);
  downState = digitalRead(downPin);
  leftState = digitalRead(leftPin);
  rightState = digitalRead(rightPin);
  selectState = digitalRead(selectPin);
  startState = digitalRead(startPin);
  instructionState = digitalRead(instructionPin);
  // LOW means that the button has been pressed and the numbers or letters will later be recognized by the C++ program to execute changes in the game
  if (upState == LOW){
    message[0] = '3';
  }
  else if (downState == LOW){
    message[0] = '1';
  }
  else if (leftState == LOW){
    message[0] = '2';
  }
  else if (rightState == LOW){
    message[0] = '4';
  }
  if (selectState == LOW){
    message[1] = 'E';
  }
  else if (startState == LOW){
    message[1] = 'S';
  }
  else if (instructionState == LOW){
    message[1] = 'I';
  }
  Serial.println(message); // println is essential in this code to make it much more computational efficient by separating the lines and not creating a limitless message

  for(int i = 0; i < MAX_MESSAGE_LENGTH; i++){ //Reset the message composition to 0 when nothing has been pressed
    message[i] = '0';
  }
 
  // Check to see if anything is available in the serial receive buffer
  if(Serial.available()>0){    
    String info;
    info = Serial.readStringUntil('\n'); //Reads Serial Port and inputs the information into the string
    if(info.equals("X")){ // If the string is 'X' it means that the user has collected unsustainable candies so the lights will turn more red f0r harming the planet
      redLevel = redLevel + 30;
      redLevel = constrain(redLevel, 0, 255);
      greenLevel = greenLevel - 30;
      greenLevel = constrain(greenLevel, 0, 255);
      analogWrite(rRGB, redLevel);
      analogWrite(gRGB, greenLevel);
    }
    else if(info.equals("Y")){ // If the string is 'Y' it means that the user has collected sustainable candies so the lights will turn more green for saving the planet
      redLevel = redLevel - 30;
      redLevel = constrain(redLevel, 0, 255);
      greenLevel = greenLevel + 30;
      greenLevel = constrain(greenLevel, 0, 255);
      analogWrite(rRGB, redLevel);
      analogWrite(gRGB, greenLevel);
    }
  }
  delay(100); // Necessary tested delay to avoid errors with the Serial Port writing and reading
}