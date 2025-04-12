#include "GFC600_utils.h"


const char *DOWN_ARROW = "↓"; // Symbol for a downward arrow.
const char *UP_ARROW   = "↑"; // Symbol for an upward arrow.
const int   ARROW_SIZE = 20;  // Size of the arrow symbol.

Rect activeLateralArea = {0, 0, WIDTH_ACTIVE, CLEAR_HEIGHT};
Rect armedLateralArea = {0, Y_ARMED-15, WIDHT_ARMED, CLEAR_HEIGHT};
Rect activeVerticalArea = {X_DIV1 + CORRECTION_FACTOR, 0, WIDTH_ACTIVE, CLEAR_HEIGHT};
Rect armedVerticalAreaOne = {X_DIV1 + CORRECTION_FACTOR, Y_ARMED-15, WIDHT_ARMED, CLEAR_HEIGHT};
Rect armedVerticalAreaTwo = {X_DIV1 + CORRECTION_FACTOR, Y_ARMED-15, WIDHT_ARMED, CLEAR_HEIGHT};
Rect verticalSettingsArea = {X_DIV2 - 70,0,70,CLEAR_HEIGHT};