#ifndef DISPLAYHANDLER_H
#define DISPLAYHANDLER_H

#include <Arduino.h>

// oled 128x64 pixel
class DisplayHandler {
public:
    static void setup(byte sdaPin, byte sclPin);

    static void clear();
    static void writeLine(unsigned int line, String text, bool updateDisplay = true);
    static void writeLine(String text);
    static void display();
};
#endif
