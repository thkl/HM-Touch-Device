
#include "HMControl.h"

  void HMControl::init(Adafruit_ILI9341 * _tft, GfxUi * _ui,HMDeviceHandler * _deviceHandler){
    ui = _ui;
    tft = _tft;
    deviceHandler = _deviceHandler;
  }

   void HMControl::setRect(Rect _frame) {
    frame = _frame;
    buttonRect = _frame;
   }

  void HMControl::updateState() {
      tft->drawRect(frame.x,frame.y,frame.w,frame.h,ILI9341_BLACK);
  }

  void HMControl::applyFont(){
    switch (fontSize) {
      case 0:
        tft->setFont(&DejaVu_Sans_12);
        break;

      case 1:
        tft->setFont(&DejaVu_Sans_14);
        break;

      case 2:
        tft->setFont(&DejaVu_Sans_16);
        break;

      case 3:
        tft->setFont(&DejaVu_Sans_18);
        break;

      case 4:
        tft->setFont(&DejaVu_Sans_Bold_14);
        break;

      case 5:
        tft->setFont(&DejaVu_Sans_Bold_16);
        break;

      case 6:
        tft->setFont(&DejaVu_Sans_Bold_18);
        break;
    }
  }

  bool HMControl::touched(TS_Point point) {
    return rectContainsPoint(frame,point);
  }


  bool HMControl::rectContainsPoint(Rect _frame,TS_Point point) {
	if ((point.x >= _frame.x) && (point.x <= _frame.x+_frame.w) && (point.y >= _frame.y) && (point.y <= _frame.y+_frame.h)) {
      return true;
    } else {
      return false;
    }
  }

  bool HMControl::buttonContainsPoint(HMInterfaceButton _ifb , TS_Point point) {
	if ((point.x >= _ifb.frame.x) && (point.x <= _ifb.frame.x+_ifb.frame.w) && (point.y >= _ifb.frame.y) && (point.y <= _ifb.frame.y+_ifb.frame.h)) {
      return true;
    } else {
      return false;
    }
  }


  void HMControl::drawButton(HMInterfaceButton _ifb, bool pressed) {
    // A Button with an image
  if (_ifb.img_name != "") {
    ui->drawBmp(_ifb.img_name,_ifb.frame.x+_ifb.imgPosition.x,_ifb.frame.y+_ifb.imgPosition.y);
    // If there is a caption draw that
    if (_ifb.caption != "") {
      if (pressed) {
        tft->setTextColor(_ifb.invForeColor);
      } else {
        tft->setTextColor(_ifb.foreColor);
      }
      tft->setFont(&DejaVu_Sans_12);
      ui->setTextAlignment(CENTER);
      ui->setTextColor(_ifb.foreColor, _ifb.backColor);
      ui->drawString( _ifb.frame.x + _ifb.imgPosition.x + _ifb.textOffset.x, _ifb.frame.y + (_ifb.frame.h/2) + _ifb.textOffset.y - 3 , _ifb.caption);
    }
	}
   // Only draw the caption
  else {

    if (_ifb.large) {
      tft->setFont(&ArialRoundedMTBold_36);
    } else {
      tft->setFont(&ArialRoundedMTBold_14);
    }

    tft->drawRoundRect(_ifb.frame.x,_ifb.frame.y,_ifb.frame.w,_ifb.frame.h,4,_ifb.foreColor);

	  if (pressed) {
	  	tft->fillRoundRect(_ifb.frame.x,_ifb.frame.y,_ifb.frame.w,_ifb.frame.h,4,_ifb.invBackColor);
	  	tft->setTextColor(_ifb.invForeColor);
	  	ui->setTextColor(_ifb.invForeColor,_ifb.invBackColor);
	  } else {
	  	tft->fillRoundRect((_ifb.frame.x+1),(_ifb.frame.y+1),(_ifb.frame.w-2),(_ifb.frame.h-2),4,_ifb.backColor);
	  	tft->setTextColor(_ifb.foreColor);
	  	ui->setTextColor(_ifb.foreColor,_ifb.backColor);
	  }
    ui->setTextAlignment(CENTER);
    ui->drawString( (_ifb.frame.w /2 )+ _ifb.frame.x , _ifb.frame.y + (_ifb.frame.h/2) + 10, _ifb.caption);
  }


  }

  void HMControl::willDissapear() {
    // Restore Coordinates
    Serial.println("Restore Coordinates");
    frame = buttonRect;
  }

  void HMControl::willApear(bool fullscreen) {
    isFullscreen = fullscreen;
    if (fullscreen) {
      frame = Rect(0,0,tft->width(),tft->height());
    }
  }
