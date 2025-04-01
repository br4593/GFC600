#pragma once

#include <Arduino.h>
// Display layout
constexpr uint8_t DISPLAY_WIDTH = 254;
constexpr uint8_t DISPLAY_HEIGHT = 64;

// Section widths
constexpr uint8_t WIDTH_LATERAL  = 50;
constexpr uint8_t WIDTH_VERTICAL = 125;
constexpr uint8_t WIDTH_MESSAGES = 75;

// X coordinates of each section
constexpr uint8_t X_LATERAL  = 0;
constexpr uint8_t X_DIV1     = X_LATERAL + WIDTH_LATERAL;       // 50
constexpr uint8_t X_VERTICAL = X_DIV1 + 2;                       // 52
constexpr uint8_t X_DIV2     = X_VERTICAL + WIDTH_VERTICAL;     // 177
constexpr uint8_t X_MESSAGES = X_DIV2 + 2;                       // 179

// Font settings
#define FONT_ACTIVE u8g2_font_logisoso18_tr
#define FONT_ARMED    u8g2_font_6x10_tr

// Message ID mapping
enum DisplayArea {
    AREA_LATERAL_MODES,
    AREA_VERTICAL_MODES,
    AREA_MESSAGES,
    AREA_COUNT
};

enum ModesMessageId
{
    HDG=0,
    ROL,
    NAV,
    APR,
    BC,
    VS,
    VNAV,
    ALT,
    ALTS
};
