#include "HMSwitch.h"
#include "Images.h"
#include "Dialog.h"

void HMSwitch::updateState() {

  bool isOn = deviceHandler->isOn(adress);

  if ((lastUpdate == 0) || (c_isOn != isOn)) {
    tft->fillRect(frame.x,frame.y,frame.w,frame.h,0x0000);
    if (isOn) {
      ui->drawBmp("/gfx/btn_lightbulb_on.bmp",frame.x,frame.y);
    } else {
      ui->drawBmp("/gfx/btn_lightbulb_off.bmp",frame.x,frame.y);
    }
    tft->setFont(&Dialog_plain_12);
    tft->setTextColor(0xFFFF);
    ui->setTextColor(0xFFFF, ILI9341_BLACK);
    ui->setTextAlignment(CENTER);
    ui->drawString(frame.x+(frame.w/2)-3, frame.y+frame.h, ctrl_name);
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
