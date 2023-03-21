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

#ifndef _HS330X_H_
#define _HS330X_H_

#include <Arduino.h>
#include <Wire.h>

enum {
  FAHRENHEIT,
  CELSIUS
};

class HS300xClass {
public:
  HS300xClass(TwoWire& wire);

  int begin();
  void end();

  float readTemperature(int units = CELSIUS);
  float readHumidity();

private:
  TwoWire* _wire;
  float _humidity;
  float _temperature;
  uint8_t _status;
  uint8_t _readSensor();
  int8_t _measurementReq();
};

extern HS300xClass HS300x;

#endif
