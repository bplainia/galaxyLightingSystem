/*
 * Shared Functions
 * Author: Benjamin Plain
 */

#include <shared.h>

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
    // TODO: Finish setting up ADC (clock, other)
}

unsigned adc_update(unsigned char chA) // Read specific pin - raw
{
    if(chA > 0b11111) return false;
    ADCHS0Lbits.CH0SA = chA;
    SAMP = 1; // Initiate sample
}

/*! \brief Updates two ADC channels to the buffers.
 *
 *  Inputs: two unsigned chars, `chA` and `chB`, specify which ADC channel to update.
 *
 *  Outputs: A single bit specifying success. (1=success)
 */
unsigned adc_update2(unsigned char chA, unsigned char chB)
{
    if((chA > 0b11111) || (chB > 0b11111)) return false;
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
    return true; // TODO: adc updates: what will cause false to be returned?
}

/// Update all the ADC buffers that are activated in `adc_setup()`.
void adc_updateAll() 
{
    ADCON2Hbits.CSCNA = 1; // Enable Channel Scan
    // TODO: adcUpdateAll() needs finisheds
    ADCON2Hbits.CSCNA = 0; // Disable Channel Scan

}

/// Setup TMR2 for use by all the CCP modules
void pwm_setup() 
{
    // Initialize TMR2
    // TODO: Setup the timer for PWM
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

/// Sets up the i2c1 module
void i2c_setup() // Initialize the I2C pins
{
    SSP1CON1 = 0b00101000; // Enable MSSP in master i2c mode. Auto-configs the pins.
    SSP1ADD  = 0x4B; // for 400kHz Clock at 48MHz primary clock.
    // FIXME: Check the baud rate generator or test it. Needs to be 100kHz.
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
    SSP1CON2bits.SEN = 1;
    // Wait until SSP1IF is set
    SSP1BUF = packet.addr;
    while(!SSP1IF) continue; // wait for transmission to finish
    if(SSP1CON2bits.ACKDT || SSP1CON2bits.ACKSTAT) // if not acknoleged or got NO-ACK
    {
        return false; // failed to get object
    }
    // check the acknowledge bit
    //if(SSP1CON2bits.ack bit 6);
    // check SSP1IF again to see if it is done
    for(i = 0; i < packet.dataLength; i++)
    {
        SSP1BUF = packet.data[i];
        while(!SSP1IF) continue;
        if(SSP1CON2bits.ACKDT)
        {
            return false; // NO-ACK = slave did not get data
        }
        // check ack bit and flag again
    }
    // generate stop bit
    SSP1CON2bits.PEN = 1;
    // interrupt flag is set again
    return false;
}

/*! \brief Gets Data from a slave at a specific address. Returns a char array pointer.
 *
 *  Inputs: `device`: The 7-bit address of the device. `addr`: the 8-bit address in
 */
unsigned char* i2c_rx(unsigned char device, unsigned char addr, unsigned char qty) // recieve data from address
{
    /// i2cRx: recieves `qty` bytes from the slave at `addr`. Returns an array `qty` long.
    unsigned char data[qty];
    

    // write address
    // enable recieve
    // read number of chars
    return data;
}