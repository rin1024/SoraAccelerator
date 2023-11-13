#include "SoraAccelerator.h"

/**
 * コンストラクタ
 */
SoraAccelerator::SoraAccelerator() {
  this->sensorStatus = SA_SENSOR_NOT_DETECTED;
  this->lastSensorStatus = SA_SENSOR_NOT_DETECTED;
  this->changeStatusFlag = false;

  this->ignoreMillis        = SA_DEFAULT_IGNORE_MILLIS;
  this->lastDetectedMillis  = millis() + SA_DEFAULT_IGNORE_MILLIS;

  this->setNumReadings(SA_DEFAULT_NUM_READINGS);

  this->threashold = SA_DEFAULT_THREASHOLD;
  this->numSensors = SA_DEFAULT_NUM_SENSORS;
}

/**
 * デストラクタ
 */
SoraAccelerator::~SoraAccelerator() {
  delete[] readQueue;
  delete[] sensorPins;
}

/**
 * 初期化
 */
void SoraAccelerator::init(uint16_t id, uint8_t _numSensors, uint8_t *pins) {
  this->sensorId = id;
  this->numSensors = _numSensors;

  if (this->sensorPins != NULL) {
    delete[] this->sensorPins;
  }
  this->sensorPins = new uint16_t[this->numSensors];
  for (uint8_t i=0;i<this->numSensors;i++) {
    this->sensorPins[i] = pins[i];
  }
}

/**
 * アップデート関数 : ここでセンサの値を更新したり、検知したりする
 */
void SoraAccelerator::update() {
  double sensorValues[this->numSensors] = {};
  double totalSensorValue = 0.0;
  for (uint8_t i=0;i<this->numSensors;i++) {
    sensorValues[i] = analogRead(this->sensorPins[i]);
    totalSensorValue += sensorValues[i] * sensorValues[i];
  }

  double mag = sqrt(totalSensorValue);

  // high pass filter
  this->readTotal -= readQueue[this->readIndex];
  readQueue[this->readIndex] = mag;
  this->readTotal += readQueue[this->readIndex];
  mag -= this->readTotal / (double)this->numReadings;
  this->readIndex++;

  // タッチを検出
  if (this->readIndex >= this->numReadings) {
    if (debugType == SA_DEBUG_TYPE_PRINT) {
      Serial.print(this->sensorStatus == SA_SENSOR_DETECTED ? F("1024") : F("0"));
      Serial.print(SA_SEPARATOR);
      for (uint8_t i=0;i<this->numSensors;i++) {
        Serial.print(sensorValues[i]);
        Serial.print(SA_SEPARATOR);
      }
      Serial.print(this->readTotal);
      Serial.print(SA_SEPARATOR);
      Serial.print(mag);
      Serial.print(SA_SEPARATOR);
      if (mag > this->threashold) {
        Serial.print(F("1024"));
      }
      else {
        Serial.print(F("0"));
      }
      Serial.println();
    }

    // しきい値を越えた場合
    if (mag > this->threashold) {
      this->sensorStatus = SA_SENSOR_DETECTED;

      this->lastDetectedMillis = millis();
    }
    // センサが閾値以下で、さらに最後にdetectしてから暫くたっている場合
    else if (millis() - this->lastDetectedMillis > this->ignoreMillis) {
      this->sensorStatus = SA_SENSOR_NOT_DETECTED;
    }

    // 状態が変化した場合
    if (this->lastSensorStatus != this->sensorStatus) {
      this->changeStatusFlag = true;
    }

    // 一次データを掃除
    this->lastSensorStatus = this->sensorStatus;
    this->lastMag = mag;
    this->readIndex = 0;
  }
}

/**
 *
 */
bool SoraAccelerator::isDetected() {
  bool result = false;
  if (this->changeStatusFlag == true && this->sensorStatus == SA_SENSOR_DETECTED) {
    this->changeStatusFlag = false;
    result = true;
  }
  return result;
}

/**
   デバッグ表示を有効化
*/
void SoraAccelerator::enableDebug(uint8_t _debugType) {
  debugType = _debugType;
}

/**
   デバッグ表示を無効化
*/
void SoraAccelerator::disableDebug() {
  debugType = SA_DEBUG_TYPE_NONE;
}

/**
 * センサのスムージングする回数をセット、その関連変数の初期化も行う
 */
void SoraAccelerator::setNumReadings(uint16_t _numReadings) {
  this->numReadings = _numReadings;

  // readingsの数を減らした際の対応
  if (this->readIndex >= this->numReadings - 1) {
    this->readIndex = this->numReadings - 1;
  }

  this->readIndex = 0;
  this->readTotal = 0;
  
  if (readQueue != NULL) {
    delete[] readQueue;
  }
  this->readQueue = new double[this->numReadings];
  for (uint16_t i=0;i<this->numReadings;i++) {
    this->readQueue[i] = 0;
  }
}

/**
 * 一度detectしてから無視するミリ秒数をセット
 */
void SoraAccelerator::setIgnoreMillis(uint32_t _ignoreMillis) {
  this->ignoreMillis = _ignoreMillis;
}

/**
 * detect判定する閾値
 */
void SoraAccelerator::setThreashold(double _threashold) {
  this->threashold = _threashold;
}

/**
 * センサIDを取得
 */
uint16_t SoraAccelerator::getSensorId() {
  return sensorId;

}

/**
 * センサの状態を取得
 *
 * @return SA_SENSOR_DETECTED or SA_SENSOR_NOT_DETECTED
 */
uint16_t SoraAccelerator::getSensorStatus() {
  return sensorStatus;

}

/**
 * 前回のセンサの状態を取得
 */
uint16_t SoraAccelerator::getLastSensorStatus() {
  return lastSensorStatus;

}

/**
 * 直近のmagを取得
 */
double SoraAccelerator::getLastMag() {
  return lastMag;
}
