#ifndef SOLARLIGHT_H
#define SOLARLIGHT_H

// Headers
#include <shared.h>

// Defines

#define LED_PIN   TRISCbits.TRISC7
#define LED_OUT   PORTCbits.RC7

#define LED_CHAN 7
#define LED_DUTY_OFF 100
#define LED_DUTY_DIM 14
#define LED_DUTY_ON 0

#define AUTO 1
#define ON 2
#define DIM 3
#define OFF 4

/// varibles for debug testing, will have bits toggled depending on function entered

unsigned char light = 0;

// Functions

void led_setup(void);
void led(unsigned char);
void led_auto(void);
void led_maintenance(unsigned char);
void bulb_status(unsigned char);

#endif