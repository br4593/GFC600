#include "GFC600.h"
#include "allocateMem.h"
#include "commandmessenger.h"

/* **********************************************************************************
    This is just the basic code to set up your custom device.
    Change/add your code as needed.
********************************************************************************** */



GFC600::GFC600(uint8_t cs, uint8_t dc, uint8_t rst)
    : _display(U8G2_R0, cs, dc, rst)
{
    initModes(); // Initialize the modes
    _dirty = true; // Set the dirty flag to true
}




void GFC600::begin()
{

}

void GFC600::attach()
{

    if(_display.begin()) {
        _initialised = true;
        _display.clearBuffer();
        _display.setFont(u8g2_font_ncenB08_tr); // Choose a suitable font
        _display.drawStr(0, 10, "Device Started"); // Display the message
        _display.sendBuffer();
        delay(1000); // Wait for a second to show the message
        _display.clearBuffer(); // Clear the buffer for the next drawing
    } else {
        _initialised = false;
    }

    
}

void GFC600::detach()
{
    if (!_initialised)
        return;
    _initialised = false;
}

void GFC600::set(int16_t messageID, char *setPoint)
{
    /* **********************************************************************************
        Each messageID has it's own value
        check for the messageID and define what to do.
        Important Remark!
        MessageID == -2 will be send from the board when PowerSavingMode is set
            Message will be "0" for leaving and "1" for entering PowerSavingMode
        MessageID == -1 will be send from the connector when Connector stops running
        Put in your code to enter this mode (e.g. clear a display)

    ********************************************************************************** */
    int32_t  data = atoi(setPoint);
    uint16_t output;

    switch (messageID) {
        case -1:
            // MobiFlight shuts down
            clearDisplay();
            break;

        case -2:
            // Power saving mode
            if (data == 1) clearDisplay();
            break;

        case ROL:
            setState(&_rol, data);
            _dirty = true;
            break;

        case HDG:
            setState(&_hdg, data);
            _dirty = true;
            break;

        case VOR:
            setState(&_vor, data);
            _dirty = true;
            break;

        case GPS:
            setState(&_gps, data);
            _dirty = true;
            break;

        case APR:
            setState(&_apr, data);
            _dirty = true;
            break;

        case BC:
            setState(&_bc, data);
            _dirty = true;
            break;

        case LVL:
            setState(&_lvl, data);
            _dirty = true;
            break;

        case GA:
            setState(&_ga, data);
            _dirty = true;
            break;

        case LOC:
            setState(&_loc, data);
            _dirty = true;
            break;  

        case VS:
            setState(&_vs, data);
            _dirty = true;
            break;

        case IAS:
            setState(&_ias, data);
            _dirty = true;
            break;

        case ALT:
            setState(&_alt, data);
            _dirty = true;
            break;

        case ALTS:
            setState(&_alts, data);
            _dirty = true;
            break;

        case VPTH:
            setState(&_vpth, data);
            _dirty = true;
            break;

        case VNAV:
            setState(&_vnav, data);
            _dirty = true;
            break;

        case GS_ARMED:
            setState(&_gs_armed, data);
            _dirty = true;
            break;

        case GS_ACTIVE:
            setState(&_gs_active, data);
            _dirty = true;
            break;


        case PIT:
            setState(&_pit, data);
            _dirty = true;
            break;

        case ALTITUDE_VALUE_100FT:
            // Set the altitude value
            _altitude_value_100ft = data;
            _dirty = true;
            break;

        case ALTITUDE_WITHIN_50FT:
            // Set the altitude value
            _within50ft = data;
            _dirty = true;
            break;

        case ALTITUDE_WITHIN_200FT:
            // Set the altitude value
            _within200ft = data;
            _dirty = true;
            break;

        case ALTITUDE_LOCK_VAL:
            // Set the altitude lock value
            _altitude_lock_value = data;
            _dirty = true;
            break;

        case VS_VAL:
            // Set the vertical speed value
            _vs_value = data;
            _dirty = true;
            break;
        
        case IAS_VAL:
            // Set the indicated airspeed value
            _ias_lock_value = data;
            _dirty = true;
            break;

        default:
            // Unknown message ID
            break;
    }
}

void GFC600::drawDisplayLayout()
{
    _display.drawLine(X_DIV1, 0, X_DIV1, DISPLAY_HEIGHT); // Vertical line between Lateral and Vertical section
    _display.drawLine(X_DIV2, 0, X_DIV2, DISPLAY_HEIGHT); // Vertical line between Vertical and Message section
}

