//
// Created by mezallamosas.j on 11/20/2022.
//

#ifndef CANDYCRUSH_V2_BOARD_H
#define CANDYCRUSH_V2_BOARD_H
#include <SFML/Graphics.hpp>
#include <sstream>
using namespace sf;
using namespace std;

class Board {
    Font font;
    ostringstream ssPrint; //Standard Output String
public:
    int value = 0;
    Text lblPrint; //Label printed in the displayed window
    Board(float posx, float posy);
    void reset();
};


#endif //CANDYCRUSH_V2_BOARD_H
