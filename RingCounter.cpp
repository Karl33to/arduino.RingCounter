#include "Arduino.h"
#include "EEPROM.h"
#include "RingCounter.h"

// the memory location at which the ring counter starts [0-65535]
uint16_t _ringStart;

// the number of bytes to use for the counter [0-65535]
uint16_t _ringSize;

RingCounter::RingCounter(uint16_t ringStart, uint16_t ringSize)
{
	_ringStart = ringStart;
	_ringSize = ringSize;
}

// clear any previously saved values - makes debugging easier
void RingCounter::clear() {
	const uint16_t counterEnd = (_ringStart + _ringSize) - 1;
	for (uint16_t i = _ringStart; i <= counterEnd; i++) {
		EEPROM.write(i, 0);
	}
}

// prints the EEPROM memory to Serial
void RingCounter::debug() {
	Serial.print("RingCounter::debug	[");
	const uint16_t counterEnd = (_ringStart + _ringSize) - 1;
	for (uint16_t i = _ringStart; i <= counterEnd; i++) {
		Serial.print(EEPROM.read(i));
		Serial.print(",");
	}
	Serial.println("]");
}

// looks for a change in the counter values and returns the _offset of the last updated value
// 0 = first item
uint16_t RingCounter::current() {
	const uint16_t counterEnd = (_ringStart + _ringSize) - 1;
	const uint8_t initalVal = EEPROM.read(_ringStart);
	// setup a default incase none of the values are different, return the last one
	uint16_t current_offset = (_ringSize - 1);
	for (uint16_t i = _ringStart; i <= counterEnd; i++) {
		uint8_t thisVal = EEPROM.read(i);
		if (thisVal != initalVal) {
			current_offset = (i - _ringStart) - 1;
			break;
		}
	}
	return current_offset;
}

// finds the next available memory location _offset
uint16_t RingCounter::next() {
	uint16_t next_offset = current() + 1;
	// if the last write was at the end of the register, start again
	if (next_offset == _ringSize) {
		next_offset = 0;
	}
	return next_offset;
}

// write a new byte of data to the EEPROM
void RingCounter::increment() {
	const uint16_t next_offset = next();
	const uint16_t nextPos = _ringStart + next_offset;
	const uint8_t nextVal = EEPROM.read(nextPos);
	// write the new counter position
	if (nextVal == 1) {
		EEPROM.write(nextPos, 0);
	} else {
		EEPROM.write(nextPos, 1);
	}
}

bool RingCounter::isFirst(){
	uint16_t current_offset = current();
	if (current_offset == 0) {
		return true;
	} else {
		return false;
	}
}

bool RingCounter::isLast(){
	uint16_t current_offset = current();
	if (current_offset == (_ringSize -1)) {
		return true;
	} else {
		return false;
	}
}