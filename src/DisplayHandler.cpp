#include <SSD1306Wire.h>
#include "DisplayHandler.h"

SSD1306Wire  *whDisplay = NULL;
int whCurrentDisplayLine = 0;

void DisplayHandler::setup(byte sdaPin, byte sclPin) {
    Serial.print(F("[OLED] Initialize display... "));
    whDisplay = new SSD1306Wire(0x3c, sdaPin, sclPin);
    whDisplay->init();
    whDisplay->flipScreenVertically();
    whDisplay->setFont(ArialMT_Plain_10);
    whDisplay->setTextAlignment(TEXT_ALIGN_LEFT);
    Serial.println(F("done"));
}

void DisplayHandler::clear() {
    whDisplay->clear();
}

void DisplayHandler::writeLine(unsigned int line, String text, bool updateDisplay) {
    short pixelLine = line > 0 ? (line - 1) * 10 + 13 : 0;
    whDisplay->drawString(0, pixelLine, text);
    if (updateDisplay) display();
}

void DisplayHandler::writeLine(String text) {
    writeLine(whCurrentDisplayLine, text, false);
    whCurrentDisplayLine++;
}

void DisplayHandler::display() {
    whDisplay->display();
    whCurrentDisplayLine = 0;
}