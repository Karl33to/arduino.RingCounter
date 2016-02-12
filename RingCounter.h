/*
  RingCounter.h - Library for EEPROM wear levelling
  Created by Karl Payne, February 10, 21016.
  Released into the public domain.
*/
#ifndef RingCounter_h
#define RingCounter_h

#include "Arduino.h"

class RingCounter
{
	public:
		RingCounter(uint16_t ringStart, uint16_t ringSize);
		void clear();
		void debug();
		uint16_t current();
		uint16_t next();
		void increment();
		bool isFirst();
		bool isLast();
	private:
		uint16_t _offset;
		uint16_t _ringStart;
		uint16_t _ringSize;
};

#endif