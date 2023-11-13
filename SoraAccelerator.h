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

/*******************************************
   const
 *******************************************/
#define SA_DEBUG_TYPE_NONE                0
#define SA_DEBUG_TYPE_PRINT               1

#define SA_SENSOR_NOT_DETECTED            0
#define SA_SENSOR_DETECTED                1

#define SA_DEFAULT_NUM_READINGS          50
#define SA_DEFAULT_IGNORE_MILLIS       1200
#define SA_DEFAULT_THREASHOLD          20.0
#define SA_DEFAULT_NUM_SENSORS            3

#define SA_SEPARATOR                   "\t"

/**************************************************************************************
 * SoraAccelerator : 加速度センサ(X,Y,Z軸の値を直にもらうやつ)
 **************************************************************************************/
class SoraAccelerator {
  public:
    SoraAccelerator();
    ~SoraAccelerator();
    
    void init(uint16_t id, uint8_t *pins);
    void update();
    bool isDetected();

    void enableDebug(uint8_t _debugType);
    void disableDebug();

    void setNumReadings(uint16_t _numReadings);
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
    uint8_t numSensors;

    double lastMag;            // 最後のMag

    bool changeStatusFlag;    // sensorStatusが変わった場合trueに

    uint32_t ignoreMillis;        // 検知されてから無視するまでの時間
    uint32_t lastDetectedMillis;  // 最後に検知された時間

    uint16_t readIndex;          // 現在のセンシング回数
    double readTotal;           // センサの合計値
    double *readQueue; // センサの値一次保持変数

    uint16_t numReadings;          // センサのスムージングをする回数
    double threashold;          // Magの閾値

    uint16_t *sensorPins;     // センサが接続されているピン

    uint8_t debugType;
};

#endif
