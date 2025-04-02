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
    drawActiveLateralMode(decideActiveLateralMode()); // Draw the active lateral mode
    drawArmedLateralMode(decideArmedLatealMode()); // Draw the armed lateral mode
    _display.sendBuffer(); // Send the buffer to the display

}

void GFC600::clearDisplay()
{
    _display.clearBuffer(); // Clear the display buffer
    _display.sendBuffer(); // Send the buffer to the display
}


Mode GFC600::decideActiveLateralMode()
{
    
    if (_rol.state)
    {
        return _rol;
    }

    if (_hdg.state && _vor.state && _gps.state)
    {
        return _hdg;
    }

    else if (_hdg.state && _vor.state && !_gps.state)
    {
        return _vor;
    }

    else if (_hdg.state && !_vor.state && !_gps.state)
    {
        return _hdg;
    }

    else if (!_hdg.state && _vor.state && !_gps.state)
    {
        return _vor;
    }

    else if (!_hdg.state && _vor.state && _gps.state)
    {
        return _gps;
    }

    else if (_hdg.state && !_vor.state && _gps.state)
    {
        return _hdg;
    }

    else if (!_hdg.state && _vor.state && _gps.state)
    {
    return _gps;
    }

    return {false, "NONE", NONE}; // Default case if no mode is active
}


Mode GFC600::decideArmedLatealMode()
{
    if (_hdg.state && _vor.state && _gps.state)
    {
        return _gps;
    }

    else if (_hdg.state && _vor.state && !_gps.state)
    {
        return _vor;
    }


    return {false, "NONE", NONE}; // Default case if no mode is active
}



void GFC600::clearArea(uint8_t x, uint8_t y, uint8_t width, uint8_t height)
{
    _display.setDrawColor(BLACK); // Set draw color to black (clear)
    _display.drawBox(x, y, width, height); // Clear the specified area
    _display.setDrawColor(WHITE); // Reset draw color to white (draw)
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
    


     clearArea(0, 0, X_DIV1 - X_LATERAL, CLEAR_HEIGHT+15); // Clear the active mode area
     printTextToDisplay(X_LATERAL, Y_ACTIVE, FONT_ACTIVE, mode.name.c_str()); // Draw the active mode name



}

void GFC600::drawArmedLateralMode(Mode mode)
{
  

     clearArea(X_LATERAL, Y_ARMED - CLEAR_HEIGHT, X_DIV1 - X_LATERAL, CLEAR_HEIGHT); // Clear the armed mode area
     printTextToDisplay(X_LATERAL + FONTS_LEFT_ALIGN_FACTOR, Y_ARMED, FONT_ARMED, mode.name.c_str()); // Draw the armed mode name

}

/*
void GFC600::drawActiveVerticalMode(VerticalMode mode)
{
    _u8g2.setFont(FONT_ACTIVE); // Set the font for active mode
    _u8g2.drawStr(X_VERTICAL + 5, 20, mode.name.c_str()); // Draw the active mode name
}*/

void GFC600::update()
{
    
    renderDisplay(); // Render the display
    // Do something which is required regulary
}
