#include "core/powerSave.h"
#include "core/utils.h"
#include <M5Unified.h>
#include <interface.h>

/***************************************************************************************
** Function name: _setup_gpio()
** Description: initial setup for the device
****************************************************************************************/
void _setup_gpio() {
    auto cfg = M5.config();
    M5.begin(cfg);
    pinMode(SEL_BTN, INPUT);
    pinMode(UP_BTN, INPUT);
}

/***************************************************************************************
** Function name: _post_setup_gpio()
** Description: second stage gpio setup
****************************************************************************************/
void _post_setup_gpio() {}

/***************************************************************************************
** Function name: getBattery()
****************************************************************************************/
int getBattery() {
    int percent = M5.Power.getBatteryLevel();
    return (percent < 0) ? 0 : (percent >= 100) ? 100 : percent;
}

/*********************************************************************
** Function: setBrightness
**********************************************************************/
void _setBrightness(uint8_t brightval) { M5.Display.setBrightness(brightval); }

/*********************************************************************
** Function: InputHandler
**********************************************************************/
void InputHandler(void) {
    static unsigned long tm = 0;
    if (millis() - tm < 180 && !LongPress) return;

    M5.update();
    checkPowerSaveTime();

    bool selPressed = (digitalRead(SEL_BTN) == BTN_ACT);
    bool upPressed = (digitalRead(UP_BTN) == BTN_ACT);
    bool anyPressed = selPressed || upPressed;

    PrevPress = false;
    NextPress = false;
    SelPress = false;
    EscPress = false;
    AnyKeyPress = false;

    if (anyPressed) {
        tm = millis();
        if (wakeUpScreen()) return;
        AnyKeyPress = true;
    }

    if (selPressed) {
        SelPress = true;
    }

    if (upPressed) {
        PrevPress = true;
        EscPress = true;
    }
}

/*********************************************************************
** Function: powerOff
**********************************************************************/
void powerOff() { M5.Power.powerOff(); }

void goToDeepSleep() { M5.Power.deepSleep(); }

/*********************************************************************
** Function: checkReboot
**********************************************************************/
void checkReboot() {}

bool isCharging() {
    return M5.Power.isCharging() || M5.Power.getBatteryCurrent() > 0;
}
