#include <Arduino.h>
#include <RCSwitch.h>
#include "DisplayHandler.h"

#define RC_SND_PIN D2    // esp8862 pin D2 = GPIO4
#define RC_RCV_PIN D1    // esp8862 pin D1 = GPIO5
#define OLED_SDA_PIN D3
#define OLED_SCL_PIN D5

RCSwitch *rcSwitch = NULL;

void receiveSignal();
void sendSignal(const char* code, unsigned int protocol, unsigned int pulseLength, unsigned int repeatTransmit);
char* bin2tristate(const char* bin);
char* dec2binWzerofill(unsigned long dec, unsigned int bitLength);

void setup() {
    Serial.begin(115200);

    Serial.print(F("[433MHz] Initialize switch... "));
    rcSwitch = new RCSwitch();
    rcSwitch->enableTransmit(RC_SND_PIN);
    rcSwitch->enableReceive(RC_RCV_PIN);
    Serial.println(F("done"));

    DisplayHandler::setup(OLED_SDA_PIN, OLED_SCL_PIN);
    DisplayHandler::clear();
    DisplayHandler::writeLine("Waiting for signal...");
    DisplayHandler::display();
}

void loop() {
    if (rcSwitch->available())
        receiveSignal();
}

void receiveSignal() {
    DisplayHandler::clear();

    if (rcSwitch->getReceivedProtocol() != -1) {
        const char* binValue = dec2binWzerofill(rcSwitch->getReceivedValue(), rcSwitch->getReceivedBitlength());

        Serial.print("Protocol: ");
        Serial.print(rcSwitch->getReceivedProtocol());
        DisplayHandler::writeLine("Protocol: " + String(rcSwitch->getReceivedProtocol()));

        Serial.print(", Bit: ");
        Serial.print(rcSwitch->getReceivedBitlength());
        DisplayHandler::writeLine("Bit: " + String(rcSwitch->getReceivedBitlength()));

        Serial.print(", Length: ");
        Serial.print(rcSwitch->getReceivedDelay());
        DisplayHandler::writeLine("Length: " + String(rcSwitch->getReceivedDelay()));

        Serial.print(", Binary: ");
        Serial.print(binValue);
        DisplayHandler::writeLine("Binary: " + String(binValue));

        Serial.print(", Tri-State: ");
        Serial.print(bin2tristate(binValue));
        DisplayHandler::writeLine("Tri-State: " + String(bin2tristate(binValue)));

        Serial.print(", Inverted: ");
        Serial.print((bool)rcSwitch->getReceivedFirstDigital() ? "false" : "true");
        DisplayHandler::writeLine("Inverted: " + String((bool)rcSwitch->getReceivedFirstDigital() ? "false" : "true"));

    } else {
        // unknown protocol
        Serial.print("Protocol: unknown");
        DisplayHandler::writeLine("Protocol: unkown");

        Serial.print(", Bit: ");
        Serial.print(rcSwitch->getReceivedBitlength());
        DisplayHandler::writeLine("Bit: " + String(rcSwitch->getReceivedBitlength()));

        Serial.print(", Length: "); // divide by longer syncLength
        Serial.print(rcSwitch->getReceivedDelay());
        DisplayHandler::writeLine("Length: " + String(rcSwitch->getReceivedDelay()));

        Serial.print(", Repetition: ");
        Serial.print(rcSwitch->getReceivedValue());
        DisplayHandler::writeLine("Repetition: " + String(rcSwitch->getReceivedValue()));

        Serial.print(", Inverted: ");
        Serial.print((bool)rcSwitch->getReceivedFirstDigital() ? "false" : "true");
        DisplayHandler::writeLine("Inverted: " + String((bool)rcSwitch->getReceivedFirstDigital() ? "false" : "true"));

        Serial.println();
        unsigned int* rawData = rcSwitch->getReceivedRawdata();
        String rawDataString = "";
        for (unsigned int i = 0; i < rcSwitch->getReceivedBitlength() * 2 + 2; i++) {
            if (i > 0) Serial.print("-");
            Serial.print(rawData[i]);
            rawDataString += rawData[i] + "-";
        }
        DisplayHandler::writeLine(rawDataString);
    }

    Serial.println();
    DisplayHandler::display();

    rcSwitch->resetAvailable();
}

void sendSignal(const char* code, unsigned int protocol, unsigned int pulseLength, unsigned int repeatTransmit) {
    rcSwitch->send(code); // "101010101010010101010101"
    rcSwitch->setProtocol(protocol);
    rcSwitch->setPulseLength(pulseLength);
    rcSwitch->setRepeatTransmit(repeatTransmit);
}

char* bin2tristate(const char* bin) {
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

char* dec2binWzerofill(unsigned long dec, unsigned int bitLength) {
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