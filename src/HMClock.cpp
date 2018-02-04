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
			ui->setTextColor(0xFFFF, ILI9341_BLACK);
			applyFont();
      ui->setTextAlignment(RIGHT);
      String time = timeClient.getHours() + ":" + timeClient.getMinutes();
	  	ui->drawString(frame.x+frame.w, frame.y+ (frame.h/2), time);
      lastUpdate = millis();
 }

 byte HMClock::handleTouch(TS_Point point) {

 }

 void HMClock::setTimeZone(int offset){
	 timeClient = TimeClient(offset);
 }

 void HMClock::drawDetail() {

 }
