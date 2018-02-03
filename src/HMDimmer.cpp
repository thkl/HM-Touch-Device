#include "HMDimmer.h"
#include "Images.h"
#include "Dialog.h"

HMDimmer::HMDimmer() {
  haszDetail = true;
  plusButton = HMInterfaceButton(130,140,90,80,"+",true);
  plusButton.setImage("/gfx/btnplus.bmp",25,10);

  minusButton = HMInterfaceButton(20,140,90,80,"-",true);
  minusButton.setImage("/gfx/btnminus.bmp",25,10);

}

void HMDimmer::updateState() {
  double level = deviceHandler->getLevel(adress);

  if ((lastUpdate == 0) || (_level != level)) {
     tft->fillRect(x,y,w,h,0x0000);
     tft->drawRect(x,y,w,h,0xFFFF);
     tft->setFont(&Dialog_plain_12);
     ui->setTextAlignment(RIGHT);

     if (level > 0) {
       tft->fillRect(x,y,w,h,0xFFFF);
       tft->setTextColor(0x0000);
       ui->setTextColor(0x0000, 0xFFFF);
       ui->drawString(x+w-10, y+h-14 , ctrl_name);
       tft->drawBitmap(x+(w/2)-24,y+5,bulb,48,48,0x0000);
     } else {
       tft->setTextColor(0xFFFF);
       ui->setTextColor(0xFFFF, ILI9341_BLACK);
       ui->drawString(x+w-10, y+h-14 , ctrl_name);
       tft->drawBitmap(x+(w/2)-24,y+5,bulb,48,48,0xFFFF);

     }

     lastUpdate = millis();
     _level = level;
 }
}


void HMDimmer::drawDetail() {
  ui->setTextColor(0xFFFF, ILI9341_BLACK);
  ui->setTextAlignment(LEFT);
  uint16_t xpos = ((w/2)-50);
  ui->drawBmp("/gfx/lightbulb.bmp",xpos,y+30);
  // draw + / - Buttons
  drawButton(plusButton,false);
  drawButton(minusButton,false);
  ui->drawString(x, y + 20 ,ctrl_name);
}

byte HMDimmer::handleTouch(TS_Point point) {
  if (rectContainsPoint(x,y,w,130,point)) {
    return TOUCH_HANDLED_BACKBUTTON;
  }

  return TOUCH_NOT_HANDLED;
}
