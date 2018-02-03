#pragma once

#include <Arduino.h>
#include <Adafruit_ILI9341.h>
#include <Adafruit_STMPE610.h>
#include "HMControl.h"

class PageManager {

   public:
    int currentPage = 0;
    int currentControl = -1;
    long lastDetailPageUpdate;
    uint16_t voltage;

    void init(Adafruit_ILI9341 * _tft, GfxUi * _ui);
    bool touched(TS_Point point,bool longPress);
    void updatePage();
    void jumpBack();
    void updateVoltage(uint16_t voltage);
    void addControl(HMControl *control, int detailPageId);

   private:
     Adafruit_ILI9341 * tft;
	 GfxUi * ui;
     HMControl * devices [5];
     int deviceCount = 0;

};
