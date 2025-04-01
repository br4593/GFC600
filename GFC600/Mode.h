#pragma once
#include <Arduino.h>

class Mode
{

public:
    Mode(boolean state, String name); // Constructor to initialize the mode with a state and name
    String getName();
    void setName(String name);
    boolean getState();
    void setState(bool state);

private:
    bool _state;
    String _name;
};