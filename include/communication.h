#ifndef COMM_H
#define COMM_H

// Definitions
// Buffer sizes. Maximum of 256 (or need to change pointer size)
#define RXBUFFSIZE 128
#define TXBUFFSIZE 32

// Pins
#define TXPIN   TRISDbits.TRISD2
#define RXTXPIN TRISDbits.TRISD1
#define RXPIN   TRISDbits.TRISD3
#define RXTXDIR LATDbits.LATD1


// Functions for Communication

void comm_start(void); // Initialize Communication
void comm_flush(void); // Flush the buffers
unsigned comm_go(void); // Begin transmission of the buffer
unsigned comm_tx(unsigned char,unsigned); // Transmit a byte (add to TX FIFO) returns boolean
unsigned char comm_rx(void); // Get a byte from the RX FIFO
unsigned short comm_rx_word(void); // get a word from the RX FIFO

// Variables
char comm_overflow;
struct comstat_t
{
    unsigned  RXOVER   : 1; // rx buffer overflow error
    unsigned  TERROR   : 1; // no its not a terror, its a token error
    unsigned  STATE    : 3; // State of machine (00 = waiting, 01 = master, 10 = slave listening, 11=slave talking.
    unsigned           : 3; // leftovers
}COMSTAT;

//struct {
//    unsigned char byte;
//    unsigned isAddress;
//}rxBuff[RXBUFFSIZE];
unsigned char rxBuff[RXBUFFSIZE];

unsigned char rxPtrOut, rxPtrIn, masterAddr;
#endif