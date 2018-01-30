
#include "HMControl.h"

  void HMControl::init(Adafruit_ILI9341 * _tft, GfxUi * _ui,HMDeviceHandler * _deviceHandler){
    ui = _ui;
    tft = _tft;
    deviceHandler = _deviceHandler;
  }
    
   void HMControl::setRect(uint16_t _x, uint16_t _y, uint16_t _w, uint16_t _h) {

    x = _x;
    y = _y;
    w = _w;
    h = _h;
   
   }

  void HMControl::updateState() {
      tft->drawRect(x,y,w,h,0xFFFF);
  }

  bool HMControl::touched(TS_Point point) {
    return rectContainsPoint(x,y,w,h,point);
  }
  
  
  bool HMControl::rectContainsPoint(uint16_t _x, uint16_t _y, uint16_t _w, uint16_t _h,TS_Point point) {
	if ((point.x >= _x) && (point.x <= _x+_w) && (point.y >= _y) && (point.y <= _y+_h)) {
      return true;
    } else {
      return false;
    }  
  }
  
  bool HMControl::buttonContainsPoint(HMInterfaceButton _ifb , TS_Point point) {
	if ((point.x >= _ifb.x) && (point.x <= _ifb.x+_ifb.w) && (point.y >= _ifb.y) && (point.y <= _ifb.y+_ifb.h)) {
      return true;
    } else {
      return false;
    }  
  }

  
  void HMControl::drawButton(HMInterfaceButton _ifb, bool pressed) {
	if (_ifb.img_name != "") {
    ui->drawBmp(_ifb.img_name,_ifb.x+_ifb.img_x,_ifb.y+_ifb.img_y);
	} else {
	  tft->drawRoundRect(_ifb.x,_ifb.y,_ifb.w,_ifb.h,4,_ifb.foreColor);

	  if (_ifb.large) {
	  	tft->setFont(&ArialRoundedMTBold_36);
	  } else {
	  	tft->setFont(&ArialRoundedMTBold_14);
	  }
	
	  ui->setTextAlignment(CENTER);
	
	  if (pressed) {
	  	tft->fillRoundRect(_ifb.x,_ifb.y,_ifb.w,_ifb.h,4,_ifb.invBackColor);
	  	tft->setTextColor(_ifb.invForeColor);
	  	ui->setTextColor(_ifb.invForeColor,_ifb.invBackColor);
	  } else {
	  	tft->fillRoundRect((_ifb.x+1),(_ifb.y+1),(_ifb.w-2),(_ifb.h-2),4,_ifb.backColor);
	  	tft->setTextColor(_ifb.foreColor);
	  	ui->setTextColor(_ifb.foreColor,_ifb.backColor);
	  }
      ui->drawString( (_ifb.w /2 )+ _ifb.x , _ifb.y + (_ifb.h/2) + 10, _ifb.caption);
    }
  }


