#ifndef EEPROM_H
#define EEPROM_H

// CONSTANTS
#define EEPROMADDR 0xA0

// Enumerations
typedef enum powerStatus {FIRSTRUN, POR, VBATT, RUNNING};

// functions
void mem_start(void);
powerStatus mem_check(void);


#endif