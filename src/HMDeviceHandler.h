#include <Arduino.h>

class HMDeviceHandler {

  public:
    String ccuIP;
    //Thermostat
    double getCurrentTemperature(String adress);
    double getTargetTemperature(String adress);
	void setTargetTemperature(String adress,double target);
	bool getBoost(String adress);
	void setMode(String adress,int mode);
	int getMode(String adress);
	
    // Switch
    bool isOn(String adress);
    void setOn(String adress,bool _on);
};


