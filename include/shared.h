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
struct {
    unsigned    RTCINIT : 1;
    unsigned    MMODE   : 2; // 0=off, 1=main, 2=slave
    unsigned    STATE   : 3;
    unsigned            : 2;
}status;

typedef struct i2cPacket
{
    unsigned char addr;
    unsigned char data[];
    unsigned char dataLength;
};

// Standard Functions

float adcRead(unsigned char); // Read specific pin
unsigned short adcReadRaw(unsigned char); // Read specific pin - raw

void pwmSetup(); // Initialize TMR2 (you get to setup on your own)
unsigned pwmSet(unsigned char, unsigned char); // Set pin to duty cycle

void i2cSetup(); // Initialize the I2C pins
unsigned i2cTx(unsigned char, unsigned char); // send data to address
unsigned char i2cRx(unsigned char); // recieve data from address

#endif	/* SHARED_H */

