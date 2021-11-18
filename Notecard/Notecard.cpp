// Copyright 2018 Inca Roads LLC.  All rights reserved.
// Use of this source code is governed by licenses granted by the
// copyright holder including that found in the LICENSE file.

#include "Notecard.h"

// Forward references to C-callable functions defined below
extern "C" {
	bool noteSerialReset(void);
	void noteSerialTransmit(uint8_t *text, size_t len, bool flush);
	bool noteSerialAvailable(void);
	char noteSerialReceive(void);
	bool noteI2CReset(uint16_t DevAddress);
	const char *noteI2CTransmit(uint16_t DevAddress, uint8_t* pBuffer, uint16_t Size);
	const char *noteI2CReceive(uint16_t DevAddress, uint8_t* pBuffer, uint16_t Size, uint32_t *avail);
	size_t debugSerialOutput(const char *message);
    void delay(uint32_t ms);
    long unsigned int millis(void);
    void lockI2C(void);
    void unlockI2C(void);
    void lockNote(void);
    void unlockNote(void);
}

// For serial debug output
static bool debugSerialInitialized;
static Serial *debugSerial;

// Mbed ports
static Serial *hwSerial;
static I2C *hwI2C;

// Mutual exclusion
static Mutex mutexNote;
static Mutex mutexI2C;

// Initialize for serial I/O
void NoteInitSerial(Serial &selectedPort) {
	hwSerial = &selectedPort;
    NoteSetFnMutex(NULL, NULL, lockNote, unlockNote);
	NoteSetFnDefault(malloc, free, delay, millis);
	NoteSetFnSerial(noteSerialReset, noteSerialTransmit, noteSerialAvailable, noteSerialReceive);
}

// Initialize for I2C I/O
void NoteInitI2C(I2C &selectedPort) {
	hwI2C = &selectedPort;
    NoteSetFnMutex(lockI2C, unlockI2C, lockNote, unlockNote);
	NoteSetFnDefault(malloc, free, delay, millis);
	NoteSetFnI2C(NOTE_I2C_ADDR_DEFAULT, NOTE_I2C_MAX_DEFAULT, noteI2CReset, noteI2CTransmit, noteI2CReceive);
}

// Initialize for I2C I/O with extended details
void NoteInitI2CExt(I2C &selectedPort, uint32_t i2caddress, uint32_t i2cmax) {
	hwI2C = &selectedPort;
    NoteSetFnMutex(lockI2C, unlockI2C, lockNote, unlockNote);
	NoteSetFnDefault(malloc, free, delay, millis);
	NoteSetFnI2C(i2caddress, i2cmax, noteI2CReset, noteI2CTransmit, noteI2CReceive);
}

// Serial output method
size_t debugSerialOutput(const char *message) {
	if (!debugSerialInitialized)
		return 0;
    size_t i, len = strlen(message);
    for (i=0; i<len; i++)
        debugSerial->putc(message[i]);
    return len;
}

void NoteSetDebugOutput(Serial &dbgserial) {
	debugSerial = &dbgserial;
	debugSerialInitialized = true;
	NoteSetFnDebugOutput(debugSerialOutput);
}

// Serial port reset
bool noteSerialReset() {
    while (hwSerial->readable())
        hwSerial->getc();

    return true;
}

// Serial transmit function
void noteSerialTransmit(uint8_t *text, size_t len, bool flush) {
    size_t i;
    for (i=0; i<len; i++)
        hwSerial->putc(text[i]);
}

// Serial "is anything available" function
bool noteSerialAvailable() {
	return (hwSerial->readable());
}

// Serial read a byte function, guaranteed only ever to be called if there is data Available()
char noteSerialReceive() {
	return hwSerial->getc();
}

// I2C port reset
bool noteI2CReset(uint16_t DevAddress) {
    return true;
}

// Transmits in master mode an amount of data in blocking mode.	 The address
// is the actual address; the caller should have shifted it right so that the
// low bit is NOT the read/write bit.  If TimeoutMs == 0, the default timeout is used.
// An error message is returned, else NULL if success.
const char *noteI2CTransmit(uint16_t DevAddress, uint8_t* pBuffer, uint16_t Size) {
    const char *errstr = NULL;
    int writelen = sizeof(uint8_t) + Size;
    uint8_t *writebuf = (uint8_t *) malloc(writelen);
    if (writebuf == NULL) {
        errstr = "i2c: insufficient memory (write)";
    } else {
        writebuf[0] = Size;
        memcpy(&writebuf[1], pBuffer, Size);
        int err_code = hwI2C->write(DevAddress<<1, (const char *) writebuf, writelen);
        free(writebuf);
        if (err_code != 0)
            errstr = "i2c: write error";
    }
    return errstr;
}

// Receives in master mode an amount of data in blocking mode.
const char *noteI2CReceive(uint16_t DevAddress, uint8_t* pBuffer, uint16_t Size, uint32_t *available) {
    const char *errstr = NULL;
    int err_code;

    // Retry transmit errors several times, because it's harmless to do so
    for (int i=0; i<3; i++) {
        uint8_t hdr[2];
        hdr[0] = (uint8_t) 0;
        hdr[1] = (uint8_t) Size;
        err_code = hwI2C->write(DevAddress<<1, (const char *) hdr, sizeof(hdr));
        if (err_code == 0) {
            errstr = NULL;
            break;
        }
        errstr = "i2c: write error";
    }

    // Only receive if we successfully began transmission
    if (errstr == NULL) {

        int readlen = Size + (sizeof(uint8_t)*2);
        uint8_t *readbuf = (uint8_t *) malloc(readlen);
        if (readbuf == NULL) {
            errstr = "i2c: insufficient memory (read)";
        } else {
            err_code = hwI2C->read(DevAddress<<1, (char *) readbuf, readlen);
            if (err_code != 0) {
                errstr = "i2c: read error";
            } else {
                uint8_t availbyte = readbuf[0];
                uint8_t goodbyte = readbuf[1];
                if (goodbyte != Size) {
                    errstr = "i2c: incorrect amount of data";
                } else {
                    *available = availbyte;
                    memcpy(pBuffer, &readbuf[2], Size);
                }
            }
            free(readbuf);
        }
    }

    // Done
    return errstr;

}

// Computationally-delay the specified number of milliseconds
void delay(uint32_t ms) {
    ThisThread::sleep_for(ms);
}

// Get the number of app milliseconds since boot (this *will* wrap)
long unsigned int millis() {
    return (uint32_t) Kernel::get_ms_count();
}

// Lock mutex for synchronizing I/O for all tasks using I2C                                                             
void lockI2C() {
    mutexI2C.lock();
}

// Unlock mutex for synchronizing I/O for all tasks using I2C                                                             
void unlockI2C() {
    mutexI2C.unlock();
}

// Lock mutex for synchronizing I/O for all tasks using the Notecard package                                                             
void lockNote() {
    mutexNote.lock();
}

// Unlock mutex for synchronizing I/O for all tasks using the Notecard package
void unlockNote() {
    mutexNote.unlock();
}
