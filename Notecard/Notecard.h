// Copyright 2019 Blues Inc.  All rights reserved.
// Use of this source code is governed by licenses granted by the
// copyright holder including that found in the LICENSE file.

#pragma once

#include "mbed.h"
#include "note-c/note.h"

void NoteInitI2C(I2C &i2c);
void NoteInitI2CExt(I2C &i2c, uint32_t i2cAddress, uint32_t i2cMax);
void NoteInitSerial(Serial &serial);
void NoteSetDebugOutput(Serial &dbgserial);
