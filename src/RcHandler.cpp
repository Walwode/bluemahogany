#include <RCSwitch.h>
#include "RcHandler.h"

RCSwitch *whRcSwitch = NULL;

void RcHandler::setup(byte sndPin, byte rcvPin) {
    Serial.print(F("[433MHz] Initialize switch... "));
    whRcSwitch = new RCSwitch();
    whRcSwitch->enableTransmit(sndPin);
    whRcSwitch->enableReceive(rcvPin);
    Serial.println(F("done"));
}

void RcHandler::send(unsigned long decimal, unsigned int length) {
    whRcSwitch->send(decimal, length);
}

void RcHandler::sendTriState(const char* code) {
    whRcSwitch->send(code);
}

void RcHandler::send(const char* code, unsigned int protocol, unsigned int pulseLength, unsigned int repeatTransmit) {
    whRcSwitch->send(code); // "101010101010010101010101"
    whRcSwitch->setProtocol(protocol);
    whRcSwitch->setPulseLength(pulseLength);
    whRcSwitch->setRepeatTransmit(repeatTransmit);
}

void RcHandler::loop() {
    if (whRcSwitch->available()) {
        Serial.println(F("[433MHz] >> Signal received:"));
        printSignal();
        whRcSwitch->resetAvailable();
        Serial.println(F("[433MHz] << Signal received"));
    }
}

bool RcHandler::available() {
    return whRcSwitch->available();
}

void RcHandler::resetAvailable() {
    whRcSwitch->resetAvailable();
}

String RcHandler::getValue() {
    return
     String(whRcSwitch->getReceivedValue())
     + " ("
     + String(whRcSwitch->getReceivedBitlength())
     + "bit)";
}

char* RcHandler::getSignalCode() {
    return dec2binWzerofill(whRcSwitch->getReceivedValue(), whRcSwitch->getReceivedBitlength());
}

unsigned int RcHandler::getSignalPulseLength() {
    return whRcSwitch->getReceivedDelay();
}

unsigned int RcHandler::getSignalProtocol() {
    return whRcSwitch->getReceivedProtocol();
}

void RcHandler::printSignal() {
    printSignal(
        whRcSwitch->getReceivedValue(),
        whRcSwitch->getReceivedBitlength(),
        whRcSwitch->getReceivedDelay(),
        whRcSwitch->getReceivedRawdata(),
        whRcSwitch->getReceivedProtocol());
}

void RcHandler::printSignal(unsigned long decimal, unsigned int length, unsigned int delay, unsigned int* raw, unsigned int protocol) {

    const char* b = dec2binWzerofill(decimal, length);
    Serial.print("Decimal: ");
    Serial.print(decimal);
    Serial.print(" (");
    Serial.print(length);
    Serial.print("Bit) Binary: ");
    Serial.print(b);
    Serial.print(" Tri-State: ");
    Serial.print(bin2tristate(b));
    Serial.print(" PulseLength: ");
    Serial.print(delay);
    Serial.print(" microseconds");
    Serial.print(" Protocol: ");
    Serial.println(protocol);
    
    Serial.print("Raw data: ");
    for (unsigned int i=0; i<= length*2; i++) {
        Serial.print(raw[i]);
        Serial.print(",");
    }
    Serial.println();
    Serial.println();
}

char* RcHandler::bin2tristate(const char* bin) {
    static char returnValue[50];
    int pos = 0;
    int pos2 = 0;
    while (bin[pos]!='\0' && bin[pos+1]!='\0') {
        if (bin[pos]=='0' && bin[pos+1]=='0') {
            returnValue[pos2] = '0';
        } else if (bin[pos]=='1' && bin[pos+1]=='1') {
            returnValue[pos2] = '1';
        } else if (bin[pos]=='0' && bin[pos+1]=='1') {
            returnValue[pos2] = 'F';
        } else {
            return "not applicable";
        }
        pos = pos+2;
        pos2++;
    }
    returnValue[pos2] = '\0';
    return returnValue;
}

char* RcHandler::dec2binWzerofill(unsigned long dec, unsigned int bitLength) {
    static char bin[64];
    unsigned int i=0;

    while (dec > 0) {
        bin[32+i++] = ((dec & 1) > 0) ? '1' : '0';
        dec = dec >> 1;
    }

    for (unsigned int j = 0; j< bitLength; j++) {
        if (j >= bitLength - i) {
            bin[j] = bin[ 31 + i - (j - (bitLength - i)) ];
        } else {
            bin[j] = '0';
        }
    }
    bin[bitLength] = '\0';
    
    return bin;
}
