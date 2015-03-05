#ifndef SOLARLIGHT_H
#define SOLARLIGHT_H

// Include all the headers
#include <shared.h>
#include <communication.h>
#include <eeprom.h>
#include <hid.h>
#include <movement.h>
#include <powerman.h>
#include <rtc.h>
#include <sensors.h>

// Functions
// void main() is in the associated C file
void setup(void); // setup things with out any interrupts
void loop(void);  // do this stuff forever. Interrupts enabled.
void interrupt low_priority isr_low(void);
void interrupt high_priority isr_high(void);

#endif