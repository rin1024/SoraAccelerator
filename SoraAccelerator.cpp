#include "SoraAccelerator.h"

/**
 * コンストラクタ
 */
SoraAccelerator::SoraAccelerator() {
  this->sensorStatus = SENSOR_NOT_DETECTED;
  this->lastSensorStatus = SENSOR_NOT_DETECTED;

  this->changeStatusFlag = false;

  this->ignoreMillis        = SENSOR_IGNORE_MS;
  this->lastDetectedMillis  = millis() + SENSOR_IGNORE_MS;

  this->readIndex = 0;
  this->readTotal = 0;
  this->readQueue = new double[SENSOR_READINGS];//(double*)malloc(sizeof(double) * SENSOR_READINGS);
  for (uint16_t i=0;i<SENSOR_READINGS;i++) {
    this->readQueue[i] = 0;
  }

  this->threashold = SENSOR_THREASHOLD;
}

/**
 * 初期化
 */
void SoraAccelerator::init(uint16_t id, uint16_t *pins) {
  this->sensorId = id;

  for (uint16_t i=0;i<NUM_SENSORS;i++) {
    this->sensorPins[i] = pins[i];
  }
}

/**
 * アップデート関数 : ここでセンサの値を更新したり、検知したりする
 */
void SoraAccelerator::update(boolean debugDump) {
  double v[NUM_SENSORS];
  for (uint16_t i=0;i<NUM_SENSORS;i++) {
    v[i] = analogRead(this->sensorPins[i]);
  }

  double mag = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);

  // high pass filter
  this->readTotal -= readQueue[this->readIndex];
  readQueue[this->readIndex] = mag;
  this->readTotal += readQueue[this->readIndex];
  mag -= this->readTotal / (double)SENSOR_READINGS;
  this->readIndex++;

  // タッチを検出
  if (this->readIndex >= SENSOR_READINGS) {
    // デバッグ表示する場合
    if (debugDump == true) {
      Serial.print(this->sensorStatus == SENSOR_DETECTED ? "*" : " ");
      Serial.print(SEPARATOR);
      Serial.print(v[0]);
      Serial.print(SEPARATOR);
      Serial.print(v[1]);
      Serial.print(SEPARATOR);
      Serial.print(v[2]);
      Serial.print(SEPARATOR);
      Serial.print(this->readTotal);
      Serial.print(SEPARATOR);
      Serial.print(mag);
      if (mag > this->threashold) {
        Serial.print(SEPARATOR);
        Serial.print(" ... detected");
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
boolean SoraAccelerator::isDetected() {
  boolean result = false;
  if (this->changeStatusFlag == true && this->sensorStatus == SENSOR_DETECTED) {
    this->changeStatusFlag = false;
    result = true;
  }
  return result;
}

/**
 *
 */
void SoraAccelerator::setIgnoreMillis(uint32_t _ignoreMs) {
  this->ignoreMillis = _ignoreMs;
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
