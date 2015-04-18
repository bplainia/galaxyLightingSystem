/* 
 * File:   shared.h
 * Author: 118506
 *
 * Created on February 5, 2015, 7:53 PM
 */

#ifndef SHARED_H
#define	SHARED_H

#define VERSION1 0
#define VERSION2 1
#define COMPANY "Patience, Inc."
#define FIRMWARE_VERSION "Firmware Ver:" #VERSION1 "." #VERSION2

/// Address for communicaiton between poles
#define ADDRESS 0x05

#define true 1
#define false 0

/// System frequency is 64MHz. USB is at 48MHz
#define _XTAL_FREQ 64000000 //

#include <xc.h>

// Global Variables (beware race conditions)

/// The first status byte - Includes the two state machine states, the rtc initialization state, and whether or not the loop will sleep at the end.
struct {
    unsigned    rtcInit   : 1;
    unsigned    alarmInit : 1;
    unsigned    mmode     : 2; // 0=off, 1=main, 2=slave
    unsigned    state     : 2;
    unsigned    sleepable : 1;
    unsigned              : 1;
}status;

unsigned char myAddr; // Address of the pole (Configurable)
volatile unsigned short time,delayTime; // Timeout varialbe

/// A packet that contains an address (8 or 10bit), a data array, and how long the array is.
typedef struct 
{
    unsigned char addr;
    unsigned char regl; // low byte of register
    unsigned char regh; // high byte of register
    unsigned reg16; // is a 16 bit register address
    unsigned char *data;  // The pointer to the data
    unsigned char dataLength; // the length of the data
}i2cPacket;

/// A structure with a byte and a pointer to the next pointer
/// This is for dynamic FIFO
struct fifo
{
    unsigned char byte;
    struct fifo* nextAddr;
};

// Standard Functions

void adc_setup(void); // setup the adc module
void adc_update(unsigned char); // Update a pin
//unsigned int adc_read(unsigned char); // Read the given adc value out of the adc buffer
void adc_update2(unsigned char, unsigned char); // Update 2 pins consecutively
void adc_updateAll(); // Update all the ADC buffers that were selected in setup

void pwm_setup(void); // Initialize TMR2 (you get to setup on your own)
unsigned pwm_set(unsigned char, unsigned char); // Set pin to duty cycle

void i2c_setup(void); // Initialize the I2C pins
unsigned i2c_tx(unsigned char, unsigned char, unsigned char, unsigned char, unsigned char*, unsigned short); // send data to address
unsigned i2c_rx(unsigned char, unsigned char, unsigned char, unsigned char, unsigned char*, unsigned short); // recieve data from address
unsigned i2c_check(unsigned char); // Check and address
void i2c_lcdInit(void); // setup LCD screen
static void i2c_start(void); // transmit a start bit
static void i2c_restart(void); // transmit a start bit again
static void i2c_stop(void); // transmit a stop bit
static void i2c_ack(void); // transmit ack bit
static void i2c_nack(void); // transmit nack bit
static void i2c_wait(void); // Check and wait to see if a i2c function is running
static unsigned i2c_send(unsigned char); // private function to send a byte
static unsigned char i2c_recv(unsigned); // private function to recieve a byte

void delay(unsigned char);

#endif	/* SHARED_H */

