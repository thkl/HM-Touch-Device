#pragma once

#include <Arduino.h>
#include <Adafruit_STMPE610.h>
#include <Adafruit_ILI9341.h>
#include "HMDeviceHandler.h"
#include "ArialRoundedMTBold_14.h"
#include "ArialRoundedMTBold_36.h"
#include "GfxUi.h"
#include "HMInterfaceButton.h"

#define TOUCH_HANDLED 1
#define TOUCH_NOT_HANDLED 0
#define TOUCH_HANDLED_BACKBUTTON 3

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


    HMDeviceHandler * deviceHandler;
    Adafruit_ILI9341 * tft;
    GfxUi * ui;

    virtual void updateState();
    virtual ~HMControl () { Serial.println ("HMControl destructor."); }

    void init(Adafruit_ILI9341 * _tft, GfxUi * _ui,HMDeviceHandler * _deviceHandler);
    void setRect(uint16_t _x, uint16_t _y, uint16_t _w, uint16_t _h);

    void drawButton(HMInterfaceButton _ifb, bool pressed);

    bool touched(TS_Point point);

	  bool rectContainsPoint(uint16_t _x, uint16_t _y, uint16_t _w, uint16_t _h,TS_Point point);
	  bool buttonContainsPoint(HMInterfaceButton _ifb , TS_Point point);

    virtual byte handleTouch(TS_Point point){};
    virtual void drawDetail(){};

    virtual void willDissapear(){};

};
