#ifndef SOLARLIGHT_H
#define SOLARLIGHT_H

// Headers
#include <shared.h>
#include <xc.h>
#include <communication.h>
#include <eeprom.h>
#include <hid.h>
#include <movement.h>
#include <powerman.h>
#include <rtc.h>
#include <sensors.h>

// Defines

#define LED_IO TRISCbits.TRISC7
#define LED_OUT PORTCbits.PORTC7

#define AUTO 1
#define ON 2
#define DIM 3
#define OFF 4

// test variables

unsigned char light = 0;

// Functions

void led_setup(void);
void led_light(unsigned char);
void led_maintenance(unsigned char);

#endif