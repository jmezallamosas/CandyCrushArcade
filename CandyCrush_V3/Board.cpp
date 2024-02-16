//
// Created by mezallamosas.j on 11/20/2022.
//

#include "Board.h"

Board::Board(float posx, float posy) {
    font.loadFromFile("C:/WINDOWS/FONTS/ERASDEMI.TTF");
    lblPrint.setCharacterSize(30);
    lblPrint.setPosition({posx,posy});
    lblPrint.setFont(font);
    lblPrint.setCharacterSize(100);
    lblPrint.setFillColor(Color(0,0,255,255));
    lblPrint.setOutlineColor(Color(0,0,0,255));
    lblPrint.setOutlineThickness(2);
    ssPrint << value;
    lblPrint.setString(ssPrint.str()); //Set string information to be printed in the drawn label
}

void Board::reset() {
    ssPrint.str(""); //Reset string value
    ssPrint << value; //Change output string to new value
    lblPrint.setString(ssPrint.str()); //Print new score in the text label
}
