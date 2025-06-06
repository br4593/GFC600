#pragma once

#include <Arduino.h>

const unsigned long FLASH_DURATION = 10000; // 10 seconds
const unsigned long FLASH_INTERVAL = 500;   // flash every 500ms
const int ALT_CAPTURE_THRESHOLD = 200; // feet before selected altitude to start flashing ALTS


// Display layout
constexpr uint8_t DISPLAY_WIDTH = 254;
constexpr uint8_t DISPLAY_HEIGHT = 64;

// Section widths
constexpr uint8_t WIDTH_LATERAL  = 50;
constexpr uint8_t WIDTH_VERTICAL = 125;
constexpr uint8_t WIDTH_MESSAGES = 75;

constexpr uint8_t CORRECTION_FACTOR = 3;


// X coordinates of each section
constexpr uint8_t X_ACTIVE_SPACING_FACTOR = 15;
constexpr uint8_t X_LATERAL  = 0;
constexpr uint8_t X_DIV1     = X_LATERAL + WIDTH_LATERAL;       // 50
constexpr uint8_t X_VERTICAL = X_DIV1 + 2;                   // 52
constexpr uint8_t X_DIV2     = X_VERTICAL + WIDTH_VERTICAL;     // 177

constexpr uint8_t X_VERTICAL_THREE_DIGITS = 140; 
constexpr uint8_t X_VERTICAL_FOUR_DIGITS = X_VERTICAL_THREE_DIGITS - 12;;
constexpr uint8_t X_VERTICAL_FIVE_DIGITS = X_VERTICAL_FOUR_DIGITS - 12;;



constexpr uint8_t X_VERTICAL_TWO_DIGITS = X_DIV2 - X_ACTIVE_SPACING_FACTOR - CORRECTION_FACTOR;
constexpr uint8_t X_MESSAGES = X_DIV2 + 2;    
constexpr uint8_t X_UNITS = X_DIV2 - 20;  
            // 179

constexpr uint8_t Y_ACTIVE = 25;
constexpr uint8_t Y_ARMED = 60;
constexpr uint8_t Y_UNITS = 35;     


constexpr uint8_t CLEAR_HEIGHT = 40; // Height of the area to clear
constexpr uint8_t FONTS_LEFT_ALIGN_FACTOR = 2;

const int NUM_OF_MODES = 15; // Number of modes

extern const char *DOWN_ARROW; // Symbol for a downward arrow.
extern const char *UP_ARROW;   // Symbol for an upward arrow.       
extern const int   ARROW_SIZE; // Size of the arrow symbol. 

// Font settings
#define FONT_ACTIVE u8g2_font_logisoso18_tr
#define FONT_ARMED    u8g2_font_6x10_tr
#define ARROW_FONT u8g2_font_9x15_m_symbols

// Message ID mapping
enum DisplayArea {
    AREA_LATERAL_MODES,
    AREA_VERTICAL_MODES,
    AREA_MESSAGES,
    AREA_COUNT
};

enum ModesMessageId
{
    // Lateral Modes
    ROL = 0,   // Heading Select
    HDG,       // Roll Hold
    VOR,
    GPS,       // Navigation (GPS, VOR, LOC)
    APR,       // Approach (includes GS or GP)
    BC,        // Backcourse
    LVL,       // Level (Wings Level)
    GA,        // Go Around
    LOC,       // Localizer (VOR/LOC)

    // Vertical Modes
    VS,        // Vertical Speed
    IAS,       // Indicated Airspeed
    ALT,       // Altitude Hold
    ALTS,      // Selected Altitude Capture
    VPTH,      // Vertical Path
    VNAV,      // Generic VNAV (optional, if you track VNAV separately from VPTH)
    GS_ARMED,        // Glideslope
    GS_ACTIVE,        // Glidepath
    PIT,
    GP,

    ALTITUDE_VALUE_100FT = 27,
    ALTITUDE_WITHIN_50FT = 28,
    ALTITUDE_WITHIN_200FT = 29,
    ALTITUDE_LOCK_VAL = 30,
    
    VS_VAL = 31,
    IAS_VAL = 32,

    NONE = 9999
};

enum Colors {
    BLACK = 0,
    WHITE = 1
};

typedef struct {
    int x;
    int y;
    int width;
    int height;
}Rect;

extern Rect activeLateralArea;
extern Rect armedLateralArea;
extern Rect activeVerticalArea;
extern Rect armedVerticalArea;

