
#include "Mode.h"
#include <Arduino.h>
#include "GFC600_constants.h"

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

