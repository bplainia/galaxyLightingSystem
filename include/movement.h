#ifndef MOVEMENT_H
#define MOVEMENT_H

// Headers
#include <shared.h>
#include <xc.h>
#include <communication.h>
#include <eeprom.h>
#include <hid.h>
#include <powerman.h>
#include <rtc.h>
#include <sensors.h>
#include <solarLight.h>

// Varibles
// Limits will show the analog values on the pots where the limits are
unsigned int leftlimit = 0;
unsigned int rightlimit = 0;
unsigned int uplimit = 0;
unsigned int downlimit = 0;

// Defines

#define PINIO_MOVE_LEFTRIGHT_1 TRISBbits.TRISB1
#define PINIO_MOVE_LEFTRIGHT_2 TRISBbits.TRISB2
#define PINIO_MOVE_LEFTRIGHT_3 TRISBbits.TRISB3
#define PINIO_MOVE_LEFTRIGHT_4 TRISBbits.TRISB4
#define PINIO_MOVE_UPDOWN_1 TRISDbits.TRISD4
#define PINIO_MOVE_UPDOWN_2 TRISDbits.TRISD5
#define PINIO_MOVE_UPDOWN_3 TRISDbits.TRISD6
#define PINIO_MOVE_UPDOWN_4 TRISDbits.TRISD7

/// varibles for debug testing, will have bits toggled depending on function entered
unsigned char move = 0;

// Functions

void movement_setup(void);
void limit_test(void);
void daytime_move(void);
void dusk_moveback(void);
void season_adjust(void);
void maintenance_move(unsigned char);

#endif