//
// Created by mezallamosas.j on 11/14/2022.
//

#include "Button.h"

Button::Button(){
    selectPressed = false;
    startPressed = false;
    instructionsPressed = false;
}


void Button::readButton(char Data[]) {
    if (Data[1] == 'E'){ // Select
        selectPressed = true;
        startPressed = false;
        instructionsPressed = false;
    }
    else if (Data[1] == 'S'){ // Start
        selectPressed= false;
        startPressed = true;
        instructionsPressed = false;
    }
    else if (Data[1] == 'I'){ // Instructions
        selectPressed = false;
        startPressed = false;
        instructionsPressed = true;
    }
    else{ // None of them pressed
        selectPressed = false;
        startPressed = false;
        instructionsPressed = false;
    }
}

