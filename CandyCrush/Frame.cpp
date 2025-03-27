//
// Created by mezallamosas.j on 11/15/2022.
//

#include "Frame.h"

void Frame::readJoystick(char Data[]) {
    if(Data[0] == '1' && row > 1){ // Up
        row -= 1;
    }
    else if(Data[0] == '3' && row < 8){ // Down
        row += 1;
    }
    else if(Data[0] == '2' && col < 8){ // rRight
        col += 1;
    }
    else if(Data[0] == '4' && col > 1){ // Left
        col -= 1;
    }
}


