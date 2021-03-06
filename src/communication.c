/*! \file
 *  \brief This page includes the communications functions to talk to other poles via RS/EIA-485.
 *
 * See [baudrate](baudrate.html "Baud Rate Calculation") note.
 * Author: Benjamin Plain
 */

//! \todo TODO: Finish skeleton For communications
//! \todo TODO[Review]: Communications (BEN)
// 
#include <shared.h>
#include <communication.h>
#include <eeprom.h>

//#include "usb/usb_hal_pic18.h"

/// Setup the pins and modules for communication outside the pole.
void comm_start()
{
    //! \todo TODO: Communications start function
    // This function initialized the pins and sets up UART1 for 9-bit RS-485

    // Setup PPS-Lite
    RPOR22_23bits.RPO22R = 0x1;  // TX pin output
    RPINR0_1             = 0x55; // TX\RX pin inputs
    // RPOR22_23bits.RPO23R = 0x1;  // RX pin output
    RPINR0_1bits.U1RXR   = 0X5;  // USART1 RX input set to RP23
    TXPIN   = 0;    // Transmit (recieve is already digital input)
    RXTXPIN = 0;    // Direction Pin
    RXTXDIR = 0;    // Set to Recieve Mode

    //Setup UART Module 1 Transmision
    TXSTA1bits.BRGH = 1; // High Baud Rate mode
    SPBRGH1 = 0; // Baud rate
    SPBRG1  = 31; // set for 250kbps (Maximum is 500kbps with the transciever)
    TXSTA1bits.SYNC = 0; // Make sure the module is not in syncronous mode
    RCSTA1bits.SPEN = 1; // Enable serial port
    TXSTA1bits.TXEN = 1; // Enable transmitter
    TX1IF = 0;
    TX1IE = 0; // no interrupt for transmission.
    TX1IP = 0; // Low Priority
    //enable tx1ie after clearing flag
    TXSTA1bits.TX9 = 1; // Enable 9-bit mode

    //Setup UART Module 1 Recieving
    RCSTA1 = 0b11011000; // Enable 9 bit reciever. Put into address mode first.
    /// Use the last 3 bits of RCSTA1 for errors and 9th bit.
    /// Address that is received will be in RCREG1

    comm_flush();
    comm_overflow = 0; // Has not happened
    COMSTAT.STATE = 0b000; // idle mode - wait for a keypress or a address to come in
    RCSTA1bits.ADEN = 1; // address filter
    RC1IE = 1; // interrupt is now enabled for recieving addresses
}

/// Flush both communications buffers
void comm_flush()
{
    unsigned char i;
    for(i=0;i<RXBUFFSIZE;++i) rxBuff[i] = 0;// Clear the Buffer to all zeros
    rxPtrIn = rxPtrOut = 0; // reset pointers
}

/* \brief Add a byte to the transmit stack
 *
 * Inputs:
 * Outputs: Outputs the success bit.
 * Description: Adds a single byte to the transmit stack. Make sure to call `comm_go()` so that the
 */
unsigned comm_tx(unsigned char data, unsigned isAddress)
{
    /*! # Addresses
     *  - 255 - Reserved since all high bits
     *  - 254 = Master is now active - Followed by the master's address
     *  - 253 = Master is inactive
     *  - 252 = Slave's turn to send
     *  - 251 = Slave's turn is over
     *  - 250 = GLOBAL ADDRESS
     *  - 0-249 = pole's addresses. Assigned in maintainence mode or via dip switches.
     */
    TXSTAbits.TX9D = isAddress;
    TXREG1 = data;
    while(!TXSTAbits.TRMT) continue;
    return 0;
}

/// Get a byte off the stack. Returns that byte.
unsigned char comm_rx()
{
    unsigned char myReturn;
    myReturn = rxBuff[rxPtrOut++];
    if(rxPtrOut >= RXBUFFSIZE) rxPtrOut = 0;
    //! \todo  TODO: Add buffer overflow detection (low priority for this system)
    return myReturn;

    // for better, dynamic FIFO, see p628 in the C book for how to use a queue
}

/// Get two bytes off the stack. Returns a short.
unsigned int comm_rx_word()
{
    //! \todo  TODO: What happens if ther is no data there?
    return (comm_rx() << 8) + comm_rx();
}

// TODO: comm_send_addr subject for removal upon approval:
//void comm_send_addr(unsigned char addr)
//{
//
//    TXSTA1bits.TX9D = 1; // Sending an address
//    comm_tx(addr, 0);
//    TXSTA1bits.TX9D = 0; // Done sending address.
//    if(addr == 254) // if it is a master active address, we need to send the address of the master too.
//    {
//        comm_tx(myAddr, 0);
//    }
//}

unsigned char comm_get_status(unsigned char addr)
{
    unsigned char byte;
    comm_tx(addr,1);        // send address of the specific pole
    comm_tx(0b00000101, 0);
    byte = comm_rx();
    return byte;
}

/// this is called by the main loop.exe
void comm_loop()
{
//    switch(COMSTAT.STATE)
//    {
//        case 0b011: // we need to talk to confirm things
//            /// \todo TODO: Take care of commands that we recieved
//            if
//    }
    unsigned char command,parity;

    if(COMSTAT.STATE == 0b011 && rxPtrOut != rxPtrIn) // slave response mode
    {
        command = rxBuff[rxPtrOut++];
        parity = command & 1;
        command = command & 0b01111110; // remove parity and reserved bit
        if(rxPtrOut >= RXBUFFSIZE) rxPtrOut = 0;
//        if(command % 2) // check for parity
//        {
//            comm_tx()
//        }

    }
    
    
    if(COMSTAT.TERROR)
    {
//        mem_log_error(ERR_TOKEN);
    }
}