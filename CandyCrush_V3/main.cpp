#include <SFML/Graphics.hpp>
#include <ctime>
#include <iostream>
#include <random>
#include <string>
#include <vector>
#include <fstream>
#include "SerialPort.h"
#include "Button.h"
#include "Frame.h"
#include "Board.h"

#define SCORE_POSITIVE 2
#define SCORE_NEGATIVE 1
#define SLEEP_TIME 250
#define OUTPUT_FILE_NAME "results.txt"
#define NUMBER_QUESTIONS 2

using namespace sf;
using namespace std;

char *port = "\\\\.\\COM4"; // change the name of the port with the port name of your computer, must remember that the backslashes are essential so do not remove them
char incomingData[MAX_DATA_LENGTH]; // character array that will receive the input data from the Serial Port
char *out = NULL; // character pointer that will print information into the Serial Port so that the Arduino code can analyse it
SerialPort arduino(port);
void connectingArduino();
void readArduino();
void resetButtons();

Button selectionButton;
Button startButton;
Button instructionsButton;
Frame visible; // First green frame that allows the user to lock the first swap position
Frame invisible; // Red frame that locks the second swap position
// Positions found from test and failure
Board Score(230,245);
Board Moves(230,470);
Board Result(940,440);

int ts = 120; //tile size (size of separation of the quadrants in which the candies are located), helps with the references of x and y coordinates to print in the window
Vector2i offset(840,60); //Class for manipulating 2-dimensional vectors (centers the position of the candy's grid)

// Structures is a way to group several related variables into one place. Unlike an array it can contain many different data types. "Piece" is now a data type.
struct piece
{ int x,y,col,row,kind,match,alpha; // Row and col gives the position in the grid of each candy, alpha refers to the transparency of the pieces, kind represents the different types of candies, x and y represent the coordinates to print in the window
    piece(){match=0; alpha=255;}
} grid[10][10]; // Creates a piece structure and stores it in grid[x][y] variable which is going to store the candies in each row-column position

void swap(piece p1,piece p2); //Function that allows to swap positions of the candies in the grid

