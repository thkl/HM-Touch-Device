#include <Arduino.h>
#include "HMControl.h"
#include "TimeClient.h"

class HMClock: public HMControl {

   public:
    HMClock();
    void updateState();
    void setTimeZone(int offset);
    void drawDetail();
    uint8_t handleTouch(TS_Point point);

   private:
    TimeClient timeClient = TimeClient(+1);
};
