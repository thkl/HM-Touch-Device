#include "PageManager.h"

	void PageManager::init(Adafruit_ILI9341 * _tft, GfxUi * _ui){
    	ui = _ui;
		tft = _tft;
	}

	bool PageManager::touched(TS_Point point) {
		
		for (int i=0; i<deviceCount; i++  ) {
	    	if (currentPage == 0) {
		    	Serial.println("Page Manager Main Page");
		    	if (devices[i]->touched(point)) {
			    	// Check if we had a detail Page and jump there
					if (devices[i]->haszDetail == true) {
						Serial.print(devices[i]->ctrl_name);
						Serial.print("haz details. jump to page ");
						Serial.println(devices[i]->onPage);
						
			    		currentPage = devices[i]->onPage;
			    		updatePage();
					} else {
						// else send touch to control
						Serial.print("Page Manager Single Touch Control - send touch to");
						Serial.println(devices[i]->ctrl_name);
			    		devices[i]->handleTouch(point); 
			    	}
			    }
			} else if (currentPage == devices[i]->onPage) {
				currentControl = i;
				byte th = devices[currentControl]->handleTouch(point);
				
				if (th == TOUCH_HANDLED) {
					// If the Devices has handled the touch reset the last Update
				 	lastDetailPageUpdate = millis();
				}; 
				
				if (th == TOUCH_HANDLED_BACKBUTTON) {
					devices[currentControl]->willDissapear();
					jumpBack();
				}
			}
		}
	}
    
    void PageManager::jumpBack(){
	    if (currentPage != 0) {
		    
		    if (currentControl != -1) {
			    // notify current control
			    devices[currentControl]->willDissapear();
			    currentControl = -1;
		    }
		    Serial.println("Jump Back to Page 0");
			tft->fillScreen(0x0000);
			currentPage = 0;
	    	// Reset Last Update to make sure of complete redraw
	    	for (int i=0; i<deviceCount; i++  ) {
		    	devices[i]->lastUpdate = 0;
		    }
			updatePage();
    	}
    }
    
    void PageManager::updatePage(){
	    Serial.println("Update Page called");
	    for (int i=0; i<deviceCount; i++  ) {
	    	if (currentPage == 0) {
		    	Serial.println("Main Page");
		    	devices[i]->updateState();
		    }
		    
		    if (currentPage == devices[i]->onPage) {
			    Serial.print("Detail Page ");
			    Serial.println(currentPage);
			    tft->fillScreen(0x0000);
				devices[i]->drawDetail();
			    lastDetailPageUpdate = millis();
		    }
		}
    }
    
    void PageManager::addControl(HMControl *control, int detailPageId) {
	    devices[deviceCount] = control;
	    control->onPage = detailPageId;
		deviceCount++;
	}
	
	void PageManager::updateVoltage(uint16_t _voltage) {
		if (voltage!=_voltage) {
		tft->setTextColor(ILI9341_CYAN);
		tft->setFont(&ArialRoundedMTBold_14);
		ui->setTextColor(ILI9341_CYAN, ILI9341_BLACK);
		ui->setTextAlignment(RIGHT);
		ui->drawString(tft->width(), tft->height()-5, String(voltage*2) + " V");
		voltage = _voltage;
		}
	}

