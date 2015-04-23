#ifndef LIGHT_H
#define LIGHT_H

// Headers
#include <shared.h>
#include <eeprom.h>

// Defines

#define LED_PIN   TRISAbits.TRISA1
#define LED_OUT   PORTAbits.RA1

#define LED_CHAN 3
#define LED_DUTY_OFF 256 // 100%
#define LED_DUTY_DIM 250  //
#define LED_DUTY_ON 0    // remember that these are inverted

#define AUTO 3
#define ON 2
#define DIM 1
#define OFF 0

/// varibles for debug testing, will have bits toggled depending on function entered

unsigned char light = 0;

// Functions

void led_setup(void);
void led(unsigned char);
void led_auto(void);
void led_maintenance(unsigned char);
void bulb_status(unsigned char);

#endif