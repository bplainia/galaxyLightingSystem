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

#include <xc.h>

// Global Variables (beware race conditions)
struct {
    unsigned    RTCINIT : 1;
    unsigned    MMODE   : 2;
    unsigned    STATE   : 3;
    unsigned            : 2;
}status;

// Standard Functions

float adcRead(unsigned char); // Read specific pin
unsigned short adcReadRaw(unsigned char); // Read specific pin - raw

void pwmSetup(); // Initialize TMR2 (you get to setup on your own)
unsigned pwmSet(unsigned char, unsigned char); // Set pin to duty cycle

#endif	/* SHARED_H */

