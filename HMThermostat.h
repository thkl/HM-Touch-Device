#include <Arduino.h>
#include "HMControl.h"


class HMThermostat: public HMControl 
{
   public:
    HMThermostat();
    void updateState();
    void drawDetail();
    void willDissapear();
    byte handleTouch(TS_Point point);
    

   private:
    double c_temp;
    double c_settemp;
    bool c_isBoost;
    bool c_manuMode;
    double n_settemp;
    void showTemperature();
    
    HMInterfaceButton plusButton;
    HMInterfaceButton minusButton;
    HMInterfaceButton boostButton;
    HMInterfaceButton manuButton;
};


