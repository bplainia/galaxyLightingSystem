/*! \file
 * This file containes the functions that are used in more than one file.
 * This includes the ADC, I2C, and PWM.
 * Author: Benjamin Plain
 */

#include <shared.h>

/// Setup the ADC module
void adc_setup()
{
    // AN4 = Battery, AN2,3 = Pots, 8,19,18,17 = Photocells, AN0 = Temp
    ADCSS0L = 0b00011101; //temp, pots, batt
    ADCSS0H = 0b00000001; // photo pin 8
    ADCSS1L = 0b00001110; // rest of photos
    ADCSS0H = 0; // no adc's on this one
    SAMC1   = 10;// sample time
    ADRC    = 1;// auto conversion trigger
    // enable ADC
    //! \todo TODO: Finish setting up ADC (clock, other)
}

/*! \brief Update a single ADC buffer
 *
 * Input: A single number specifying which channel to update from 0 to 15.
 * Outputs: A single bit specifying success. (1=success)
 */
void adc_update(unsigned char chA) // Read specific pin - raw
{
    if(chA > 0b11111) return;
    ADCHS0Lbits.CH0SA = chA;
    SAMP = 1; // Initiate sample
}

/*! \brief Updates two ADC channels to the buffers.
 *
 *  Inputs: two unsigned chars, `chA` and `chB`, specify which ADC channel to update.
 *
 *  Outputs: A single bit specifying success. (1=success)
 */
void adc_update2(unsigned char chA, unsigned char chB)
{
    if((chA > 0b11111) || (chB > 0b11111)) return;
    ADCHS0Lbits.CH0SA = chA;
    ADCHS0Hbits.CH0SB = chB;
    ADCON2Lbits.ALTS = 1;
    SAMP = 1; // Initiate sample(s)
    while(!DONE) continue;
    DONE = 0;
    while(!DONE) continue;
    DONE = 0;

    // Done.
    ADCON2Lbits.ALTS = 0;
}

/// Update all the ADC buffers that are activated in `adc_setup()`.
void adc_updateAll() 
{
    ADCON2Hbits.CSCNA = 1; // Enable Channel Scan
    //! \todo  TODO: adcUpdateAll() needs finisheds
    ADCON2Hbits.CSCNA = 0; // Disable Channel Scan

}

/// Setup TMR2 for use by all the CCP modules
void pwm_setup() 
{
    // Initialize TMR2
    //! \todo  TODO: Setup the timer for PWM
    PR2   = 0XFF;       //
    T2CON = 0b00000111; // 1:16 prescaler from instruction = 750kHz

    // Some reminders:
    // PWM Period = (PR2 + 1) * 4 * (1/<Primary Oscilator>) * <TMR2 Prescale>
    // Max Resolution = log_2 (Fosc/Fpwm)
    // Duty Cycle = (CCPR4L:CCP4CON<5:4>)*Tosc*Prescale
}

/*! \brief Sets a specific pwm channel to a certain duty cycle in %
 *
 *  Inputs: Channel - which PWM channel to use, duty - the duty cycle in percent.
 *
 *  Outputs: A single bit specifying whether or not the command succeded (1 = success)
 */
unsigned pwm_set(unsigned char channel, unsigned char duty) // Set pin to duty cycle
{

    return true;
}

/// Set up the i2c1 module
void i2c_setup() // Initialize the I2C pins
{
    SSP1CON1 = 0b00101000; // Enable MSSP in master i2c mode. Auto-configs the pins.
    SSP1ADD  = 0x4B; // for 400kHz Clock at 48MHz primary clock.
    //! \todo  FIXME: Check the baud rate generator or test it. Needs to be 100kHz.
}

/*! \brief Takes a packet and transmits it according to its contents. Returns true if successful.
 *
 *  Inputs: a i2c transmission packet (consists of address and array of data)
 *
 *  Outputs: a single bit specifying failure or success
 *
 *  Description: This function takes the packet and transmits the `packet.addr` byte first.
 *  It then transmits `packet.dataLength` bytes from the `packet.data` array.
 *  If at any time a byte is not acknowleged, the function will return 0 (false). 
 */
