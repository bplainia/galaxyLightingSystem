#ifndef COMM_H
#define COMM_H

// Definitions
// Buffer sizes. Maximum of 256 (or need to change pointer size)
#define RXBUFFSIZE 128
#define TXBUFFSIZE 32

// Pins
#define TXPIN   TRISDbits.TRISD0
#define RXTXPIN TRISDbits.TRISD1
#define RXTXDIR LATDbits.LATD1


// Functions for Communication

void comm_start(void); // Initialize Communication
void comm_flush(void); // Flush the buffers
unsigned char comm_tx(unsigned char); // Transmit a byte (add to TX FIFO) returns boolean
unsigned char comm_rx(void); // Get a byte from the RX FIFO
unsigned short comm_rx_word(void); // get a word from the RX FIFO

// Variables
char comm_overflow;
struct comstat_t
{
    unsigned  TOKEN    : 1; // the token (TOKEN BUS! :)
    unsigned  TXOVER   : 1; // tx buffer overflow error
    unsigned  RXOVER   : 1; // rx buffer overflow error
    unsigned  TERROR   : 1; // no its not a terror, its a token error
    unsigned           : 4; // leftovers
}COMSTAT;

unsigned char rxBuff[RXBUFFSIZE];
unsigned char txBuff[TXBUFFSIZE];
unsigned char rxPtrOut, txPtrOut, rxPtrIn, txPtrIn;
#endif