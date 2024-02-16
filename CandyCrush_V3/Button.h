//
// Created by mezallamosas.j on 11/14/2022.
//

#ifndef CANDYCRUSH_V1_BUTTON_H
#define CANDYCRUSH_V1_BUTTON_H



class Button{
public:
    bool selectPressed;
    bool startPressed;
    bool instructionsPressed;
    Button();
    void readButton(char Data[]);
};
#endif //CANDYCRUSH_V1_BUTTON_H
