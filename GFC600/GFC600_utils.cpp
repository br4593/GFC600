#include "GFC600_utils.h"


const char *DOWN_ARROW = "↓"; // Symbol for a downward arrow.
const char *UP_ARROW   = "↑"; // Symbol for an upward arrow.
const int   ARROW_SIZE = 20;  // Size of the arrow symbol.

Rect activeLateralArea = {0, 0, WIDTH_LATERAL - CORRECTION_FACTOR, CLEAR_HEIGHT};
Rect armedLateralArea = {0, Y_ARMED - CORRECTION_FACTOR * 3, WIDTH_LATERAL - CORRECTION_FACTOR, CLEAR_HEIGHT};
Rect activeVerticalArea = {X_DIV1 + CORRECTION_FACTOR, 0, WIDTH_VERTICAL - CORRECTION_FACTOR, CLEAR_HEIGHT};
Rect armedVerticalArea = {X_DIV1 + CORRECTION_FACTOR, Y_ARMED - CORRECTION_FACTOR * 3, WIDTH_VERTICAL - CORRECTION_FACTOR, CLEAR_HEIGHT};
