#include "HMDimmer.h"
#include "Images.h"
#include "Dialog.h"

HMDimmer::HMDimmer() {
  haszDetail = true;
  plusButton = HMInterfaceButton(Rect(130,140,90,80),"",true);
  plusButton.setImage("/gfx/btnplus.bmp",Point(25,10));

  minusButton = HMInterfaceButton(Rect(20,140,90,80),"",true);
  minusButton.setImage("/gfx/btnminus.bmp",Point(25,10));


  ButtonOff = HMInterfaceButton(Rect(10,250,50,80),"Off",true);
  ButtonOff.setImage("/gfx/btnempty.bmp",Point(5,10));
  ButtonOff.textOffset = Point(22,1);

  Button50 = HMInterfaceButton(Rect(90,250,50,80),"50",true);
  Button50.setImage("/gfx/btnempty.bmp",Point(5,10));
  Button50.textOffset = Point(22,0);

  ButtonOn = HMInterfaceButton(Rect(180,250,50,80),"100",true);
  ButtonOn.setImage("/gfx/btnempty.bmp",Point(5,10));
  ButtonOn.textOffset = Point(21,0);

}

void HMDimmer::updateState() {
  double level = deviceHandler->getLevel(adress);

  if ((lastUpdate == 0) || (_level != level)) {
    tft->fillRect(frame.x,frame.y,frame.w,frame.h,0x0000);
    tft->drawRect(frame.x,frame.y,frame.w,frame.h,0xFFFF);
    tft->setFont(&Dialog_plain_12);
    ui->setTextAlignment(RIGHT);

    if (level > 0) {
      tft->fillRect(frame.x,frame.y,frame.w,frame.h,0xFFFF);
      tft->setTextColor(0x0000);
      ui->setTextColor(0x0000, 0xFFFF);
      ui->drawString(frame.x+frame.w-10,frame.y+frame.h-14 , ctrl_name);
      tft->drawBitmap(frame.x+(frame.w/2)-24,frame.y+5,bulb,48,48,0x0000);
    } else {
      tft->setTextColor(0xFFFF);
      ui->setTextColor(0xFFFF, ILI9341_BLACK);
      ui->drawString(frame.x+frame.w-10, frame.y+frame.h-14 , ctrl_name);
      tft->drawBitmap(frame.x+(frame.w/2)-24,frame.y+5,bulb,48,48,0xFFFF);

    }

    lastUpdate = millis();
    _level = level;
  }
}


void HMDimmer::drawDetail() {
  uint16_t xpos = ((frame.w/2)-50);
  ui->drawBmp("/gfx/lightbulb.bmp",xpos,frame.y+30);
  // draw + / - Buttons
  drawButton(plusButton,false);
  drawButton(minusButton,false);
  drawButton(ButtonOff,false);
  drawButton(Button50,false);
  drawButton(ButtonOn,false);
  tft->setFont(&ArialRoundedMTBold_14);
  ui->setTextColor(0xFFFF, ILI9341_BLACK);
  ui->setTextAlignment(LEFT);
  ui->drawString(frame.x, frame.y + 20 ,ctrl_name);
  tft->setFont(&DejaVu_Sans_Bold_18);
  ui->setTextAlignment(LEFT);

  xpos = frame.x+(frame.w / 2) + 20;
  tft->fillRect(xpos,frame.y + 100,frame.w-xpos,20,0x0000);
  if (_level == 0) {
    ui->drawString(xpos, frame.y + 120 ,String("Off"));
  } else {
    ui->drawString(xpos, frame.y + 120 ,String(int(_level*100)) + " %");
  }
}

byte HMDimmer::handleTouch(TS_Point point) {
  // Check +
  if (buttonContainsPoint(plusButton,point)) {
    drawButton(plusButton,true);
    if (_level <= 1) {
      _level = _level + 0.1;
      drawDetail();
      deviceHandler->setLevel(adress,_level);
      return TOUCH_HANDLED;
    }
  }

  // Check -
  if (buttonContainsPoint(minusButton,point)) {
    drawButton(minusButton,true);
    if (_level > 0) {
      _level = _level - 0.1;
      drawDetail();
      deviceHandler->setLevel(adress,_level);
      return TOUCH_HANDLED;
    }
  }

  // Check Off
  if (buttonContainsPoint(ButtonOff,point)) {
    drawButton(ButtonOff,true);
    _level = 0;
    drawDetail();
    deviceHandler->setLevel(adress,_level);
    return TOUCH_HANDLED;
  }

  // Check 50
  if (buttonContainsPoint(Button50,point)) {
    drawButton(Button50,true);
    _level = 0.5;
    drawDetail();
    deviceHandler->setLevel(adress,_level);
    return TOUCH_HANDLED;
  }

  // Check 100
  if (buttonContainsPoint(ButtonOn,point)) {
    drawButton(ButtonOn,true);
    _level = 1;
    drawDetail();
    deviceHandler->setLevel(adress,_level);
    return TOUCH_HANDLED;
  }

  if (rectContainsPoint(Rect(frame.x,frame.y,frame.w,130),point)) {
    return TOUCH_HANDLED_BACKBUTTON;
  }

  return TOUCH_NOT_HANDLED;
}
