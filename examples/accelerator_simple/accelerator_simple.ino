#include <SoraAccelerator.h>

#define __M_COMPILE_DATE__ __DATE__ " " __TIME__
#define __M_FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

/////////////// serial
#define BAUD_RATE 115200

/////////////// Sensor
#define SENSOR_ID 1
uint16_t SENSOR_PINS[3] = {A0, A1, A2};

SoraAccelerator sensor;

////////////////////////////////////////
/**
 * 
 */
void setup() {
  Serial.begin(BAUD_RATE);
  delay(1000);

  Serial.println();
  Serial.print("[");
  Serial.print(__M_COMPILE_DATE__);
  Serial.print("]");
  Serial.println(__M_FILENAME__);

  sensor.init(SENSOR_ID, SENSOR_PINS);
}

/**
 * 
 */
void loop() {
  sensor.update();

  if (sensor.isDetected()) {
    Serial.println(sensor.getLastMag());
  }
}
