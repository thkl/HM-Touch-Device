#include <Arduino.h>
#include "HMControl.h"


class HMSwitch: public HMControl {

   public:
    void updateState();
    void drawDetail();
    byte handleTouch(TS_Point point);
   
   private:
    boolean c_isOn;
     
};


