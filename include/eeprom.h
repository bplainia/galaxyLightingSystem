#ifndef EEPROM_H
#define EEPROM_H

#include <shared.h>
#include <rtc.h> // need rtc library to do log

// CONSTANTS
#define EEPROMADDR 0xA0

// Enumerations
/* power-up status in order: 
 FIRSTRUN: first time initialization
 BATTFAIL: vdd and vbatt went down
 VDDFAIL : came back from vbatt
 MEMFAIL : eeprom is missing or is corrupted
 RUNNING : everything is ok now
 */

// functions
void mem_start(void);
void mem_check(void);
unsigned char mem_append_log(unsigned char); // saves an event.
private:



#endif