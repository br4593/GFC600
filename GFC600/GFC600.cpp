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
    _gs   = {false, "GS", GS};
    _gp   = {false, "GP", GP};
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

        case GS:
            setState(&_gs, data);
            break;

        case GP:
            setState(&_gp, data);
            break;

        case PIT:
            setState(&_pit, data);
            break;

        case ALTITUDE_VAL:
            // Set the altitude value
            _altitude_value = data;
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
    Mode armedLateralMode = decideArmedLatealMode(); // Decide the armed lateral mode
    Mode activeVerticalMode = decideActiveVertialMode(); // Decide the active vertical mode
    Mode armedVerticalModeOne = decideArmedVerticalModeOne(); // Decide the armed vertical mode

    drawActiveLateralMode(activeLateralMode); // Draw the active lateral mode
    drawArmedLateralMode(armedLateralMode); // Draw the armed lateral mode

    drawActiveVerticalMode(activeVerticalMode); // Draw the active vertical mode
    drawVerticalSetting(activeVerticalMode); // Draw the vertical setting
    drawArmedVerticalModeOne(armedVerticalModeOne); // Draw the armed vertical mode


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
    if (_vor.state && _loc.state) return _loc;
    if (_vor.state && _gps.state) return _gps;
    if (_vor.state) return _vor;
    if (_gps.state) return _gps;

    // Default fallback
    return {false, "NONE", NONE};
}


Mode GFC600::decideArmedLatealMode()
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
    return {false, "NONE", NONE};
}





Mode GFC600::decideActiveVertialMode()
{
    if (_alt.state)
    {
        return _alt;
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

    if (_alts.state)
    {
        return _alts;
    }



    return {false, "NONE", NONE}; // Default case if no mode is active
}


void GFC600::altModeDrawingHandler()
{
    char altitude_str[20]; // Buffer for the altitude value
    sprintf(altitude_str, "%d", _altitude_value); // Convert integer to string

    if (_altitude_value > 999 && _altitude_value < 10000)
    {
        printTextToDisplay(X_ALT_FOUR_DIGITS, Y_ACTIVE, FONT_ACTIVE, altitude_str); // Draw the active mode name
    }
    else if (_altitude_value < 1000)
    {

        printTextToDisplay(X_ALT_THREE_DIGITS, Y_ACTIVE, FONT_ACTIVE, altitude_str); // Draw the active mode name
    }
    else if (_altitude_value > 9999)
    {

        printTextToDisplay(X_ALT_FIVE_DIGITS, Y_ACTIVE, FONT_ACTIVE, altitude_str); // Draw the active mode name
    }

    else if(_altitude_value == 0)
    {

        printTextToDisplay(X_DIV2, Y_ACTIVE, FONT_ACTIVE, altitude_str); // Draw the active mode name
    }

    printTextToDisplay(X_UNITS-CORRECTION_FACTOR,Y_UNITS,FONT_ARMED,"FT");
}


Mode GFC600::decideArmedVerticalModeOne()
{
    if (_pit.state || _ias.state || _vs.state || _ga.state)
    {
        return _alts;
    }

    if (_alts.state)
    {
        return _alt;
    }

    return {false, "NONE", NONE}; // Default case if no mode is active
}


void GFC600::drawActiveVerticalMode(Mode mode)
{
    // Clear only the active vertical mode area
    clearArea(activeVerticalArea.x, activeVerticalArea.y, activeVerticalArea.width, activeVerticalArea.height); // Clear the active lateral mode area
    printTextToDisplay(X_DIV1 + FONTS_LEFT_ALIGN_FACTOR, Y_ACTIVE, FONT_ACTIVE, mode.name.c_str()); // Draw the active mode name
}



void GFC600::drawArmedVerticalModeOne(Mode mode)
{
    // Clear only the armed vertical mode area
    clearArea(armedVerticalArea.x, armedVerticalArea.y, armedVerticalArea.width, armedVerticalArea.height); // Clear the active lateral mode area
    printTextToDisplay(X_DIV1 + FONTS_LEFT_ALIGN_FACTOR, Y_ARMED, FONT_ARMED, mode.name.c_str()); // Draw the active mode name
}


void GFC600::iasModeDrawingHandler()
    {
        char ias_str[20]; // Buffer for the altitude value
        sprintf(ias_str, "%d", _ias_lock_value); // Convert integer to string

        if (_ias_lock_value < 999 && _ias_lock_value > 99)
        {
            printTextToDisplay(X_ALT_THREE_DIGITS-3, Y_ACTIVE, FONT_ACTIVE, ias_str); // Draw the active mode name
        }
        else if (_ias_lock_value < 100 && _ias_lock_value > 9)
        {
            printTextToDisplay(X_ALT_TWO_DIGITS- CORRECTION_FACTOR, Y_ACTIVE, FONT_ACTIVE, ias_str); // Draw the active mode name
        }

        printTextToDisplay(X_UNITS-CORRECTION_FACTOR-2,Y_UNITS,FONT_ARMED,"KTS");

    }

void GFC600::vsModeDrawingHandler()
{

    char vs_str[20]; // Buffer for the altitude value
    sprintf(vs_str, "%d", _vs_value); // Convert integer to string

    if (_vs_value < 999 && _vs_value > 99)
    {
        printTextToDisplay(X_ALT_THREE_DIGITS-3, Y_ACTIVE, FONT_ACTIVE, vs_str); // Draw the active mode name
        u8g2.setFont(u8g2_font_unifont_t);
        u8g2.drawUTF8(X_ALT_THREE_DIGITS - 10, Y_ACTIVE, "\u2191"); // Up arrow (↑)
    }
    else if (_vs_value < 100 && _vs_value > 9)
    {
        printTextToDisplay(X_ALT_TWO_DIGITS- CORRECTION_FACTOR, Y_ACTIVE, FONT_ACTIVE, vs_str); // Draw the active mode name
    }

    printTextToDisplay(X_UNITS-CORRECTION_FACTOR-2,Y_UNITS,FONT_ARMED,"FPM");
}


void GFC600::drawVerticalSetting(Mode mode)
{
    
    if (mode.name == "ALT")
    {
        altModeDrawingHandler(); // Draw the active mode name
    }

    if (mode.name == "IAS")
    {
        iasModeDrawingHandler(); // Draw the active mode name
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
    
    renderDisplay(); // Render the display
    // Do something which is required regulary
}