void GFC600::renderDisplay()
{
    drawDisplayLayout(); // Draw the layout of the display

    Mode activeLateralMode = decideActiveLateralMode(); // Decide the active lateral mode
    Mode armedLateralMode = decideArmedLateralMode(); // Decide the armed lateral mode
    Mode activeVerticalMode = decideActiveVerticalMode(); // Decide the active vertical mode
    Mode armedVerticalModeOne = decideArmedVerticalModeOne(); // Decide the armed vertical mode
    Mode armedVerticalModeTwo = decideArmedVerticalModeTwo(); // Decide the armed vertical mode

    drawActiveLateralMode(activeLateralMode); // Draw the active lateral mode
    drawArmedLateralMode(armedLateralMode); // Draw the armed lateral mode

    drawActiveVerticalMode(activeVerticalMode); // Draw the active vertical mode
    drawVerticalSetting(activeVerticalMode); // Draw the vertical setting
    drawArmedVerticalModeOne(armedVerticalModeOne); // Draw the armed vertical mode
    drawArmedVerticalModeTwo(armedVerticalModeTwo); // Draw the armed vertical mode

    _display.sendBuffer(); // Send the buffer to the display

}

void GFC600::clearDisplay()
{
    _display.clearBuffer(); // Clear the display buffer
    _display.sendBuffer(); // Send the buffer to the display
}


Mode GFC600::decideActiveLateralMode()
{
    // Highest priority: when LVL is active
    if (_lvl.state)
    {
        if (_vor.state && _loc.state && !_gps.state) return _loc;
        if (_vor.state && !_loc.state && !_gps.state) return _vor;
        if (_vor.state && _gps.state) return _gps;
        if (_bc.state) return _bc;
    }

    // Roll mode takes over if active
    if (_rol.state)
        return _rol;

    // Back Course / Heading logic
    if (_bc.state && !_hdg.state)
        return _bc;

    if (_hdg.state)
        return _hdg;

    // VOR/LOC/GPS fallback logic
    if (_vor.state && _loc.state && !_gps.state) return _loc;
    if (_apr.state && _loc.state && !_gps.state) return _loc;
    if (_vor.state && _gps.state) return _gps;
    if (_vor.state) return _vor;


    // Default fallback
    return _none;
}


Mode GFC600::decideArmedLateralMode()
{
    // Roll mode + armed conditions
    if (_rol.state)
    {
        if (_loc.state) return _loc;
        if (_bc.state) return _bc;
        if (_vor.state && !_gps.state) return _vor;
        if (_vor.state && _gps.state) return _gps;
    }

    // Heading mode + armed conditions
    if (_hdg.state)
    {
        if (_bc.state) return _bc;

        if (_vor.state && !_gps.state)
        {
            if (_loc.state) return _loc;
            return _vor;
        }

        if (_vor.state && _gps.state)
            return _gps;
    }

    // Default fallback
    return _none;
}


void GFC600::updateAltitudeFlashState()
{
    unsigned long now = millis();

    // Phase 1: Approaching selected altitude, ALTS is armed
    if (_within200ft &&
        (_pit.state || _ias.state || _vs.state || _ga.state) &&
        _alts.state)
    {
        // Start or re-trigger ALTS flashing if expired
        if (!_flashAlts || (now - _flashAltsStart > FLASH_DURATION)) {
            _flashAlts = true;
            _flashAltsStart = now;
            _dirty = true;
        }
    }
    else if (_flashAlts && (now - _flashAltsStart > FLASH_DURATION))
    {
        _flashAlts = false;
        _dirty = true;
    }

    // Phase 2: Within capture range (ALT mode becomes active)
    if (_alt.state && !_alts.state && _within50ft)
    {
        if (!_flashAlt || (now - _flashAltStart > FLASH_DURATION)) {
            _flashAlt = true;
            _flashAltStart = now;
            _dirty = true;
        }
    }
    else if (_flashAlt && (now - _flashAltStart > FLASH_DURATION))
    {
        _flashAlt = false;
        _dirty = true;
    }
}



void GFC600::drawFlashingText(uint8_t x, uint8_t y, const uint8_t* font, const char* text, bool flashEnabled)
{
    static const unsigned long FLASH_INTERVAL = 500;
    bool flashState = (millis() / FLASH_INTERVAL) % 2 == 0;



    _display.setFont(font);
    int textWidth = _display.getStrWidth(text);
    int textHeight = _display.getMaxCharHeight();

    int boxX = x - 2;
    int boxY = y - textHeight;
    int boxW = textWidth + 4;
    int boxH = textHeight + 4;

    _display.setDrawColor(BLACK);
    _display.drawBox(boxX, boxY, boxW, boxH);

    if (!flashEnabled || flashState) {
        _display.setDrawColor(WHITE);
        _display.drawStr(x, y, text);
    }
}






