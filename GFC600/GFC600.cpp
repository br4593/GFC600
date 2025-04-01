#include "GFC600.h"
#include "allocateMem.h"
#include "commandmessenger.h"

/* **********************************************************************************
    This is just the basic code to set up your custom device.
    Change/add your code as needed.
********************************************************************************** */



GFC600::GFC600(uint8_t cs, uint8_t dc, uint8_t rst)
    : _cs(cs), _dc(dc), _rst(rst), _display(U8G2_R0, cs, dc, rst), _hdg(false, "HDG"),
      _rol(false, "ROL"), _nav(false, "NAV"), _apr(false, "APR"),
      _bc(false, "BC"), _vs(false, "VS"), _vnav(false, "VNAV"),
      _alt(false, "ALT"), _alts(false, "ALTS")
{

}

Mode GFC600::decideActiveLateralMode()
{

    if (_hdg.getState()){
        return _hdg; // Return the active heading mode
    }
    if (_rol.getState()){
        return _rol; // Return the active roll mode
    }
    if (_nav.getState()){
        return _nav; // Return the active navigation mode
    }
    if (_apr.getState()){
        return _apr; // Return the active approach mode
    }
    if (_bc.getState()){
        return _bc; // Return the active back course mode
    }
    if (_nav.getState()){
        return _nav; // Return the active navigation mode
    }

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

    // do something according your messageID
    switch (messageID) {
    case -1:
        // tbd., get's called when Mobiflight shuts down
    case -2:
        // tbd., get's called when PowerSavingMode is entered
    case HDG:
        output = (uint16_t)data;
        data   = output;
        _hdg.setState(data); // Set the state of the heading mode
        break;
    case ROL:
        /* code */
        break;
    case NAV:
        output = (uint16_t)data;
        data   = output;
        _nav.setState(data); // Set the state of the heading mode
        /* code */
        break;
    default:
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
    _display.sendBuffer(); // Send the buffer to the display

}

void GFC600::clearDisplay()
{
    _display.clearBuffer(); // Clear the display buffer
    _display.sendBuffer(); // Send the buffer to the display
}


void GFC600::drawActiveLateralMode(Mode mode)
{
    // Clear the lateral area before drawing the active mode
    _display.setDrawColor(0); // Set draw color to black (clear)
    _display.drawBox(X_LATERAL, 0, X_DIV1 - X_LATERAL, DISPLAY_HEIGHT); // Clear the lateral area
    _display.setDrawColor(1); // Reset draw color to white (draw)
    _display.setFont(FONT_ACTIVE); // Set the font for active mode
    _display.drawStr(X_LATERAL + 5, 20, mode.getName().c_str()); // Draw the active mode name
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
