//
// Created by mezallamosas.j on 11/14/2022.
//

#ifndef CANDYCRUSH_V1_SERIALPORT_H
#define CANDYCRUSH_V1_SERIALPORT_H

#define ARDUINO_WAIT_TIME 0
#define MAX_DATA_LENGTH 2

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

class SerialPort
{
private:
    HANDLE handler;
    bool connected;
    COMSTAT status;
    DWORD errors;
public:
    SerialPort(char *portName);
    ~SerialPort();

    int readSerialPort(char *buffer, unsigned int buf_size);
    bool writeSerialPort(char *buffer, unsigned int buf_size);
    bool isConnected();
};

#endif //CANDYCRUSH_V1_SERIALPORT_H
