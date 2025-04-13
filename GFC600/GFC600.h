 #pragma once

#include "Arduino.h"
#include <U8g2lib.h>
#include "GFC600_utils.h"
#include "Mode.h"

class GFC600
{
public:
    // ────────────── Lifecycle ──────────────
    GFC600(uint8_t cs, uint8_t dc, uint8_t rst);
    void begin();
    void attach();
    void detach();
    void update();           // Called every loop()
    void set(int16_t messageID, char *setPoint); // Handle Mobiflight updates

    // ────────────── Rendering Core ──────────────
    void renderDisplay();
    void clearDisplay();
    void drawDisplayLayout();

    // ────────────── Mode Decision Logic ──────────────
    Mode decideActiveLateralMode();
    Mode decideArmedLateralMode();
    Mode decideActiveVerticalMode();
    Mode decideArmedVerticalModeOne();
    Mode decideArmedVerticalModeTwo();

    // ────────────── Mode Drawing (Lateral) ──────────────
    void drawActiveLateralMode(Mode mode);
    void drawArmedLateralMode(Mode mode);

    // ────────────── Mode Drawing (Vertical) ──────────────
    void drawActiveVerticalMode(Mode mode);
    void drawArmedVerticalModeOne(Mode mode);
    void drawArmedVerticalModeTwo(Mode mode);
    void drawVerticalSetting(Mode mode);  // ALT, IAS, VS specific visuals
    void drawFlashingText(uint8_t x, uint8_t y, const uint8_t* font, const char* text);
    void resetFlashingOnModeChange(); // Reset flashing state when mode changes
    void updateAltitudeFlashingState(); // Update flashing state based on altitude conditions
    void updateAltitudeFlashState(); // Update flashing state based on vertical mode conditions

    // ────────────── Numeric Handlers ──────────────
    void altModeDrawingHandler(Mode mode);
    void iasModeDrawingHandler();
    void vsModeDrawingHandler();

    // ────────────── Utility Drawing ──────────────
    void drawArrow(uint8_t x, uint8_t y, const char *arrow);
    void clearArea(uint8_t x, uint8_t y, uint8_t width, uint8_t height);
    void printTextToDisplay(uint8_t x, uint8_t y, const uint8_t *font, const char *text);


    // ────────────── Initialization Helpers ──────────────
    void initModes();
    void initMode(Mode* mode, const char* name, ModesMessageId id);

    // ────────────── Math/Layout Helpers ──────────────
    int computeXPosForValue(int value);

private:
    // ────────────── Hardware ──────────────
    bool    _initialised = false;
    uint8_t _cs, _dc, _rst;
    U8G2_SSD1322_NHD_256X64_F_4W_HW_SPI _display;

    // ────────────── Modes ──────────────
    Mode _hdg, _rol, _vor, _gps, _apr, _bc, _loc, _ga, _lvl;
    Mode _vnav, _vs, _alt, _alts, _ias, _vpth, _gs_armed, _gs_active, _gp, _pit;
    Mode _none;

    Mode _activeLateralMode;
    Mode _armedLateralMode;
    Mode _activeVerticalMode;
    Mode _armedVerticalModeOne;
    Mode _armedVerticalModeTwo;

    // ────────────── State Data ──────────────
    int _altitude_lock_value = 0;
    int _altitude_value_100ft = 0;
    int _vs_value = 0;
    int _ias_lock_value = 0;
    bool _within50ft = false;
    bool _within300ft = false;

    // ────────────── Flashing State ──────────────

    bool _flashAlt = false; // Flashing state for ALT mode
    bool _flashAlts = false; // Flashing state for ALTS mode

    bool _altFlashed = false; // Flashing state for ALT mode
    bool _altsFlashed = false; // Flashing state for ALTS mode

    unsigned long _flashAltStart = 0;
    unsigned long _flashAltsStart = 0;

    bool _flashing = false; // Flashing state for ALT mode
    bool _flashFlag = false; // Flashing state for ALTS mode

    unsigned long _lastFlashToggle = 0; // Last time the text was flashed
    

    // ────────────── Render Flag ──────────────
    bool _dirty = true;
};
