/*! \file
 * \\brief This file is for later use for power management functions as we see fit for them.
 * May not even use at all.
 */

#include <powerman.h>                         //PIC hardware mapping

void power_intiate()
{
    PIE2bits.HLVDIE=1;      //  initiated in order to enable HLVD interupts
    INTCONbits.GIE=1;


    PORTAbits.RA4 = 0;   // RA4 set as output to relay
    PORTAbits.RA5 = 0;   // battery input
    //ANSELAbits.AN4=1;        //analog input is AN4
    LATAbits.LATA4=0;         //output initially set to zero


    //setup HLVD
    HLVDCONbits.HLVDEN=1;       //enable HLVD
    HLVDCONbits.VDIRMAG=0;  //Low-level voltage detect
    HLVDCONbits.IRVST=1;    //Internal reference voltage stable flag bit
    HLVDCONbits.HLVDL=1111;    //external analog input is used

}

void interrupt ipr()        //Power switch depending on battery level
{
    if(HLVDIE && HLVDIF)
        PORTAbits.RA4 = 1;            //This will change the relay to grid power.
}

void power_grid_chk()      //Check for main grid power if battery is getting low.
{
    if(HLVDIE)
            //needs to enter sleep mode? Ben?

}

void power_batt_chk()       //Check for battery power before switching.
{
    

}

    //TODO: Any other power interupts??