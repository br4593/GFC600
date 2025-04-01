#include "GFC600.h"
#include "allocateMem.h"
#include "commandmessenger.h"

/* **********************************************************************************
    This is just the basic code to set up your custom device.
    Change/add your code as needed.
********************************************************************************** */

LateralMode hdg = {"HDG", false};

GFC600::GFC600(uint8_t cs, uint8_t dc, uint8_t rst)
    : _cs(cs), _dc(dc), _rst(rst), _u8g2(U8G2_R0, cs, dc, rst)
{
}




void GFC600::begin()
{
}

void GFC600::attach()
{

    if(_u8g2.begin()) {
        _initialised = true;
        _u8g2.clearBuffer();
        _u8g2.setFont(u8g2_font_ncenB08_tr); // Choose a suitable font
        _u8g2.drawStr(0, 10, "Device Started"); // Display the message
        _u8g2.sendBuffer();
        delay(1000); // Wait for a second to show the message
        _u8g2.clearBuffer(); // Clear the buffer for the next drawing
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
    case 0:
        output = (uint16_t)data;
        data   = output;
        if (data == 1)
        {
            drawActiveLateralMode(hdg); // Draw the active mode name
        }
        break;
    case 1:
        /* code */
        break;
    case 2:
        /* code */
        break;
    default:
        break;
    }
}

void GFC600::drawDisplayLayout()
{
    _u8g2.drawLine(X_DIV1, 0, X_DIV1, DISPLAY_HEIGHT); // Vertical line between Lateral and Vertical section
    _u8g2.drawLine(X_DIV2, 0, X_DIV2, DISPLAY_HEIGHT); // Vertical line between Vertical and Message section
}

void GFC600::renderDisplay()
{
    drawDisplayLayout(); // Draw the layout of the display
    _u8g2.sendBuffer(); // Send the buffer to the display
}

void GFC600::clearDisplay()
{
    _u8g2.clearBuffer(); // Clear the display buffer
    _u8g2.sendBuffer(); // Send the buffer to the display
}

void GFC600::drawActiveLateralMode(LateralMode mode)
{
    _u8g2.setFont(FONT_ACTIVE); // Set the font for active mode
    _u8g2.drawStr(X_LATERAL + 5, 10, mode.name.c_str()); // Draw the active mode name
}

void GFC600::update()
{
    renderDisplay(); // Render the display
    // Do something which is required regulary
}
