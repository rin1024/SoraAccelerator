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

#define SA_DETECT_TYPE_MAGNITUDE           0
#define SA_DETECT_TYPE_ACCEL               1
#define SA_DETECT_TYPE_DIFF                2
#define SA_DETECT_TYPE_AVERAGE             3

#define SA_SENSOR_NOT_DETECTED            0
#define SA_SENSOR_DETECTED                1

#define SA_DEFAULT_NUM_READINGS          10
#define SA_DEFAULT_IGNORE_MILLIS       1000
#define SA_DEFAULT_THREASHOLD          20.0
#define SA_DEFAULT_NUM_SENSORS            3

#define SA_SEPARATOR                   "\t"

/**************************************************************************************
 * SoraAccelerator : 加速度センサ等
 **************************************************************************************/
class SoraAccelerator {
  public:
    SoraAccelerator();
    ~SoraAccelerator();
    
    void init(uint16_t _id, uint8_t _numSensors, uint8_t *_pins);
    void update();
    bool isDetected();

    void enableDebug(uint8_t _debugType);
    void disableDebug();

    void setDetectType(uint8_t _detectType);
    void setNumReadings(uint16_t _numReadings);
    void setSensors(uint8_t _numSensors, uint8_t *_pins);
    void setIgnoreMillis(uint32_t _ignoreMs);
    void setThreashold(double _threashold);

    uint16_t getSensorId();
    uint16_t getSensorStatus();
    uint16_t getLastSensorStatus();
    double getLastMag();

  private:
    // 基本設定
    uint16_t sensorId;           // センサのID
    uint8_t debugType; // デバッグの種類
    uint8_t detectType; // タッチの検出方法
    
    // センサの状態に関するパラメータ
    uint16_t sensorStatus;        // センサの状態
    uint16_t lastSensorStatus;
    bool changeStatusFlag;    // sensorStatusが変わった場合trueに
    
    // センサの設定に関するパラメータ
    uint8_t numSensors; // センサの総数
    uint8_t *sensorPins;     // センサが接続されているピン

    // 無効化する時間に関するパラメータ
    uint32_t ignoreMillis;        // 検知されてから無視するまでの時間
    uint32_t lastDetectedMillis;  // 最後に検知された時間

    // センサのスムージングに関するパラメータ
    uint16_t readIndex;          // 現在のセンシング回数
    double readTotal;           // センサの合計値
    double *readQueue; // センサの値一次保持変数
    uint16_t numReadings;          // センサのスムージングをする回数
    
    // タッチ判定に関するパラメータ
    double threashold;          // Magの閾値
    double lastMag;            // 最後のMag
    uint16_t *lastSensorValues;
};

#endif
