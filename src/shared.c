/*
 * Shared Functions
 * Author: Benjamin Plain
 */

#include <shared.h>

void adcSetup()
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

unsigned adcReadRaw(unsigned char chA) // Read specific pin - raw
{
    if(chA > 0b11111) return false;
    ADCHS0Lbits.CH0SA = chA;
    SAMP = 1; // Initiate sample
}

unsigned adcUpdate2(unsigned char chA, unsigned char chB)
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

void adcUpdateAll(unsigned enable) // Update the ADC Buffers
{
    ADCON2Hbits.CSCNA = 1; // Enable Channel Scan
    // TODO: adcUpdateAll() needs finisheds
    ADCON2Hbits.CSCNA = 0; // Disable Channel Scan

}

void pwmSetup() // Initialize TMR2 (you get to setup on your own)
{

}
unsigned pwmSet(unsigned char, unsigned char) // Set pin to duty cycle
{
    
}