#include <Arduino.h>
#include "HMControl.h"


class HMDimmer: public HMControl {

   public:
    HMDimmer();
    void updateState();
    void drawDetail();
    uint8_t handleTouch(TS_Point point);
  private:
    double _level;
    HMInterfaceButton plusButton;
    HMInterfaceButton minusButton;

    HMInterfaceButton ButtonOff;
    HMInterfaceButton Button50;
    HMInterfaceButton ButtonOn;
};
