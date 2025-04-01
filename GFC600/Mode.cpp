
#include "Mode.h"
#include <Arduino.h>


Mode::Mode(bool state, String name) {
    _state = state; // Initialize the state of the mode
    _name = name; // Initialize the name of the mode
}

String Mode::getName() {
    return _name; // Return the name of the mode
}

void Mode::setName(String name) {
    _name = name; // Set the name of the mode
}

boolean Mode::getState() {
    return _state; // Return the state of the mode
}

void Mode::setState(bool state) {
    _state = state; // Set the state of the mode
}
