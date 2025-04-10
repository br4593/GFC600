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
    _hdg = {false, "HDG", HDG};
    _rol = {false, "ROL", ROL};
    _vor = {false, "VOR", VOR};
    _gps = {false, "GPS", GPS};
    _apr = {false, "APR", APR};
    _bc  = {false, "BC", BC};
    _loc = {false, "LOC", LOC};
    _ga  = {false, "GA", GA};
    _lvl = {false, "LVL", LVL};
    _vnav = {false, "VNAV", VNAV};
    _vs   = {false, "VS", VS};
    _alt  = {false, "ALT", ALT};
    _alts = {false, "ALTS", ALTS};
    _ias  = {false, "IAS", IAS};
    _vpth = {false, "VPTH", VPTH};
    _gs_armed   = {false, "GS", GS_ARMED};
    _gs_active   = {false, "GS", GS_ACTIVE};
    _gp   = {false, "GP", GP};
    _pit = {false, "PIT", PIT};
    _none = {false, "NONE", NONE};


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
            break;

        case HDG:
            setState(&_hdg, data);
            break;

        case VOR:
            setState(&_vor, data);
            break;

        case GPS:
            setState(&_gps, data);
            break;

        case APR:
            setState(&_apr, data);
            break;

        case BC:
            setState(&_bc, data);
            break;

        case LVL:
            setState(&_lvl, data);
            break;

        case GA:
            setState(&_ga, data);
            break;

        case LOC:
            setState(&_loc, data);
            break;  

        case VS:
            setState(&_vs, data);
            break;

        case IAS:
            setState(&_ias, data);
            break;

        case ALT:
            setState(&_alt, data);
            break;

        case ALTS:
            setState(&_alts, data);
            break;

        case VPTH:
            setState(&_vpth, data);
            break;

        case VNAV:
            setState(&_vnav, data);
            break;

        case GS_ARMED:
            setState(&_gs_armed, data);
            break;

        case GS_ACTIVE:
            setState(&_gs_active, data);
            break;


        case PIT:
            setState(&_pit, data);
            break;

        case ALTITUDE_VALUE_100FT:
            // Set the altitude value
            _altitude_value_100ft = data;
            break;

        case ALTITUDE_WITHIN_50FT:
            // Set the altitude value
            _within50ft = data;
            break;

        case ALTITUDE_WITHIN_200FT:
            // Set the altitude value
            _within200ft = data;
            break;

        case ALTITUDE_LOCK_VAL:
            // Set the altitude lock value
            _altitude_lock_value = data;
            break;

        case VS_VAL:
            // Set the vertical speed value
            _vs_value = data;
            break;
        
        case IAS_VAL:
            // Set the indicated airspeed value
            _ias_lock_value = data;
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
    Mode activeVerticalMode = decideActiveVertialMode(); // Decide the active vertical mode
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


void GFC600::flashModeTranistion()
{


    //int delta = abs(_altitude_value_10ft - _altitude_lock_value); // Assuming _altitude_value_100ft is in 100s of feet

    // Phase 1: Approaching selected altitude, ALTS is armed
    if (_within200ft)
    {
        if ((_pit.state || _ias.state || _vs.state || _ga.state) && _alts.state)
        {
            if (!_flashAlts) {
                _flashAlts = true;
                _flashAltsStart = millis();
            }
        }
    }

    // Stop flashing ALTS after duration
    if (_flashAlts && millis() - _flashAltsStart > FLASH_DURATION)
    {
        _flashAlts = false;
    }

    // Phase 2: Within capture range (ALT mode becomes active)
    if (_alt.state && !_alts.state && _within50ft)
    {
        if (!_flashAlt) {
            _flashAlt = true;
            _flashAltStart = millis();
        }
    }

    // Stop flashing ALT after duration
    if (_flashAlt && millis() - _flashAltStart > FLASH_DURATION)
    {
        _flashAlt = false;
    }
}


void GFC600::drawFlashingText(uint8_t x, uint8_t y, const uint8_t* font, const char* text, bool flashEnabled)
{
    static const unsigned long FLASH_INTERVAL = 500;
    bool flashState = (millis() / FLASH_INTERVAL) % 2 == 0;

    _display.setFont(font);
    int textWidth = _display.getStrWidth(text);
    int textHeight = _display.getMaxCharHeight();

    // Define bounding box for cleanup
    int boxX = x - 2;
    int boxY = y - textHeight;
    int boxW = textWidth + 4;
    int boxH = textHeight + 4;

    // Always clear the area first
    _display.setDrawColor(BLACK);
    _display.drawBox(boxX, boxY, boxW, boxH);

    // Draw text based on flash state
    if (!flashEnabled || flashState)
    {
        // Show normal white text (no flashing or during flash phase)
        _display.setDrawColor(WHITE);
        _display.drawStr(x, y, text);
    }
    // else: during flash "off" phase, we draw nothing (text remains hidden)
}