Mode GFC600::decideActiveVerticalMode()
{
    if (_alt.state && !_alts.state)
    {
        return _alt;
    }

    else if (_alt.state && _alts.state && _within50ft)
    {
        return _alt;
    }

    if (_alts.state)
    {
        return _alts;
    }

    if (_ias.state)
    {
        return _ias;
    }

    if (_vs.state)
    {
        return _vs;
    }

    if (_lvl.state)
    {
        return _lvl;
    }

    if (_pit.state)
    {
        return _pit;
    }



    if (_gps.state && _gs_active.state)
    {
        return _gp;
    }

    if (!_gps.state && _gs_active.state)
    {
        return _gs_active;
    }

    return _none;
}


void GFC600::altModeDrawingHandler(Mode mode)
{
    char altitude_str[20];
    int altitude = 0;

    switch (mode.value)
    {
        case ALTS:
            altitude = _altitude_lock_value;
            break;
        case ALT:
        default:
            altitude = _altitude_value_100ft;
            break;
    }

    snprintf(altitude_str, sizeof(altitude_str), "%d", altitude);
    int x_pos = computeXPosForValue(altitude);

    printTextToDisplay(x_pos, Y_ACTIVE, FONT_ACTIVE, altitude_str);
    printTextToDisplay(X_UNITS + 5, Y_UNITS, FONT_ARMED, "FT");
}





Mode GFC600::decideArmedVerticalModeOne()
{
    if (_pit.state)
    {
        return _alts;
    }

    if (_ias.state)
    {
        return _alts;
    }

    if (_vs.state)
    {
        return _vs;
    }

    if (_ga.state)
    {
        return _alts;
    }

    if (_alts.state)
    {
        return _alt;
    }

    return _none;
}


Mode GFC600::decideArmedVerticalModeTwo()
{
    
    if (_gps.state && _gs_armed.state)
    {
        return _gp;
    }

    if (!_gps.state && _gs_armed.state)
    {
        return _gs_armed;
    }

    return _none;
}






void GFC600::drawActiveVerticalMode(Mode mode)
{

    // Clear only the active vertical mode area
    clearArea(activeVerticalArea.x, activeVerticalArea.y, activeVerticalArea.width, activeVerticalArea.height);

    const char* name = mode.name;
    const uint8_t* font = FONT_ACTIVE;
    uint8_t x = X_DIV1 + 3;
    uint8_t y = Y_ACTIVE;

    bool isFlashingAlt  = (mode.value == ALT  && _flashAlt);
    bool isFlashingAlts = (mode.value == ALTS && _flashAlts);

    if (isFlashingAlt || isFlashingAlts)
    {
        drawFlashingText(x, y, font, name, true);
    }
    else
    {
        printTextToDisplay(x, y, font, name);
    }
}




void GFC600::drawArmedVerticalModeOne(Mode mode)
{

    
    // Clear only the armed vertical mode area
    clearArea(armedVerticalAreaOne.x, armedVerticalAreaOne.y, armedVerticalAreaOne.width, armedVerticalAreaOne.height); // Clear the active lateral mode area

    if (mode.value == ALT && _flashAlt)
    {
        drawFlashingText(X_DIV1 + 5, Y_ARMED, FONT_ARMED, mode.name, true); // Draw the active mode name
    }
    else
        // Draw the active mode name
    printTextToDisplay(X_DIV1 + 5, Y_ARMED, FONT_ARMED, mode.name); // Draw the active mode name
}





void GFC600::drawArmedVerticalModeTwo(Mode mode)
{
    
    // Clear only the armed vertical mode area
    clearArea(armedVerticalAreaTwo.x, armedVerticalAreaTwo.y, armedVerticalAreaTwo.width, armedVerticalAreaTwo.height); // Clear the active lateral mode area
    printTextToDisplay(X_UNITS+5, Y_ARMED, FONT_ARMED, mode.name); // Draw the active mode name
}



void GFC600::iasModeDrawingHandler()
{
    char ias_str[20];
    snprintf(ias_str, sizeof(ias_str), "%d", _ias_lock_value);
    int x_pos = computeXPosForValue(_ias_lock_value); // Compute the x position based on the value

    printTextToDisplay(x_pos, Y_ACTIVE, FONT_ACTIVE, ias_str);
    printTextToDisplay(X_UNITS, Y_UNITS, FONT_ARMED, "KTS");
}


void GFC600::initMode(Mode* mode, const char* name, ModesMessageId id) {
    mode->state = false;
    mode->name = name;
    mode->value = id;
}


