/*! \file
 * \\brief This file is for later use for power management functions as we see fit for them.
 * May not even use at all.
 */

#include <powerman.h>                         //PIC hardware mapping


void power_intiate()
{
    PIE2bits.HLVDIE=1;      //  initiated in order to enable HLVD interupts
    INTOCONbits.GIE=1;


    PORTAbits.RA4 = 0;   // RA4 set as output to relay
    PORTAbits.RA5 = 1;   // battery input
    ANSELbits.AN4=1;        //analog input is AN4
    LATAbits.RA4=0;         //output initially set to zero
    HLVDCON = 0b01110100;   //enables HLVD
}

void interrupt ipr()        //Power switch depending on battery level
{
    if(HLVDIE && HLVDIF)
        PORTAbits.RA4 = 1;            //This will change the relay to grid power.
}

void power_grid_chk()      //Check for main grid power if battery is getting low.
{
    if(HLVDIE)


                            //
}

void power_batt_chk()       //Check for battery power before switching.
{
    if

}

    //TODO: Any other power interupts??