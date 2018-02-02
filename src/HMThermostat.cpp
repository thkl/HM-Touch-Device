#include "HMThermostat.h"
#include "Images.h"

 HMThermostat::HMThermostat() {
	 haszDetail = true;
	 plusButton = HMInterfaceButton(20,140,90,80,"+",true);
   plusButton.setImage("/gfx/btnplus.bmp",25,10);
   
	 minusButton = HMInterfaceButton(130,140,90,80,"-",true);
   minusButton.setImage("/gfx/btnminus.bmp",25,10);

	 boostButton = HMInterfaceButton(73,240,94,80,"");
   boostButton.setImage("/gfx/btnboost.bmp",0,0);
 }

 void HMThermostat::updateState() {

   double ctemp = deviceHandler->getCurrentTemperature(adress);
   double ttemp = deviceHandler->getTargetTemperature(adress);

   if ((lastUpdate == 0) || (c_temp != ctemp) || (c_settemp != ttemp)) {

     lastUpdate = millis();
     c_temp = ctemp;
     c_settemp = ttemp;
     n_settemp = ttemp;
     showTemperature();
   }
 }

 void HMThermostat::showTemperature() {
	 tft->fillRect(x,y,w,h,0x0000);
     tft->setTextColor(ILI9341_CYAN);
     tft->setFont(&ArialRoundedMTBold_36);
     ui->setTextColor(ILI9341_CYAN, ILI9341_BLACK);
     ui->setTextAlignment(RIGHT);
     ui->drawString(x+w, y+ (h/2), String(c_temp) + " C");

     uint16_t coltarget = 0xFFFF;
     if (c_settemp > c_temp) {
	     coltarget = 0xF800;
     }
     if (c_settemp < c_temp) {
	     coltarget = 0x001F;
     }

     tft->setTextColor(coltarget);
     tft->setFont(&ArialRoundedMTBold_14);
     ui->setTextColor(coltarget, ILI9341_BLACK);
     ui->setTextAlignment(RIGHT);
     ui->drawString(x+w, y+h-14, String(c_settemp) + " C");
     // draw image
     tft->drawBitmap(x,y+(h/2)-24,thermostat,48,48,0x07FF);

 }

 byte HMThermostat::handleTouch(TS_Point point) {

 	// Check +
 	if (buttonContainsPoint(plusButton,point)) {
	  drawButton(plusButton,true);
	  if (n_settemp <= 36) {
		  n_settemp = (int)n_settemp + 1;
		  drawDetail();
		  return TOUCH_HANDLED;
	  }
 	}

 	// Check -
 	if (buttonContainsPoint(minusButton,point)) {
	  drawButton(minusButton,true);
	  if (n_settemp >= 10) {
	  	n_settemp = (int)n_settemp - 1;
	  	drawDetail();
	  	return TOUCH_HANDLED;
	  } else {
      n_settemp = 4.5;
      drawDetail();
      return TOUCH_HANDLED;
	  }
 	}

 	// Check Boost
 	if (buttonContainsPoint(boostButton,point)) {
	  drawButton(boostButton,true);
	  bool boost = deviceHandler->getBoost(adress);
	  if (boost) {
	    // back to auto mode
		deviceHandler->setMode(adress,0);
	  } else {
		deviceHandler->setMode(adress,3);
	  }
	  drawDetail();
	  return TOUCH_HANDLED;
	}

	// Touch at textzone -> back
	if (rectContainsPoint(x,y,w,h,point)) {
		return TOUCH_HANDLED_BACKBUTTON;
	}

 	return TOUCH_NOT_HANDLED;
 }

 void HMThermostat::willDissapear() {
 	if (n_settemp != -1) {
    Serial.print("Thermostat::willDissapear send temp ");
    Serial.println(n_settemp);
 		deviceHandler->setTargetTemperature(adress,n_settemp);
 		n_settemp = -1;
 	}
 }

 void HMThermostat::drawDetail() {
	 // fill the complete display
    Serial.println("Thermostat::DrawDetail");
    tft->setTextColor(0xFFFF);
    tft->setFont(&ArialRoundedMTBold_14);
    ui->setTextColor(0xFFFF, ILI9341_BLACK);
    ui->setTextAlignment(LEFT);

    if (n_settemp == -1) {
	  	n_settemp = deviceHandler->getTargetTemperature(adress);
  	}

    uint16_t xpos = ((w/2)-75)+x;

    if (n_settemp == 4.5) {
      ui->drawBmp("/gfx/therm_off.bmp",xpos,y+10);
    } else {
      ui->drawBmp("/gfx/therm_" + String((int)n_settemp) + ".bmp",xpos,y+10);
    }

  	bool boost = deviceHandler->getBoost(adress);
	  if (boost) {
      boostButton.setImage("/gfx/btnboost_on.bmp",0,10);
	  } else {
      boostButton.setImage("/gfx/btnboost.bmp",0,10);
	  }

	// draw + / - Buttons
	drawButton(plusButton,false);
	drawButton(minusButton,false);

	drawButton(boostButton,false);
  ui->drawString(x, y+5,ctrl_name);
 }
