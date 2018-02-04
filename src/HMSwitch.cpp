#include "HMSwitch.h"
#include "Images.h"
#include "Dialog.h"

void HMSwitch::updateState() {

  bool isOn = deviceHandler->isOn(adress);

  if ((lastUpdate == 0) || (c_isOn != isOn)) {
    tft->fillRect(frame.x,frame.y,frame.w,frame.h,0x0000);
    tft->drawRect(frame.x,frame.y,frame.w,frame.h,0xFFFF);
    tft->setFont(&Dialog_plain_12);
    ui->setTextAlignment(RIGHT);

    if (isOn) {
      tft->fillRect(frame.x,frame.y,frame.w,frame.h,0xFFFF);
      tft->setTextColor(0x0000);
      ui->setTextColor(0x0000, 0xFFFF);
      ui->drawString(frame.x+frame.w-10, frame.y+frame.h-14 , ctrl_name);
      tft->drawBitmap(frame.x+(frame.w/2)-24,frame.y+5,bulb,48,48,0x0000);
    } else {
      tft->setTextColor(0xFFFF);
      ui->setTextColor(0xFFFF, ILI9341_BLACK);
      ui->drawString(frame.x+frame.w-10, frame.y+frame.h-14 , ctrl_name);
      tft->drawBitmap(frame.x+(frame.w/2)-24,frame.y+5,bulb,48,48,0xFFFF);

    }



    lastUpdate = millis();
    c_isOn = isOn;
  }

}

byte HMSwitch::handleTouch(TS_Point point) {
  bool isOn = deviceHandler->isOn(adress);
  isOn = !isOn;
  deviceHandler->setOn(adress,isOn);
  updateState();
  return TOUCH_HANDLED;
}


void HMSwitch::drawDetail() {

}
