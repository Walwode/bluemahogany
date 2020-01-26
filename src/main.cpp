#include <Arduino.h>
#include "DisplayHandler.h"
#include "RcHandler.h"

#define RC_SND_PIN D2    // esp8862 pin D2 = GPIO4
#define RC_RCV_PIN D1    // esp8862 pin D1 = GPIO5
#define OLED_SDA_PIN D3
#define OLED_SCL_PIN D5

void setup() {
    Serial.begin(115200);

    RcHandler::setup(RC_SND_PIN, RC_RCV_PIN);
    DisplayHandler::setup(OLED_SDA_PIN, OLED_SCL_PIN);

    DisplayHandler::clear();
    DisplayHandler::writeLine("Waiting for signal...");
    DisplayHandler::display();
}

void loop() {
    if (RcHandler::available()) {
        DisplayHandler::clear();
        DisplayHandler::writeLine("Signal recevied...");
        DisplayHandler::writeLine("Value: " + RcHandler::getValue());
        DisplayHandler::writeLine("Pulse Length: " + String(RcHandler::getSignalPulseLength()));
        DisplayHandler::writeLine("Protocol: " + String(RcHandler::getSignalProtocol()));
        DisplayHandler::writeLine(RcHandler::getSignalCode());
        DisplayHandler::display();

        RcHandler::printSignal();
        RcHandler::resetAvailable();
    }
    delay(10);
}