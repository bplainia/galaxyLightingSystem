/*! \file
 * \\brief This file is for later use for power management functions as we see fit for them.
 * May not even use at all.
 */

#include <powerman.h>                         //PIC hardware mapping
#include <eeprom.h>                           //eeprom memory
#include <math.h>

void power_setup(void)
{
//    PIE2bits.HLVDIE=1;      //  initiated in order to enable HLVD interupts
//    INTCONbits.GIE=1;


    PORTAbits.RA4 = 0;   // RA4 set as output to relay
    TRISAbits.TRISA5 = 0;   // battery input
    //ANCON1bits.ANSEL4=1;        //analog input is AN4
    LATAbits.LATA4=0;         //output initially set to zero


    //setup HLVD
//    HLVDCONbits.HLVDEN=1;       //enable HLVD
//    HLVDCONbits.VDIRMAG=0;  //Low-level voltage detect
//    HLVDCONbits.IRVST=1;    //Internal reference voltage stable flag bit
//    HLVDCONbits.HLVDL=1111;    //external analog input is used

}


//The following are designed to send error alerts to the error page for maintenance.
void power_switch(unsigned char)
{
    battin = battin ^ 0b1;
}

void batt_live(unsigned char none)
{
    float battvolt;
    unsigned int rawvoltage;
    rawvoltage = (ADCBUF4H << 8) | ADCBUF4L;
    battvolt = rawvoltage*3.3/4096;       //input variable

    if(battvolt < 1.0f)
    {

 //       menu[7].entry[]


    }
}


void power_loop(void)        //Power switch depending on battery level
{
    float battvolt;
    unsigned int rawvoltage;
    rawvoltage = (ADCBUF4H << 8) | ADCBUF4L;
    battvolt = rawvoltage*3.3/4096;       //input variable


    if((battvolt<10.5f) | (battin==1))    //turns battery off if voltage too low
    {
        LATAbits.LATA4 = 0;            //This will change the relay to grid power.
    }


    if((battvolt>11.5f) | (battin==0))      //turns battery on if voltage too high
    {
        LATAbits.LATA4 = 1;            //This will the grid power off.
    }

    if(battvolt > 11.5)
    {
        mem_append_log(ERR_BATTLOW);
    }

}


