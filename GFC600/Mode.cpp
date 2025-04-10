
#include "Mode.h"
#include <Arduino.h>
#include "GFC600_utils.h"

unsigned long flashStartTime = 0; // Initialize flash start time
bool flashing = false; // Initialize flashing state


void setState(Mode *mode, bool state) {
    mode->state = state; // Set the state of the mode
}

void setName(Mode *mode, String name) {
    mode->name = name; // Set the name of the mode
}

void setValue(Mode *mode, ModesMessageId value) {
    mode->value = value; // Set the value of the mode
}

bool getState(Mode *mode) {
    return mode->state; // Get the state of the mode
}

String getName(Mode *mode) {
    return mode->name; // Get the name of the mode
}

int getValue(Mode *mode) {
    return mode->value; // Get the value of the mode
}

bool compareModes(Mode mode1, Mode mode2) {

    
    // Compare all fields
    return (mode1.state == mode2.state &&
            mode1.name == mode2.name &&
            mode1.value == mode2.value);
}

