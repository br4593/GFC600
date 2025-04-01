#pragma once
#include <Arduino.h>
#include "GFC600_constants.h"

class Mode
{

public:
    Mode(boolean state, String name, ModesMessageId value); // Constructor to initialize the mode with a state and name
    String getName();
    void setName(String name);
    boolean getState();
    void setState(bool state);

private:
    bool _state;
    String _name;
    int _value;
};