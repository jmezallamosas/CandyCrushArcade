//
// Created by mezallamosas.j on 11/15/2022.
//

#ifndef CANDYCRUSH_V1_FRAME_H
#define CANDYCRUSH_V1_FRAME_H
class Frame{
public:
    int row = 1;
    int col = 1;
    bool locked = false;
    Frame()=default;
    void readJoystick(char Data[]);
};

#endif //CANDYCRUSH_V1_FRAME_H
