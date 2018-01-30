#include <Arduino.h>
#include "HMControl.h"


class HMDimmer: public HMControl {

   public:
    void updateState();
    void drawDetail();
    uint8_t handleTouch(TS_Point point);
};  


