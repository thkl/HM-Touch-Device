#pragma once

#include <Arduino.h>
#include <Adafruit_STMPE610.h>
#include <Adafruit_ILI9341.h>
#include "HMDeviceHandler.h"
#include "ArialRoundedMTBold_14.h"
#include "ArialRoundedMTBold_36.h"
#include "DejaVuSans.h"

#include "GfxUi.h"
#include "HMInterfaceButton.h"

#define TOUCH_HANDLED 1
#define TOUCH_NOT_HANDLED 0
#define TOUCH_HANDLED_BACKBUTTON 3

class Rect {
public:
	Rect(void) : x(0), y(0), w(0),h(0) {}
	Rect(int16_t x, int16_t y, int16_t w, int16_t h) : x(x), y(y), w(w), h(h) {}
	int16_t x, y, w,h;
};


class HMControl {

   public:
    String adress;
    uint16_t x;
    uint16_t y;
    uint16_t w;
    uint16_t h;

    uint16_t color;
    String ctrl_name;
    long lastUpdate = 0;
    uint8_t onPage = 0;
    bool haszDetail = false;
    bool isFullscreen = false;
    int fontSize;

    HMDeviceHandler * deviceHandler;
    Adafruit_ILI9341 * tft;
    GfxUi * ui;

    virtual void updateState();
    virtual void applyFont();
    virtual ~HMControl () { Serial.println ("HMControl destructor."); }

    void init(Adafruit_ILI9341 * _tft, GfxUi * _ui,HMDeviceHandler * _deviceHandler);
    void setRect(uint16_t _x, uint16_t _y, uint16_t _w, uint16_t _h);

    void drawButton(HMInterfaceButton _ifb, bool pressed);

    bool touched(TS_Point point);

	  bool rectContainsPoint(uint16_t _x, uint16_t _y, uint16_t _w, uint16_t _h,TS_Point point);
	  bool buttonContainsPoint(HMInterfaceButton _ifb , TS_Point point);

    virtual byte handleTouch(TS_Point point){};
    virtual void drawDetail(){};
    virtual void willApear(bool fullscreen);
    virtual void willDissapear();

  private:
    Rect buttonRect;

};
