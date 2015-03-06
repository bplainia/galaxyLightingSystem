#ifndef SOLARLIGHT_H
#define SOLARLIGHT_H

// Headers
#include <shared.h>

// Defines

#define LED_IO TRISCbits.TRISC7
#define LED_OUT PORTCbits.RC7

#define AUTO 1
#define ON 2
#define DIM 3
#define OFF 4

/// varibles for debug testing, will have bits toggled depending on function entered

unsigned char light = 0;

// Functions

void led_setup(void);
void led_auto(void);
void led_maintenance(unsigned char);

#endif