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

unsigned adc_readRaw(unsigned char chA) // Read specific pin - raw
{
    if(chA > 0b11111) return false;
    ADCHS0Lbits.CH0SA = chA;
    SAMP = 1; // Initiate sample
}

unsigned adc_update2(unsigned char chA, unsigned char chB)
{
    if((chA > 0b11111) || (chB > 0b11111)) return false;
    ADCHS0Lbits.CH0SA = chA;
    ADCHS0Hbits.CH0SB = chB;
    ADCON2Lbits.ALTS = 1;
    SAMP = 1; // Initiate sample(s)

    // Done.
    ADCON2Lbits.ALTS = 0;
    return true;
}

void adc_updateAll(unsigned enable) // Update the ADC Buffers
{
    ADCON2Hbits.CSCNA = 1; // Enable Channel Scan
    // TODO: adcUpdateAll() needs finisheds
    ADCON2Hbits.CSCNA = 0; // Disable Channel Scan

}

void pwmSetup() // Setup the timer for PWM
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
unsigned pwm_set(unsigned char, unsigned char) // Set pin to duty cycle
{
    
}

void i2c_setup() // Initialize the I2C pins
{
    SSP1CON1 = 0b00101000; // Enable MSSP in master i2c mode. Auto-configs the pins.
    SSP1ADD  = 0x4B; // for 400kHz Clock at 48MHz primary clock
    // TODO[review]: Check the baud rate generator or test it.
}
unsigned i2c_tx(i2cPacket packet) // send data to address
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
unsigned char[] i2c_rx(unsigned char addr, unsigned char qty) // recieve data from address
{
    /// i2cRx: recieves <qty> bytes from the slave at <addr>. Returns an array <qty> long.
    unsigned char data[qty];
    
    addr = addr & 0b01111111; // make sure that the MSB is cleared.

    // write address
    // enable recieve
    // read number of chars
    return true;
}