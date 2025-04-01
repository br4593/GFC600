#pragma once

#include "Arduino.h"
#include <U8g2lib.h>
#include "GFC600_constants.h"
#include "GFC600_types.h"

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
    void renderDisplay();
    void drawActiveLateralMode(LateralMode mode);
    void clearDisplay();


private:
    bool    _initialised;
    uint8_t _cs, _dc, _rst;
    U8G2_SSD1322_NHD_256X64_F_4W_HW_SPI _u8g2;
};