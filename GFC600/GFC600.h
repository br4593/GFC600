#pragma once

#include "Arduino.h"
#include <U8g2lib.h>
#include "GFC600_utils.h"
#include "Mode.h"

class GFC600
{
public:
    GFC600(uint8_t cs, uint8_t dc, uint8_t rst);
    void begin();
    void attach();
    void detach();
    void set(int16_t messageID, char *setPoint);
    void update();
    void drawDisplayLayout();
    void drawActiveLateralMode(Mode mode);
    void drawArmedLateralMode(Mode mode);
    Mode decideActiveLateralMode();
    Mode decideArmedLatealMode();
    Mode decideActiveVertialMode();
    Mode decideArmedVerticalModeOne();
    Mode decideArmedVerticalModeTwo();
    void drawActiveVerticalMode(Mode mode);
    void drawArmedVerticalModeOne(Mode mode);
    void drawVerticalSetting(Mode mode);
    void altModeDrawingHandler();
    void iasModeDrawingHandler();
    void vsModeDrawingHandler();
    


    void clearArea(uint8_t x, uint8_t y, uint8_t width, uint8_t height);
    void printTextToDisplay(uint8_t x, uint8_t y, const uint8_t *font, const char *text);
    void renderDisplay();
    void clearDisplay();


private:
    bool    _initialised;
    uint8_t _cs, _dc, _rst;
    U8G2_SSD1322_NHD_256X64_F_4W_HW_SPI _display;
    Mode _hdg;
    Mode _rol;
    Mode _vor;
    Mode _gps;
    Mode _apr;
    Mode _bc;
    Mode _loc;
    Mode _ga;
    Mode _lvl;

    Mode _vnav;
    Mode _vs;
    Mode _alt;
    Mode _alts;
    Mode _ias;
    Mode _vpth;
    Mode _gs;
    Mode _gp;
    Mode _pit;

    int _altitude_lock_value;
    int _altitude_value;
    int _vs_value;
    int _ias_lock_value;


    
 


};