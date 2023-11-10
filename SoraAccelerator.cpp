#include "SoraAccelerator.h"

/**
 * コンストラクタ
 */
SoraAccelerator::SoraAccelerator() {
  this->sensorStatus = SENSOR_NOT_DETECTED;
  this->lastSensorStatus = SENSOR_NOT_DETECTED;

  this->changeStatusFlag = false;

  this->ignoreMillis        = DEFAULT_IGNORE_MILLIS;
  this->lastDetectedMillis  = millis() + DEFAULT_IGNORE_MILLIS;

  this->setNumReadings(DEFAULT_NUM_READINGS);

  this->threashold = DEFAULT_THREASHOLD;
  this->numSensors = DEFAULT_NUM_SENSORS;
}

/**
 * 初期化
 */
void SoraAccelerator::init(uint16_t id, uint8_t *pins) {
  this->sensorId = id;
  this->numSensors = sizeof(pins);

  for (uint8_t i=0;i<this->numSensors;i++) {
    this->sensorPins[i] = pins[i];
  }
}

/**
 * アップデート関数 : ここでセンサの値を更新したり、検知したりする
 */
void SoraAccelerator::update(bool debugDump) {
  double sensorValues[this->numSensors] = {};
  double totalSensorValue = 0.0;
  for (uint8_t i=0;i<this->numSensors;i++) {
    sensorValues[i] = analogRead(this->sensorPins[i]);
    totalSensorValue += sensorValues[i] * sensorValues[i];
  }

  double mag= sqrt(totalSensorValue);

  // high pass filter
  this->readTotal -= readQueue[this->readIndex];
  readQueue[this->readIndex] = mag;
  this->readTotal += readQueue[this->readIndex];
  mag -= this->readTotal / (double)this->numReadings;
  this->readIndex++;

  // タッチを検出
  if (this->readIndex >= this->numReadings) {
    // デバッグ表示する場合
    if (debugDump == true) {
      Serial.print(this->sensorStatus == SENSOR_DETECTED ? F("*") : F(" "));
      Serial.print(SEPARATOR);
      for (uint8_t i=0;i<this->numSensors;i++) {
        Serial.print(sensorValues[i]);
        Serial.print(SEPARATOR);
      }
      Serial.print(this->readTotal);
      Serial.print(SEPARATOR);
      Serial.print(mag);
      if (mag > this->threashold) {
        Serial.print(SEPARATOR);
        Serial.print(F(" ... detected"));
      }
      Serial.println();
    }

    // しきい値を越えた場合
    if (mag > this->threashold) {
      this->sensorStatus = SENSOR_DETECTED;

      this->lastDetectedMillis = millis();
    }
    // センサが閾値以下で、さらに最後にdetectしてから暫くたっている場合
    else if (millis() - this->lastDetectedMillis > this->ignoreMillis) {
      this->sensorStatus = SENSOR_NOT_DETECTED;
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
  if (this->changeStatusFlag == true && this->sensorStatus == SENSOR_DETECTED) {
    this->changeStatusFlag = false;
    result = true;
  }
  return result;
}

/**
 *
 */
void SoraAccelerator::setNumReadings(uint16_t _numReadings) {
  this->numReadings = _numReadings;

  // readingsの数を減らした際の対応
  if (this->readIndex >= this->numReadings - 1) {
    this->readIndex = this->numReadings - 1;
  }

  this->readIndex = 0;
  this->readTotal = 0;
  this->readQueue = new double[this->numReadings];
  for (uint16_t i=0;i<this->numReadings;i++) {
    this->readQueue[i] = 0;
  }
}

/**
 *
 */
void SoraAccelerator::setIgnoreMillis(uint32_t _ignoreMillis) {
  this->ignoreMillis = _ignoreMillis;
}

/**
 *
 */
void SoraAccelerator::setThreashold(double _threashold) {
  this->threashold = _threashold;
}

/**
 *
 */
uint16_t SoraAccelerator::getSensorID() {
  return sensorId;

}

/**
 *
 */
uint16_t SoraAccelerator::getSensorStatus() {
  return sensorStatus;

}

/**
 *
 */
uint16_t SoraAccelerator::getLastSensorStatus() {
  return lastSensorStatus;

}

/**
 *
 */
double SoraAccelerator::getLastMag() {
  return lastMag;
}
