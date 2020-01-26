#ifndef RCHANDLER_H
#define RCHANDLER_H

#include <Arduino.h>

// 433MHz device
class RcHandler {
public:
    static void setup(byte sndPin, byte rcvPin);

    static void sendTriState(const char* code);
    static void send(unsigned long decimal, unsigned int length);
    static void send(const char* code, unsigned int protocol = 1, unsigned int pulseLength = 320, unsigned int repeatTransmit = 15);

    static void loop();
    static void printSignal();
    static void printSignal(unsigned long decimal, unsigned int length, unsigned int delay, unsigned int* raw, unsigned int protocol);

    static bool available();
    static void resetAvailable();
    static String getValue();
    static char* getSignalCode();
    static unsigned int getSignalPulseLength();
    static unsigned int getSignalProtocol();

    static char* bin2tristate(const char* bin);
    static char * dec2binWzerofill(unsigned long dec, unsigned int bitLength);
};
#endif