int main()
{
    connectingArduino();
    //Time srand allows us to get random numbers to show random types of candies falling down when candies are collected
    srand(time(0));

    //Allows to open the window where the game is played
    RenderWindow app(VideoMode(1920, 1080), "Candy Crush Game!", Style::Fullscreen);
    app.setFramerateLimit(60); //Sets the FPS limit for a fluent game

    //Defines the textures needed for the game (images living on the graphics card that can be used for drawing)
    Texture t1, t2, t3, t4, t5, t6, t7, t8, q1, q2, q3, q4, q5, q6, q7, q8, q9, q10, q11;
    t1.loadFromFile("images/backgroundscreen.png"); //Stores background texture into SFML
    t2.loadFromFile("images/Candyland_candy.png"); //Stores candies texture into SMFL
    t3.loadFromFile("images/Candyland_cursor_green.png");
    t4.loadFromFile("images/Candyland_cursor2.png");
    t5.loadFromFile("images/startscreen.png");
    t6.loadFromFile("images/videoscreen.png");
    t7.loadFromFile("images/instructionscreen.png");
    t8.loadFromFile("images/finalscreen.png");
    q1.loadFromFile("images/Question1.jpg");
    q2.loadFromFile("images/Question2.jpg");
    q3.loadFromFile("images/Question3.jpg");
    q4.loadFromFile("images/Question4.png");
    q5.loadFromFile("images/Question5.jpg");
    q6.loadFromFile("images/Question6.png");
    q7.loadFromFile("images/Question7.png");
    q8.loadFromFile("images/Question8.png");
    q9.loadFromFile("images/Question9.jpg");
    q10.loadFromFile("images/Question10.jpg");
    q11.loadFromFile("images/Question11.jpg");

    //Creates the drawable representation of a texture, with its own transformations, color, etc
    Sprite background(t1), gems(t2), greenbox(t3), redbox(t4), startScreen(t5), videoScreen(t6), instructionsScreen (t7), finalScreen (t8), question1(q1), question2(q2), question3(q3), question4(q4), question5(q5), question6(q6), question7(q7), question8(q8), question9(q9), question10(q10), question11(q11);
    Sprite questions[] = {question1,question2,question3,question4,question5,question6,question7,question8,question9,question10,question11};
    //Sets the types of candies per grid position
    //Nested loop that goes through all the grid positions, the maximum for i and j is 8 because the candies grid is 8x8
    for (int i = 1; i <= 8; i++)
        for (int j = 1; j <= 8; j++) {
            grid[i][j].kind = rand() % 6; //Determines randomly (with srand time) the type of candy
            grid[i][j].col = j; //Sets the col position of the candy
            grid[i][j].row = i; //Sets the row position of the candy
            grid[i][j].x = j * ts; //Represents the x offset ot be printed in the window
            grid[i][j].y = i * ts; //Represents the y offset ot be printed in the window
        }

    //Defines variables used for position, mouse click, swapping and moving
    int x0, y0, x, y;
    int click = 0;
    int questionCounter = 0;
    Vector2i pos;
    ofstream outputFile;
    string outputName;
    bool isSwap = false, isMoving = false;
    bool isQuestion = false, isVideo = false, isInstruction = false, isFinal = false, isGame = false;
    bool justPressed = false;
    Moves.value = 3; // Initialize quantity of moves at the very beginning
    vector<int> questionNumbers;
    for (int i = 0; i < 11; ++i) // This script allows us to create a vect0r with numbers from 0 to 11 in a random order thaks to shuffle
        questionNumbers.push_back(i);
    shuffle(questionNumbers.begin(), questionNumbers.end(), std::mt19937(std::random_device()()));

    while (app.isOpen()){ // Makes sure that the program while only run when the window opened successfully
        readArduino();
        if (justPressed){ // Recognize if a button has just been presed and reset the read if it has in order to creaty a certain delay that one button press would not trigger more than 1 equal reaction
            resetButtons();
        }
        out = NULL; //Reset out value to NULL to avoid changing the lights when the score has not changed
        bool questionAnswers[11][3] = {startButton.startPressed, selectionButton.selectPressed, instructionsButton.instructionsPressed, instructionsButton.instructionsPressed, startButton.startPressed, selectionButton.selectPressed, instructionsButton.instructionsPressed, selectionButton.selectPressed, startButton.startPressed,instructionsButton.instructionsPressed, startButton.startPressed, selectionButton.selectPressed,selectionButton.selectPressed, startButton.startPressed, instructionsButton.instructionsPressed, instructionsButton.instructionsPressed, startButton.startPressed, selectionButton.selectPressed,startButton.startPressed, selectionButton.selectPressed, instructionsButton.instructionsPressed,instructionsButton.instructionsPressed, startButton.startPressed, selectionButton.selectPressed,selectionButton.selectPressed,  startButton.startPressed, instructionsButton.instructionsPressed, selectionButton.selectPressed,  startButton.startPressed, instructionsButton.instructionsPressed, startButton.startPressed, selectionButton.selectPressed, instructionsButton.instructionsPressed};
        justPressed = false;
        Event e; //Event references to user interface
        while (app.pollEvent(e)) //Name of event that is going to be used in the window interface
            {
            if (e.type == Event::Closed) //If the window is closed then terminate the event
                app.close();

            if (e.type == Event::MouseButtonPressed)
                if (e.key.code == Mouse::Left)
                {
                    if (!isSwap && !isMoving) click++; //If the one candy is not swapping and not moving, then count the click
                    pos = Mouse::getPosition(app) - offset; //Get the position of the mouse
                }

            }
        //Select pressed
        if (isGame) { // Only when the game is on then the script of changing the frame position and swapping should work too
            if (selectionButton.selectPressed && !isSwap && !isMoving) { // If selection button has been pressed while nothing is swapping or moving
                if (!visible.locked) { // If it is the first green frame
                    x0 = visible.col;
                    y0 = visible.row;
                    invisible.col = visible.col;
                    invisible.row = visible.row;
                    visible.locked = true;
                } else { //If it is turn to the second red frame
                    x = invisible.col;
                    y = invisible.row;
                    if (abs(x - x0) + abs(y - y0) ==
                        1) //If we are not clicking the same grid then swap the quadrants, indicate that a swap is taking place and reset the click counter
                    {
                        if (Moves.value) { // Only allow candy swapping when the user has moves available
                            swap(grid[y0][x0], grid[y][x]);
                            isSwap = 1;
                            visible.locked = false;
                            Moves.value -= 1;
                            if (!Moves.value) {
                                justPressed = true;
                            }
                        }
                    } else {
                        visible.locked = true;
                    }
                    //If we are clicking the same grid then don't count it as another click
                }
            }
        }



        // mouse click
        if (click == 1) //Saving the coordinates of the first candy clicked
            {
            x0 = pos.x / ts + 1;
            y0 = pos.y / ts + 1;
            }
        if (click == 2) //Saving the coordinates of the second candy clicked
            {
            x = pos.x / ts + 1;
            y = pos.y / ts + 1;
            if (abs(x - x0) + abs(y - y0) == 1) //If we are not clicking the same grid then swap the quadrants, indicate that a swap is taking place and reset the click counter
                {
                swap(grid[y0][x0], grid[y][x]);
                isSwap = 1;
                click = 0;
                }
            else click = 1; //If we are clicking the same grid then don't count it as another click
            }

        //Match finding
        for (int i = 1; i <= 8; i++)
            for (int j = 1; j <= 8; j++) { //If there are 3 candies from the same type in the same row
                if (grid[i][j].kind == grid[i + 1][j].kind) {
                    if (grid[i][j].kind == grid[i - 1][j].kind) {
                        for (int n = -1; n <= 1; n++) {
                            grid[i + n][j].match++;
                            if (!isMoving) {
                                if (grid[i][j].kind == 0 || grid[i][j].kind == 2 || grid[i][j].kind == 3) {
                                    Score.value -= SCORE_NEGATIVE;
                                    out = "Y";
                                } else if (grid[i][j].kind == 1 || grid[i][j].kind == 4 || grid[i][j].kind == 5) {
                                    Score.value += SCORE_POSITIVE;
                                    out = "X";
                                }
                                Sleep(10);
                            }
                        }
                    }
                }


                //If there are 3 candies from the same type in the same column
                else if (grid[i][j].kind == grid[i][j + 1].kind) {
                    if (grid[i][j].kind == grid[i][j - 1].kind) {
                        for (int n = -1; n <= 1; n++) {
                            grid[i][j + n].match++;
                            if (!isMoving) {
                                if (grid[i][j].kind == 0 || grid[i][j].kind == 2 || grid[i][j].kind == 3) {
                                    Score.value -= SCORE_NEGATIVE;
                                    out = "Y";
                                } else if (grid[i][j].kind == 1 || grid[i][j].kind == 4 || grid[i][j].kind == 5) {
                                    Score.value += SCORE_POSITIVE;
                                    out = "X";
                                }
                                Sleep(10);
                            }
                        }
                    }
                }
                }

        //Moving animation
        isMoving = false;
        for (int i = 1; i <= 8; i++)
            for (int j = 1; j <= 8; j++) {
                piece &p = grid[i][j];
                int dx, dy;
                for (int n = 0; n < 4; n++)   // 4 - speed
                    {
                    dx = p.x - p.col * ts;
                    dy = p.y - p.row * ts;
                    if (dx) p.x -= dx / abs(dx);
                    if (dy) p.y -= dy / abs(dy);
                    }
                if (dx || dy) isMoving = 1;
                }

        //Deleting amimation
        if (!isMoving)
            for (int i = 1; i <= 8; i++)
                for (int j = 1; j <= 8; j++)
                    if (grid[i][j].match)
                        if (grid[i][j].alpha > 10) {
                            grid[i][j].alpha -= 10;
                            isMoving = true;
                            }

        //Get scoreMatch
        int scoreMatch = 0;
        for (int i = 1; i <= 8; i++) {
            for (int j = 1; j <= 8; j++) {
                scoreMatch += grid[i][j].match;
            }
        }


        //Second swap if no match
        if (isSwap && !isMoving) {
            if (!scoreMatch) swap(grid[y0][x0], grid[y][x]);
            isSwap = 0;
            }

        //Update grid
        if (!isMoving) {
            for (int i = 8; i > 0; i--)
                for (int j = 1; j <= 8; j++)
                    if (grid[i][j].match)
                        for (int n = i; n > 0; n--)
                            if (!grid[n][j].match) {
                                swap(grid[n][j], grid[i][j]);
                                break;
                            };

            for (int j = 1; j <= 8; j++)
                for (int i = 8, n = 0; i > 0; i--)
                    if (grid[i][j].match) {
                        grid[i][j].kind = rand() % 6;
                        grid[i][j].y = -ts * n++;
                        grid[i][j].match = 0;
                        grid[i][j].alpha = 255;
                    }
            }

        if (justPressed){ // Recognize if a button has just been presed and reset the read if it has in order to creaty a certain delay that one button press would not trigger more than 1 equal reaction
            resetButtons();
        }

        //Check for questions option
        if (!Moves.value && !isMoving && !isSwap){
            isQuestion = true;
            isGame = false;
            isInstruction = false;
        }
        //If we have reached the max amount of questions to show then go directly to the last screen
        if(questionCounter > NUMBER_QUESTIONS - 1){
            isQuestion = false;
            isFinal = true;
            isGame = false;
            isInstruction=false;
        }
        cout << "estoy fuera" <<endl;

        //////draw///////
        if(!isVideo && !isInstruction && !isGame && !isQuestion && !isFinal){ //If the game has not started yet
            app.draw(startScreen);
            if(startButton.startPressed){
                isVideo=true;
            }
        }
        else if (isVideo && !isInstruction && !isGame && !isQuestion && !isFinal){
            app.draw(videoScreen);
            Score.value = 0; // Resets the score value after the grid is set up so the player can start from 0
            if(instructionsButton.instructionsPressed){
                isInstruction=true;
                isVideo = false;
                justPressed = true;
                Sleep(SLEEP_TIME);
            }
        }
        else if (!isVideo && isInstruction && !isGame && !isQuestion && !isFinal){
            app.draw(instructionsScreen);
            if(instructionsButton.instructionsPressed){ //Press the instructions button after reading them to start the actual game
                isGame=true;
                isInstruction=false;
                justPressed = true;
                Sleep(SLEEP_TIME);
            }
        }
        else if (!isVideo && !isInstruction && isGame && !isQuestion && !isFinal){ // If the game is on then update the grid to be drwan
            app.draw(background);
            for (int i = 1; i <= 8; i++)
                for (int j = 1; j <= 8; j++) {
                    piece p = grid[i][j];
                    gems.setTextureRect(IntRect(p.kind * 120, 0, 120, 120)); // Selecting the candy type is based on the printing value from the rectangle
                    gems.setColor(Color(255, 255, 255, p.alpha));
                    gems.setPosition(p.x, p.y);
                    gems.move(offset.x - ts, offset.y - ts);
                    app.draw(gems);
                }

            greenbox.setPosition(grid[visible.row][visible.col].x + offset.x - ts, grid[visible.row][visible.col].y + offset.y - ts);
            redbox.setPosition(grid[invisible.row][invisible.col].x + offset.x - ts, grid[invisible.row][invisible.col].y + offset.y - ts);
            if(visible.locked) { //Make sure to hide the red frame when the green is being used for the first time
                redbox.setColor(Color(255,255,255,255));
            }
            else{
                redbox.setColor(Color(255,255,255,0));
            }
            //Re read the boards values
            Score.reset();
            Moves.reset();
            app.draw(Score.lblPrint);
            app.draw(Moves.lblPrint);
            app.draw(greenbox);
            app.draw(redbox);
            if(instructionsButton.instructionsPressed){
                isGame=false;
                isInstruction=true;
                justPressed = true;
                Sleep(SLEEP_TIME);
            }
        }
        else if(!isVideo && !isInstruction && !isGame && isQuestion && !isFinal){
            app.draw(questions[questionNumbers[questionCounter]]);
            if(questionAnswers[questionNumbers[questionCounter]][0]){ // Question counter allows us to track the amount of questiosn asked and also the posiiton in the array from which we ould get a random number per se from the previous script.
                // If the question has been answered correctly.
                isQuestion = false;
                isGame = true;
                Moves.value = 3;
                questionCounter += 1;
                justPressed = true;
                Sleep(SLEEP_TIME);
            }
                // If the question hasn not been answered correctly.

            else if(questionAnswers[questionNumbers[questionCounter]][1] || questionAnswers[questionNumbers[questionCounter]][2]){
                questionCounter += 1;
                resetButtons();
                justPressed = true;
                Sleep(SLEEP_TIME);
            }
        }
        else if (!isVideo && !isInstruction && !isGame && !isQuestion && isFinal) { // Last screen result that shows how many trees have they saved
            app.draw(finalScreen);
            Result.value = Score.value / 10;
            Result.reset();
            app.draw(Result.lblPrint);
            outputName = OUTPUT_FILE_NAME;
            outputFile.open(outputName.c_str());
            if(outputFile){
                outputFile << Score.value << endl;
            }
            outputFile.close();
        }

        app.display();
        }
    return 0;
}


