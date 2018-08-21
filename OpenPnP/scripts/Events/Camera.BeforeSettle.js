/**
 * Controls lighting for the two cameras using two named actuators. The lights
 * for the up camera and down camera are turned on and off based on which camera
 * needs to capture.
 */
 var upCamLights = machine.getActuatorByName("UpCamLights");
 var downCamLights = machine.getActuatorByName("DownCamLights");

if (camera.looking == Packages.org.openpnp.spi.Camera.Looking.Up) {
  /* we need looking up camera: turn on up lights and turn off down camera lights */
	if (upCamLights) {
	  upCamLights.actuate(true);
	}
	if (downCamLights) {
    downCamLights.actuate(false);
	}
} else if (camera.looking == Packages.org.openpnp.spi.Camera.Looking.Down) {
  /* we need the down looking camera: turn off up looking camera light */
	if (upCamLights) {
	  upCamLights.actuate(false);
	}
	if (downCamLights) {
	  downCamLights.actuate(true);
	}
}