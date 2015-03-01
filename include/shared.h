/* 
 * File:   shared.h
 * Author: 118506
 *
 * Created on February 5, 2015, 7:53 PM
 */

#ifndef SHARED_H
#define	SHARED_H

#define VERSION1 0
#define VERSION2 0
#define COMPANY "Patience, Inc."

#define true 1
#define false 0

#define _XTAL_FREQ 48000000 // our frequency is 48MHz

#include <xc.h>

// Global Variables (beware race conditions)

/// The first status byte - Includes the two state machine states, the rtc initialization state, and whether or not the loop will sleep at the end.
struct {
    unsigned    rtcInit   : 1;
    unsigned    mmode     : 2; // 0=off, 1=main, 2=slave
    unsigned    state     : 2;
    unsigned    sleepable : 1;
    unsigned              : 2;
}status; 

/// A packet that contains an address (8 or 10bit), a data array, and how long the array is.
typedef struct 
{
    unsigned short addr;
    unsigned      addr16; // Is the address 10 bits or not?
    unsigned char *data;  // The data
    unsigned char dataLength; // the length of the data
}i2cPacket;

// Standard Functions

void adc_setup(void); // setup the adc module
unsigned adc_update(unsigned char); // Update a pin
unsigned adc_update2(unsigned char, unsigned char); // Update 2 pins consecutively
unsigned adc_updateAll(); // Update all the ADC buffers that were selected in setup

void pwm_setup(void); // Initialize TMR2 (you get to setup on your own)
unsigned pwm_set(unsigned char, unsigned char); // Set pin to duty cycle

void i2c_setup(void); // Initialize the I2C pins
unsigned i2c_tx(i2cPacket); // send data to address
unsigned char* i2c_rx(unsigned char, unsigned char); // recieve data from address

#endif	/* SHARED_H */