void connectingArduino(){
    if(arduino.isConnected()){
        cout<<"Connection made"<<endl<<endl;
    }
    else{
        cout<<"Error in port name"<<endl<<endl;
    }
}

void readArduino(){
    arduino.writeSerialPort(out, MAX_DATA_LENGTH); // Writes the pointer character info to serial port
    Sleep(ARDUINO_WAIT_TIME); // Delay
    arduino.readSerialPort(incomingData, MAX_DATA_LENGTH); // Reads character array info into incomingData
    cout << incomingData << endl;
    //Refresh the buttons pressed bools
    selectionButton.readButton(incomingData);
    startButton.readButton(incomingData);
    instructionsButton.readButton(incomingData);
    if(!visible.locked){ // If the green frame is unlocked then it should be the only one moving
        visible.readJoystick(incomingData);
    }
    else {
        invisible.readJoystick(incomingData);
    }
    //Debugging support
    if (incomingData[0] == '0' && incomingData[1] == '0'){
        cout << "Nothing has been pressed" << endl;
    }
    if (incomingData[0] == '1'){
        cout << "The joystick has been moved up" << endl;
        cout << incomingData[0]<<endl;
    }
    else if (incomingData[0] == '3'){
        cout << "The joystick has been moved down" << endl;
        cout << incomingData[0] << endl;
    }
    else if (incomingData[0] == '4'){
        cout << "The joystick has been moved left" << endl;
    }
    else if (incomingData[0] == '2'){
        cout << "The joystick has been moved right" << endl;
    }
    if (incomingData[1] == 'E'){
        cout << "The select button is being pressed" << endl;
    }
    if (incomingData[1] == 'S'){
        cout << "The start button is being pressed" << endl;
    }
    if (incomingData[1] == 'I'){
        cout << "The instruction button is being pressed" << endl;
    }
}

void swap(piece p1,piece p2) //Swap grid position from the pieces selected
{
    std::swap(p1.col,p2.col); //Swap position by reference
    std::swap(p1.row,p2.row);

    grid[p1.row][p1.col]=p1; //Redefine the pieces p1 and p2 with the swapped grid positions.
    grid[p2.row][p2.col]=p2;
}

void resetButtons(){ // Makes all the bools go directly to false
    selectionButton.selectPressed = false;
    startButton.startPressed = false;
    instructionsButton.instructionsPressed = false;
    Sleep(SLEEP_TIME);
};