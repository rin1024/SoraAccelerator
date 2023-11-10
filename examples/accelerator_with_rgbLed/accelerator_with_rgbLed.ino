#include <SimpleFade.h>
#include <SoraAccelerator.h>

#define __M_COMPILE_DATE__ __DATE__ " " __TIME__
#define __M_FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

/////////////// serial
#define BAUD_RATE 115200

/////////////// fade
SimpleFade simpleFade;

/////////////// RGB LED
#define R_PIN     3
#define G_PIN     5
#define B_PIN     6

/////////////// Sensor
#define SENSOR_ID 1
#define NUM_SENSORS 3

uint8_t SENSOR_PINS[NUM_SENSORS] = {A0, A1, A2};

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
  sensor.setIgnoreMillis(50);

  analogWrite(R_PIN, 0);
  analogWrite(G_PIN, 0);
  analogWrite(B_PIN, 0);
}

/**
 * 
 */
void loop() {
  boolean sensorDebugDump = false;
  sensor.update(sensorDebugDump);
  if (sensor.isDetected()) {
    double power = sensor.getLastMag();
    int currentBrightness = int(power * 5.0);
    if (currentBrightness > 255) {
      currentBrightness = 255;
    }
    int targetBrightness = 0;
    uint32_t durationMillis = 250;

    simpleFade.setup(targetBrightness, durationMillis);
    simpleFade.setCurrentValue(currentBrightness);
    simpleFade.enableDebug(DEBUG_TYPE_PRINT);
    simpleFade.start();

    Serial.print(currentBrightness);
    Serial.print("\t");
    Serial.print(power);
    Serial.println(power);
  }

  int currentBrightness = simpleFade.update();
  analogWrite(R_PIN, currentBrightness);
  analogWrite(G_PIN, 0);
  analogWrite(B_PIN, 0);
}
