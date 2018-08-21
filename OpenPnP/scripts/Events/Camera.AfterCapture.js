/**
 * Controls lighting for the two cameras using two named actuators.
 */
 var upCamLights = machine.getActuatorByName("UpCamLights");
 var downCamLights = machine.getActuatorByName("DownCamLights");
 
 /* turn off up looking camera lights, but always turn on down looking camera lights */
 if (upCamLights) {
	 upCamLights.actuate(false);
 }
 if (downCamLights) {
	 downCamLights.actuate(true); 
 }
