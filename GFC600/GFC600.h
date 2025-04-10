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
    Mode decideArmedLateralMode();
    Mode decideActiveVertialMode();
    Mode decideArmedVerticalModeOne();
    Mode decideArmedVerticalModeTwo();
    void drawActiveVerticalMode(Mode mode);
    void drawArmedVerticalModeOne(Mode mode);
    void drawArmedVerticalModeTwo(Mode mode);
    void drawVerticalSetting(Mode mode);
    void altModeDrawingHandler(Mode mode);
    void iasModeDrawingHandler();
    void vsModeDrawingHandler();
    void drawArrow(uint8_t x, uint8_t y, const char *arrow);
    void flashModeTranistion();
    void drawFlashingText(uint8_t x, uint8_t y, const uint8_t* font, const char* text, bool flashEnabled);
    


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
    Mode _gs_armed;
    Mode _gs_active;
    Mode _gp;
    Mode _pit;
    Mode _none;

    int _altitude_lock_value;
    int _altitude_value_100ft;
    //int _altitude_value_10ft;
    bool _within50ft;
    bool _within200ft;
    int _vs_value;
    int _ias_lock_value;


    bool _flashAlts;
    bool _flashAlt;
    unsigned long _flashAltsStart;
    unsigned long _flashAltStart;
    unsigned long _flashAltsMillis;

    
 


};