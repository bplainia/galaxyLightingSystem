#ifndef EEPROM_H
#define EEPROM_H

#include <shared.h>
#include <rtc.h> // need rtc library to do log

// CONSTANTS
#define EEPROMADDR 0xA0

//added by DJ's program
#define OK 0
#define NOEEPROM 1
#define NOSPACE 2
#define ERR_BATTLOW 3
#define ACDCPOWER 4
#define BULBLIFE 5
#define DAYTIMETRUE 6
#define ERR_MOTOR 7
#define ERR_PHOTO1 8
#define ERR_PHOTO2 9
#define ERR_PHOTO3 10
#define ERR_PHOTO4 11
#define ERR_MOTION 12

// Enumerations
/* power-up status in order: 
 FIRSTRUN: first time initialization
 BATTFAIL: vdd and vbatt went down
 VDDFAIL : came back from vbatt
 MEMFAIL : eeprom is missing or is corrupted
 RUNNING : everything is ok now
 */

/// \todo TODO: Have default values setup on the first run.
//from menu [ben]: I need the following variables:
unsigned char setting_timeout;
unsigned char setting_bits1; // b5-4 = AC/Battery mode, b3-2 = light mode, b1 = hurricane, b0 = locked/auto.
unsigned char setting_lightDim = 251; // approx 7.4% duty cycle (takes 0.4A)
unsigned char setting_xaxis, setting_yaxis;


typedef enum errorCode {NONE=0,SPACEOVERRIDE=1,BATTERYLOW=2};

// functions
unsigned char mem_check(void); // Check the memory. Certain things are checksumed.
void mem_push(void); // Save special bytes that are in ram to eeprom.
unsigned char mem_append_log(unsigned char); // saves an event to the log space.
static unsigned mem_write(unsigned int, unsigned char[], unsigned char); // Write byte(s) to address
static unsigned mem_read(unsigned int, unsigned char*, unsigned char); // read bytes at address


#endif