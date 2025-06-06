#pragma once
#include <Arduino.h>
#include "GFC600_utils.h"

typedef struct Mode{
    bool state; // State of the mode (on/off)
    String name; // Name of the mode
    ModesMessageId value; // Value associated with the mode
} Mode;

extern unsigned long flashStartTime;
extern bool flashing;


//methods

void setState(Mode *mode, bool state); // Set the state of the mode
void setName(Mode *mode, String name); // Set the name of the mode
void setValue(Mode *mode, ModesMessageId value); // Set the value of the mode
bool getState(Mode *mode); // Get the state of the mode
String getName(Mode *mode); // Get the name of the mode
int getValue(Mode *mode); // Get the value of the mode
bool compareModes(Mode mode1, Mode mode2); // Compare two modes for equality
