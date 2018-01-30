#include "HMSwitch.h"
#include "Images.h"
#include "Dialog.h"
#include "TimeClient.h"
#include "HMClock.h"

HMClock::HMClock() {
	timeClient = TimeClient(-1);
}


 void HMClock::updateState() {
 	  	timeClient.updateTime();
			tft->setTextColor(0xFFFF);
      tft->setFont(&Dialog_plain_12);
      ui->setTextAlignment(RIGHT);
      String time = timeClient.getHours() + ":" + timeClient.getMinutes();
	  	ui->drawString(x+w, y+ (h/2), time);
      lastUpdate = millis();
 }

 byte HMClock::handleTouch(TS_Point point) {

 }


 void HMClock::drawDetail() {

 }