unsigned i2c_tx(i2cPacket packet)
{

    unsigned char i;
    i2c_start(0);
    i2c_send(packet.addr & 0b11111110); // write mode; address
    for(i = 0; i < packet.dataLength; ++i)
    {
        SSP1BUF = packet.data[i];
        while(!SSP1IF) continue;
        if(SSP1CON2bits.ACKDT)
        {
            return false; // NO-ACK = slave did not get data
        }
        // check ack bit and flag again
    }
    i2c_stop(); // generate stop bit
    return false;
}

/*! \brief Gets Data from a slave at a specific address. Returns a char array pointer.
 *
 * Inputs: `packet`: A packet consisting of an address, a number for quantity, and an array to fill with the quantity.
 * Outputs: None formally. Modifies the `data` portion of the packet pointer that was given.
 */
unsigned i2c_rx(i2cPacket *packet) // recieve data from address
{
    #define pack (*packet) // macro to make coding easer since we call it a lot
    unsigned char i;
    if(pack.dataLength == 0) return true; // Do nothing if there is a zero data length.
    
    // write address
    i2c_start(0);
    if(i2c_send(pack.addr & 0b11111110)) return true; // write mode at address
    if(pack.reg16) i2c_send(pack.regh); // Address High Byte
    if(i2c_send(pack.regl)) return true; // Address Low Byte
    i2c_start(1); // exit write mode
    if(i2c_send(pack.addr | 0b00000001)) return true; // read mode at address
    do
    {
        pack.data[i] = i2c_recv(0); // Ack
    }
    while (++i < pack.dataLength);
    pack.data[i] = i2c_recv(1); // NOAck
    i2c_stop();
    return false;
}

//! \brief Transmit a start bit on the i2c bus
static void i2c_start(unsigned repeat)
{
    SSP1IF = 0;
    if (repeat) SSP1CON2bits.RSEN = 1; // do a repeat start bit.
    else SSP1CON2bits.SEN = 1; // do a start bit.
    while(!SSP1IF) continue; // Wait until SSP1IF is set
    SSP1IF = 0;

    // check the acknowledge bit
    //if(SSP1CON2bits.ack bit 6);
    // check SSP1IF again to see if it is done
}

//! \brief Transmit a stop bit on the i2c bus
static void i2c_stop()
{
    SSP1IF = 0;
    SSP1CON2bits.PEN = 1;
    while(!SSP1IF) continue;
    SSP1IF = 0;
}

/*! \brief Transmit a specific byte onto I2C (Private function)
 * 
 * Inputs: a byte to send. 
 * Outputs: the ack bit or lack therof.
 */
static unsigned i2c_send(unsigned char byte)
{
    SSP1IF = 0; // make sure the flag is clear
    SSP1BUF = byte; // begin transmitting the byte
    while(!SSP1IF) continue; // wait for transmission to finish
    return (SSP1CON2bits.ACKDT || SSP1CON2bits.ACKSTAT); // if not acknoleged or got NO-ACK
}

/*! \brief Recieve a byte from the I2C device
 */
static unsigned char i2c_recv(unsigned ack)
{
    SSP1IF = 0;
    SSP1CON2bits.RCEN = 1; // Enable recieve of bits.
    while(!SSP1IF) continue; // wait for recieving to finish
    SSP1CON2bits.ACKEN = ack; // ACK
    SSP1CON2bits.ACKEN = 1; // Send the ACK bit
}

void i2c_lcdInit()
{
    // this is from the LCD's datasheet.
    /// \todo TODO: What should I be doing with the R/W and RS bits?
    i2c_start(); // send start bit
    i2c_send(0x78);
    __delay_ms(10);
    i2c_send(0x38);
    __delay_ms(10);
    i2c_send(0x39);
    i2c_send(0x14);
    i2c_send(0x78);
    i2c_send(0x5E);
    i2c_send(0x6D);
    i2c_send(0x0C);
    i2c_send(0x01);
    i2c_send(0x06);
    __delay_ms(10);
    i2c_stop();
}