/*! \file
 * \\brief This file is for later use for power management functions as we see fit for them.
 * May not even use at all.
 */

#include <powerman.h>                         //PIC hardware mapping
#include <eeprom.h>                           //eeprom memory
#include <math.h>
#include <lcd.h>
#include <hid.h>

void power_setup(void)
{
    TRISAbits.TRISA4 = 0;   // RA4 set as output to relay
    TRISAbits.TRISA5 = 1;   // battery input
<<<<<<< Updated upstream
    LATAbits.LATA4=0;         //output initially set to zero
}



//check battery level using adc converter
void power_loop(void)        //Power switch depending on battery level
{
    double battvolt;
    unsigned int rawbat;
    unsigned char set;
    static unsigned errored = 0;
    set = (setting_bits1 | 0b00110000) >> 4;
    rawbat = adc_read(ANBATT);

//KEEP IN MIND: VOLTAGE DIVIDER!!!!
/// \todo FIXME: What happens if our battery is low and we lose AC? (Right now, we loose the light and motors)
    if((battvolt<2f)  || (set == 1))     //turns battery off if voltage too low
    {
        LATAbits.LATA4 = 0;            //This will change the relay to AC power.
    }
    else if((battvolt>2.3) && (set > 1))      //turns battery on if voltage too high or forced
    {
        LATAbits.LATA4 = 1;            //This will the turn the AC power off.
    }

    if(battvolt < 0.6f&& battvolt > 0.2) // error when low but there is a battery
    {
        if(errored == 0)
        {
            mem_append_log(ERR_BATTLOW);
            errored = 1;
        }
    }

}


