//TODO: Finish skeleton For communications
//TODO[Review]: Communications (BEN)
// Author: Benjamin Plain
// Communications chip library
#include <shared.h>
#include <communication.h>

/// Setup the pins and modules for communication outside the pole.
///
/// See [baudrate](baudrate.html "Baud Rate Calculation") note.
void comm_start()
{
    //! \todo TODO: Communications start function
    // This function initialized the pins and sets up UART1 for 9-bit RS-485

    // Setup PPS-Lite
    RPOR22_23bits.RPO22R = 0x1;  // TX pin output
    RPINR0_1             = 0x55; // TX\RX pin inputs
    RPOR22_23bits.RPO23R = 0x1;  // RX pin output
    TXPIN   = 0;    // Transmit (recieve is already digital input)
    RXTXPIN = 0;    // Direction Pin
    RXTXDIR = 0;    // Set to Recieve Mode

    //! \todo TODO: Setup UART Module 1 Transmision
    TXSTA1bits.BRGH = 1; // High Baud Rate mode
    SPBRGH1 = 0; // Baud rate
    SPBRG1  = 63; // set for 250kbps
    TXSTA1bits.SYNC = 0; // Make sure the module is not in syncronous mode
    RCSTA1bits.SPEN = 1; // Enable serial port
    TXSTA1bits.TXEN = 1; // Enable transmitter
    TX1IF = 0;
    TX1IE = 0;
    TX1IP = 0; // Low Priority
    //enable tx1ie after clearing flag
    TXSTA1bits.TX9 = 1; // Enable 9-bit mode

    //! \todo TODO: Setup UART Module 1 Recieving
    RCSTA1 = 0b11111000; // Enable 9 bit reciever. Put into address mode first.
    /// Use the last 3 bits of RCSTA1 for errors and 9th bit.
    /// Address that is received will be in RCREG1

    txPtrIn = rxPtrIn = txPtrOut = rxPtrOut = 0; // reset pointers
    comm_overflow = 0; // Has not happened
    comm_flush();
}

void comm_flush()
{
    unsigned char i;
    for(i=0;i<TXBUFFSIZE;++i) txBuff[i] = 0; // Clear the Buffer
    for(i=0;i<RXBUFFSIZE;++i) rxBuff[i] = 0;
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
    /*! \brief Activate transmission onto universe
     */
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