void GFC600::initModes() {
    initMode(&_hdg, "HDG", HDG);
    initMode(&_rol, "ROL", ROL);
    initMode(&_vor, "VOR", VOR);
    initMode(&_gps, "GPS", GPS);
    initMode(&_apr, "APR", APR);
    initMode(&_bc,  "BC", BC);
    initMode(&_loc, "LOC", LOC);
    initMode(&_ga,  "GA", GA);
    initMode(&_lvl, "LVL", LVL);
    initMode(&_vnav, "VNAV", VNAV);
    initMode(&_vs,   "VS", VS);
    initMode(&_alt,  "ALT", ALT);
    initMode(&_alts, "ALTS", ALTS);
    initMode(&_ias,  "IAS", IAS);
    initMode(&_vpth, "VPTH", VPTH);
    initMode(&_gs_armed, "GS", GS_ARMED);
    initMode(&_gs_active, "GS", GS_ACTIVE);
    initMode(&_gp, "GP", GP);
    initMode(&_pit, "PIT", PIT);
    initMode(&_none, "NONE", NONE);
}


void GFC600::vsModeDrawingHandler()
{
    char vs_str[20];
    int abs_vs_value = abs(_vs_value);
    snprintf(vs_str, sizeof(vs_str), "%d", abs_vs_value);

    int x_pos = computeXPosForValue(_vs_value);
    int arrow_x = x_pos - ((_vs_value != 0) ? 12 : 0);  // Only offset if arrow is shown

    // Display the VS number
    printTextToDisplay(x_pos, Y_ACTIVE, FONT_ACTIVE, vs_str);

    // Display up/down arrow depending on sign
    if (_vs_value > 0)
    {
        drawArrow(arrow_x, Y_ACTIVE, UP_ARROW);
    }
    else if (_vs_value < 0)
    {
        drawArrow(arrow_x, Y_ACTIVE, DOWN_ARROW);
    }

    // Display unit
    printTextToDisplay(X_UNITS, Y_UNITS, FONT_ARMED, "FPM");
}

    



void GFC600::drawArrow(uint8_t x, uint8_t y, const char *arrow)
{
    _display.setFont(ARROW_FONT); // Set the font for the arrow
    _display.setDrawColor(WHITE); // Set the draw color to white
    _display.drawUTF8(x, y, arrow); // Draw the arrow at the specified position
}



void GFC600::drawVerticalSetting(Mode mode)
{

    clearArea(verticalSettingsArea.x, verticalSettingsArea.y, verticalSettingsArea.width, verticalSettingsArea.height); // Clear the vertical settings area
    
    if (mode.value == ALT || mode.value == ALTS)
    {
        altModeDrawingHandler(mode); // Draw the active mode name
    }

    if (mode.value == IAS)
    {
        iasModeDrawingHandler(); // Draw the active mode name
    }


    if (mode.value == VS)
    {
        vsModeDrawingHandler(); // Draw the active mode name
    }
    
}





void GFC600::clearArea(uint8_t x, uint8_t y, uint8_t width, uint8_t height)
{
    _display.setDrawColor(BLACK); // Set draw color to black (clear)
    _display.drawBox(x, y, width, height); // Clear the specified area

}



void GFC600::printTextToDisplay(uint8_t x, uint8_t y, const uint8_t *font, const char *text)
{
    _display.setDrawColor(WHITE); // Set draw color to white (draw)
    _display.setFont(font); // Set the font for the text
    _display.drawStr(x, y, text); // Draw the text at the specified position
}


void GFC600::drawActiveLateralMode(Mode mode)
{
    // Clear only the active lateral mode area
     
     clearArea(activeLateralArea.x, 0, activeLateralArea.width, activeLateralArea.height); // Clear the active mode area
     printTextToDisplay(X_LATERAL, Y_ACTIVE, FONT_ACTIVE, mode.name); // Draw the active mode name

}

void GFC600::drawArmedLateralMode(Mode mode)
{
  
     clearArea(armedLateralArea.x, armedLateralArea.y, armedLateralArea.width, armedLateralArea.height); // Clear the armed mode area
     printTextToDisplay(X_LATERAL + FONTS_LEFT_ALIGN_FACTOR, Y_ARMED, FONT_ARMED, mode.name); // Draw the armed mode name

}

int GFC600::computeXPosForValue(int value)
{

        int abs_val = abs(value);
        if (abs_val == 0) return X_DIV2 - 13;
        if (abs_val < 100) return X_VERTICAL_TWO_DIGITS;
        if (abs_val < 1000) return X_VERTICAL_THREE_DIGITS;
        if (abs_val < 10000) return X_VERTICAL_FOUR_DIGITS;
        return X_VERTICAL_FIVE_DIGITS;
    
}


void GFC600::update()
{
    updateAltitudeFlashState();

    if ((_flashAlts || _flashAlt) && (millis() - _lastFlashToggle >= FLASH_INTERVAL)) {
        _dirty = true;
        _lastFlashToggle = millis();
    }

    if (_dirty) {
        renderDisplay();
        _dirty = false;
    }
}