Mode GFC600::decideActiveVertialMode()
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
    

    int altitude;

    if (mode.name == "ALTS")
    {
        altitude = _altitude_lock_value;
        sprintf(altitude_str, "%d", _altitude_lock_value);
    }
    else
    {
        altitude = _altitude_value_100ft;
        sprintf(altitude_str, "%d", _altitude_value_100ft);
    }

    int abs_alt = abs(altitude);
    int x_pos = X_VERTICAL_THREE_DIGITS; // Default for small numbers

    if (abs_alt == 0)
    {
        x_pos = X_DIV2 - 13;
    }
    else if (abs_alt < 1000)
    {
        x_pos = X_VERTICAL_THREE_DIGITS;
    }
    else if (abs_alt < 10000)
    {
        x_pos = X_VERTICAL_FOUR_DIGITS;
    }
    else
    {
        x_pos = X_VERTICAL_FIVE_DIGITS;
    }

    printTextToDisplay(x_pos, Y_ACTIVE, FONT_ACTIVE, altitude_str);
    printTextToDisplay(X_UNITS+5, Y_UNITS, FONT_ARMED, "FT");
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

    const char* name = mode.name.c_str();
    const uint8_t* font = FONT_ACTIVE;
    uint8_t x = X_DIV1 + 3;
    uint8_t y = Y_ACTIVE;

    bool isFlashingAlt  = (mode.name == "ALT"  && _flashAlt);
    bool isFlashingAlts = (mode.name == "ALTS" && _flashAlts);

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
    clearArea(armedVerticalArea.x, armedVerticalArea.y, armedVerticalArea.width, armedVerticalArea.height); // Clear the active lateral mode area

    if (mode.name == "ALT" && _flashAlt)
    {
        drawFlashingText(X_DIV1 + 5, Y_ARMED, FONT_ARMED, mode.name.c_str(), true); // Draw the active mode name
    }
    else
        // Draw the active mode name
    printTextToDisplay(X_DIV1 + 5, Y_ARMED, FONT_ARMED, mode.name.c_str()); // Draw the active mode name
}



void GFC600::drawArmedVerticalModeTwo(Mode mode)
{
    // Clear only the armed vertical mode area
    clearArea(armedVerticalArea.x, armedVerticalArea.y, armedVerticalArea.width, armedVerticalArea.height); // Clear the active lateral mode area
    printTextToDisplay(X_UNITS+5, Y_ARMED, FONT_ARMED, mode.name.c_str()); // Draw the active mode name
}


void GFC600::iasModeDrawingHandler()
{
    char ias_str[20];
    sprintf(ias_str, "%d", _ias_lock_value);

    int abs_ias = abs(_ias_lock_value);
    int x_pos = X_VERTICAL_THREE_DIGITS; // Default position

    if (_ias_lock_value == 0)
    {
        x_pos = X_DIV2 - 13;
    }
    else if (abs_ias < 10)
    {
        x_pos = X_VERTICAL_TWO_DIGITS;
    }
    else if (abs_ias < 100)
    {
        x_pos = X_VERTICAL_TWO_DIGITS;
    }
    else if (abs_ias < 1000)
    {
        x_pos = X_VERTICAL_THREE_DIGITS;
    }
    else
    {
        x_pos = X_VERTICAL_FOUR_DIGITS;
    }

    printTextToDisplay(x_pos, Y_ACTIVE, FONT_ACTIVE, ias_str);
    printTextToDisplay(X_UNITS, Y_UNITS, FONT_ARMED, "KTS");
}


    void GFC600::vsModeDrawingHandler()
    {
        char vs_str[20];
        
        int abs_vs_value = abs(_vs_value);
        sprintf(vs_str, "%d", abs_vs_value);
        int x_pos = X_DIV2 - 13;  // Default center alignment
        int arrow_x = x_pos;
        bool draw_up_arrow = _vs_value > 0;
        bool draw_down_arrow = _vs_value < 0;

        if (_vs_value == 0)
        {
            x_pos = X_DIV2 - 13; // Center alignment
            arrow_x = x_pos;
        }
        else if (abs_vs_value < 10 && _vs_value != 0)
        {
            x_pos = X_VERTICAL_TWO_DIGITS;
            arrow_x = x_pos - 12;
        }
    
        else if (abs_vs_value < 100)
        {
            x_pos = X_VERTICAL_TWO_DIGITS;
            arrow_x = x_pos - 12;
        }
        else if (abs_vs_value < 1000)
        {
            x_pos = X_VERTICAL_THREE_DIGITS;
            arrow_x = x_pos - 12;
        }
        else if (abs_vs_value < 10000)
        {
            x_pos = X_VERTICAL_FOUR_DIGITS;
            arrow_x = x_pos - 12;
        }
        else
        {
            x_pos = X_VERTICAL_FIVE_DIGITS;
            arrow_x = x_pos - 12;
        }
    
        printTextToDisplay(x_pos, Y_ACTIVE, FONT_ACTIVE, vs_str);
    
        if (draw_up_arrow)
        {
            drawArrow(arrow_x, Y_ACTIVE, UP_ARROW);
        }
        else if (draw_down_arrow)
        {
            drawArrow(arrow_x, Y_ACTIVE, DOWN_ARROW);
        }
    
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
    
    if (mode.name == "ALT" || mode.name == "ALTS")
    {
        altModeDrawingHandler(mode); // Draw the active mode name
    }

    if (mode.name == "IAS")
    {
        iasModeDrawingHandler(); // Draw the active mode name
    }

    if (mode.name == "VS")
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
    
     clearArea(activeLateralArea.x, activeLateralArea.y, activeLateralArea.width, activeLateralArea.height); // Clear the active mode area
     printTextToDisplay(X_LATERAL, Y_ACTIVE, FONT_ACTIVE, mode.name.c_str()); // Draw the active mode name

}

void GFC600::drawArmedLateralMode(Mode mode)
{
  
     clearArea(armedLateralArea.x, armedLateralArea.y, armedLateralArea.width, armedLateralArea.height); // Clear the armed mode area
     printTextToDisplay(X_LATERAL + FONTS_LEFT_ALIGN_FACTOR, Y_ARMED, FONT_ARMED, mode.name.c_str()); // Draw the armed mode name

}


void GFC600::update()
{
    flashModeTranistion();
    renderDisplay(); // Render the display
    // Do something which is required regulary
}
