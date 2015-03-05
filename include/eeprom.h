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

typedef enum errorCode {NONE,SPACEOVERRIDE,BATTERYLOW};

// functions
unsigned char mem_check(void); // Check the memory. Certain things are checksumed.
void mem_push(void); // Save special bytes that are in ram to eeprom.
unsigned char mem_append_log(char); // saves an event to the log space.
static unsigned mem_write(unsigned short, unsigned char[], unsigned char); // Write byte(s) to address
static unsigned char* mem_read(unsigned short, unsigned char); // read bytes at address


#endif