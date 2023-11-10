#ifndef SoraAccelerator_h
#define SoraAccelerator_h

/*******************************************
 * include
 *******************************************/
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

//////////////////////////////////////////////////////////////////////////////////////////
// 定数郡
#define SENSOR_NOT_DETECTED       0
#define SENSOR_DETECTED           1

#define SENSOR_READINGS          50
#define SENSOR_IGNORE_MS       1200
#define SENSOR_THREASHOLD      20.0

#define SEPARATOR "\t"
#define NUM_SENSORS 3

/**************************************************************************************
 * SoraAccelerator : 加速度センサ(X,Y,Z軸の値を直にもらうやつ)
 **************************************************************************************/
class SoraAccelerator {
  public:
    SoraAccelerator();
    void init(uint16_t id, uint16_t *pins);
    void update(boolean debugDump = false);
    boolean isDetected();

    void setIgnoreMillis(uint32_t _ignoreMs);
    void setThreashold(double _threashold);

    uint16_t getSensorID();
    uint16_t getSensorStatus();
    uint16_t getLastSensorStatus();
    double getLastMag();

  private:
    uint16_t sensorId;           // センサのID
    uint16_t sensorStatus;        // センサの状態
    uint16_t lastSensorStatus;
    double lastMag;            // 最後のMag

    boolean changeStatusFlag;    // sensorStatusが変わった場合trueに

    uint32_t ignoreMillis;        // 検知されてから無視するまでの時間
    uint32_t lastDetectedMillis;  // 最後に検知された時間

    int16_t readIndex;          // 現在のセンシング回数
    double readTotal;           // センサの合計値
    double *readQueue; // センサの値一次保持変数

    double threashold;          // Magの閾値

    uint16_t sensorPins[NUM_SENSORS];     // センサが接続されているピン
};

#endif
