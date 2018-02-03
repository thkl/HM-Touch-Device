#include "HMDeviceHandler.h"
#include "Rega.h"

  double HMDeviceHandler::getCurrentTemperature(String adress) {
    Rega rega;
    String result = rega.runScript(ccuIP,"WriteLine(dom.GetObject('BidCos-RF."+adress+":2.ACTUAL_TEMPERATURE').Value());");
    return result.toFloat();
  }

  double HMDeviceHandler::getTargetTemperature(String adress) {
    Rega rega;
    String result = rega.runScript(ccuIP,"WriteLine(dom.GetObject('BidCos-RF."+adress+":2.SET_TEMPERATURE').Value());");
    return result.toFloat();
  }

  void HMDeviceHandler::setTargetTemperature(String adress,double target) {
    Rega rega;
    rega.runScript(ccuIP,"WriteLine(dom.GetObject('BidCos-RF."+adress+":2.SET_TEMPERATURE').State(" + target + "));");
  }

  bool HMDeviceHandler::getBoost(String adress) {
    int mode = getMode(adress);
    return (mode == 3);
  }

  int HMDeviceHandler::getMode(String adress) {
    Rega rega;
    String result = rega.runScript(ccuIP,"WriteLine(dom.GetObject('BidCos-RF."+adress+":2.CONTROL_MODE').Value());");
    // 0 = Auto Mode
    // 1 = Manu Mode
    // 3 = Boost Mode
    return result.toInt();
  }

  void HMDeviceHandler::setMode(String adress,int mode) {
	Rega rega;
    if (mode == 0) {
		rega.runScript(ccuIP,"WriteLine(dom.GetObject('BidCos-RF."+adress+":2.AUTO_MODE').State(1));");
	  }
    if (mode == 1) {
		rega.runScript(ccuIP,"WriteLine(dom.GetObject('BidCos-RF."+adress+":2.MANU_MODE').State(1));");
	  }
    if (mode == 3) {
		rega.runScript(ccuIP,"WriteLine(dom.GetObject('BidCos-RF."+adress+":2.BOOST_MODE').State(1));");
	  }
  }


  // Switch
  void HMDeviceHandler::setOn(String adress,bool _on) {
    Rega rega;
    String s_on = _on ? "true" : "false";
    rega.runScript(ccuIP,"WriteLine(dom.GetObject('BidCos-RF."+adress+".STATE').State(" + s_on + "));");
  }

  bool HMDeviceHandler::isOn(String adress) {
    Rega rega;
    String result = rega.runScript(ccuIP,"WriteLine(dom.GetObject('BidCos-RF."+adress+".STATE').Value());");
    if (result.indexOf("true")>-1) {
      return true;
    }
    return false;
  }

  // Dimmer
  void HMDeviceHandler::setLevel(String adress,double _level) {
    Rega rega;
    rega.runScript(ccuIP,"WriteLine(dom.GetObject('BidCos-RF."+adress+".LEVEL').State(" + _level + "));");
  }

  double HMDeviceHandler::getLevel(String adress) {
    Rega rega;
    String result = rega.runScript(ccuIP,"WriteLine(dom.GetObject('BidCos-RF."+adress+".LEVEL').Value());");
    return result.toFloat();
  }
