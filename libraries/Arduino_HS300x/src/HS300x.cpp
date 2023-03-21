/*
  This file is part of the Arduino_HS300x library.
  Copyright (c) 2022 Arduino SA. All rights reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include <Wire.h>
#include "HS300x.h"

#define HS300X_ADR                  0x44
#define HS300X_TEMP_MULTY           0.010071415
#define HS300X_TEMP_MIN             40
#define HS300X_HUMD_MULTY           0.006163516
#define HS300X_MAX_ITERATION        100
#define HS300X_DELAY_MEASUREMENT    35          // typical on datasheet 16.90 ms, rounded up a little (35ms = 1 try)
#define HS300X_DELAY_ITERATION      1

#define HS300X_STALE_DATA           2
#define HS300X_OK                   1
#define HS300X_ERROR_SENSOR_BUSY    0
#define HS300X_ERROR_COLLISION_I2C  -1

int8_t HS300xClass::_measurementReq(){
  int8_t _status = 0;
  uint8_t _iteration = 0;
  _wire->beginTransmission(HS300X_ADR);
  _wire->write((uint8_t)0);
  if (_wire->endTransmission(true) != 0) {
    return HS300X_ERROR_COLLISION_I2C;
  }
  delay(HS300X_DELAY_MEASUREMENT);
  do {
    _status = _readSensor();
    _iteration ++;
    if (_iteration > HS300X_MAX_ITERATION) return HS300X_ERROR_SENSOR_BUSY;   
    delay(HS300X_DELAY_ITERATION);
  } while (!_status);
  return _status;
}

uint8_t HS300xClass::_readSensor(){
  uint16_t _rawTempMSB;
  uint16_t _rawTemp;
  uint16_t _rawHumMSB;
  uint16_t _rawHum;
  uint8_t  _rawStatus;

  _wire->requestFrom(HS300X_ADR, 4);        //true, stop message after transmission & releas the I2C bus
  if (_wire->available() != 4) {
    return 0;
  }
  _rawHumMSB = _wire->read() << 8;  // MSB
  _rawHum  = _wire->read();       // LSB
  _rawTempMSB = _wire->read() << 8;
  _rawTemp= _wire->read();

  _rawHum |= _rawHumMSB;
  _rawTemp |= _rawTempMSB;

  _rawStatus = _rawTemp >> 14;
  _rawHum = _rawHum & 0x3FFF; // mask 2 bit first
  _rawTemp = _rawTemp >>2;     // mask 2 bit last  
  if (_rawHum == 0x3FFF) return 0;
  if (_rawTemp == 0x3FFF) return 0;
  _temperature = (_rawTemp* HS300X_TEMP_MULTY) - HS300X_TEMP_MIN;
  _humidity = _rawHum * HS300X_HUMD_MULTY;
  return _rawStatus + 1;
}

HS300xClass::HS300xClass(TwoWire& wire) :
  _wire(&wire)
{
}

int HS300xClass::begin()
{
  _wire->begin();
  if (_measurementReq() <= 0) {
    return 0;
  }
  return 1;
}

void HS300xClass::end()
{
  // TODO: add disable HS300x

  _wire->end();
}

float HS300xClass::readTemperature(int units)
{
  if (_measurementReq() <= 0) {
    return NAN;
  }

  if (units == FAHRENHEIT) { // Fahrenheit = (Celsius * 9 / 5) + 32
    return (_temperature * 9.0 / 5.0) + 32.0;
  } else {
    return _temperature;
  }
}

float HS300xClass::readHumidity()
{
  if (_measurementReq() <= 0) {
    return NAN;
  }

  return _humidity;
}

#ifdef ARDUINO_ARDUINO_NANO33BLE
HS300xClass HS300x(Wire1);
#else
HS300xClass HS300x(Wire);
#endif
