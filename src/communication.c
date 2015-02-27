//TODO: Finish skeleton For communications
//TODO[Review]: Communications (BEN)
// Author: Benjamin Plain
// Communications chip library
#include <shared.h>
#include <communication.h>

// Variables for this library (PRIVATE)
unsigned char rxBuff[RXBUFFSIZE];
unsigned char txBuff[TXBUFFSIZE];
unsigned char rxPtrOut, txPtrOut, rxPtrIn, txPtrIn;

void comm_start()
{
    //TODO: Communications start function
    // This function initialized the pins and sets up UART1 for 9-bit RS-485

    // Setup PPS-Lite
    RPOR22_23bits.RPO22R = 0x1;  // TX pin output
    RPINR0_1             = 0x55; // TX\RX pin inputs
    RPOR22_23bits.RPO23R = 0x1;  // RX pin output
    TXPIN   = 0;    // Transmit (recieve is already digital input)
    RXTXPIN = 0;    // Direction Pin
    RXTXDIR = 0;    // Set to Recieve Mode

    txPtrIn = rxPtrIn = txPtrOut = rxPtrOut = 0; // reset pointers
    comm_overflow = 0; // Has not happened
    comm_flush();
}

void comm_flush()
{
    unsigned char i;
    for(i=0;i<TXBUFFSIZE;i++) txBuff[i] = 0; // Clear the Buffer
    for(i=0;i<RXBUFFSIZE;i++) rxBuff[i] = 0;
}

unsigned comm_tx(unsigned char data)
{
    // Detect that there is something in the buffer and no overflow
    if((txPtrIn + 1) != txPtrOut)
    {
        txBuff[txPtrIn++] = data;
        if(txPtrIn >= TXBUFFSIZE) txPtrIn = 0;
        return true;
    }
    else
    {
        COMSTAT.TXOVER = 1; // Set the tx overflow flag. I can't take any more
        return false;
    }
}

unsigned comm_go()
{
    /// char comm_go(void): activate transmission onto universe
    if(COMSTAT.TOKEN) // Make sure we have the token
    {
        if(txPtrOut != txPtrIn)
        {
            TX1IE = 1;                               // enable interrupt
            TXREG1 = txBuff[txPtrOut++];             // begin transmission
            if(txPtrOut >= TXBUFFSIZE) txPtrOut = 0; // reset pointer if done
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
    return true;
}

unsigned char comm_rx()
{
    unsigned char myReturn;
    myReturn = rxBuff[rxPtrOut++];
    if(rxPtrOut >= RXBUFFSIZE) rxPtrOut = 0;
    // TODO: Add buffer overflow detection
}
unsigned short comm_rx_word()
{
    // TODO: What happens if ther is no data there?
    return (comm_rx() << 8) + comm_rx();
}