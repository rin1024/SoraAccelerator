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

#define DEFAULT_NUM_READINGS          50
#define DEFAULT_IGNORE_MILLIS       1200
#define DEFAULT_THREASHOLD      20.0

#define SEPARATOR "\t"
#define NUM_SENSORS 3

/**************************************************************************************
 * SoraAccelerator : 加速度センサ(X,Y,Z軸の値を直にもらうやつ)
 **************************************************************************************/
class SoraAccelerator {
  public:
    SoraAccelerator();
    void init(uint16_t id, uint16_t *pins);
    void update(bool debugDump = false);
    bool isDetected();

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

    bool changeStatusFlag;    // sensorStatusが変わった場合trueに

    uint32_t ignoreMillis;        // 検知されてから無視するまでの時間
    uint32_t lastDetectedMillis;  // 最後に検知された時間

    uint16_t readIndex;          // 現在のセンシング回数
    double readTotal;           // センサの合計値
    double *readQueue; // センサの値一次保持変数

    uint16_t numReadings;          // センサのスムージングをする回数
    double threashold;          // Magの閾値

    uint16_t sensorPins[NUM_SENSORS];     // センサが接続されているピン
};

#endif
