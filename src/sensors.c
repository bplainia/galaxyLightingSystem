#include <sensors.h>

#include <rtc.h>
#include <light.h>
#include <shared.h>
#include <movement.h>
#include <eeprom.h>

/*! \brief setup sensors
 */
void sensor_setup(void){

    // Define sensor pins as inputs
    PINIO_POT_LEFTRIGHT = INPUT;
    PINIO_POT_UPDOWN = INPUT;
    PINIO_LIGHTLEVEL = INPUT;
    PINIO_YEARLEVEL = INPUT;
    PINIO_DAYLEVEL1 = INPUT;
    PINIO_DAYLEVEL2 = INPUT;
    PINIO_TEMP = INPUT;
    PINIO_PIR = INPUT;
    PINIO_LIMIT_DOWN = INPUT;
    PINIO_LIMIT_UP = INPUT;
    PINIO_LIMIT_EAST = INPUT;
    PINIO_LIMIT_WEST = INPUT;

    // Define inputs as digital or analog
    PINAD_POT_LEFTRIGHT = ANALOG;
    PINAD_POT_UPDOWN = ANALOG;
    PINAD_LIGHTLEVEL = ANALOG;
    PINAD_YEARLEVEL = ANALOG;
    PINAD_DAYLEVEL1 = ANALOG;
    PINAD_DAYLEVEL2 = ANALOG;
    PINAD_TEMP = ANALOG;
    PINAD_LIMIT_DOWN = DIGITAL;
    PINAD_LIMIT_UP = DIGITAL;
    PINAD_LIMIT_EAST = DIGITAL;
    PINAD_LIMIT_WEST = DIGITAL;
    return;
}

/*! \brief Check the analog value of a particular photoresistor
 *
 * Inputs: designator of photoresistor to be checked (1, 2, 3, or 4)
 *
 * Outputs: analog value off that photoresistor in binary
 *
 * --> Test values (returns) used to show if function preformed as desired
 */
unsigned int photo_value(unsigned char photonum, unsigned update)
{
    static unsigned int photoLev  = 0;
    static unsigned int photoYear = 0;
    static unsigned int photoEast = 0;
    static unsigned int photoWest = 0;

    if(update)      // if requested do a full adc conversion
    {
        switch (photonum)
        {
            case PHOTO_LEV:
                photoLev=adc_read(CHAN_PHOTO_LEV);
                break;
            case PHOTO_YEAR:
                photoYear = adc_read(CHAN_PHOTO_YEAR);
                break;
            case PHOTO_EAST:
                photoEast = adc_read(CHAN_PHOTO_EAST);
                break;
            case PHOTO_WEST:
                photoWest = adc_read(CHAN_PHOTO_WEST);
                break;
        }
    }
                    // then read adc value
    switch (photonum)
    {
        case PHOTO_LEV:
            return photoLev;
        case PHOTO_YEAR:
            return photoYear;
        case PHOTO_EAST:
            return photoEast;
        case PHOTO_WEST:
            return photoWest;
        default:
            return 0;
    }
}

/*! \brief Check the temperature inside the enclosure
 *
 * Outputs: Temperature at the sensor in degF
 *
 * --> Test value (return) used to show if function preformed as desired
 */
unsigned char temperature(void){
    unsigned char temp = 25;
    unsigned int rawTemp;
   
    rawTemp = adc_read(CHAN_TEMP);

    // temp = c(adc)

    return temp;
}


/*! \brief Check the analog value of a particular potentimeter
 *
 * Inputs: designator of potentimeter to be checked (1 or 2)
 *
 * Outputs: analog value off that potentimeter in binary
 *
 * --> Test values (returns) used to show if function preformed as desired
 */
unsigned int ang_pos(unsigned potnum, unsigned update){
    static unsigned int potVal = 0;
    static unsigned int potVal2 = 0;
    
    if(update)      // if requested do a full adc conversion
    {
        switch (potnum)
        {
             case POT_DAY:
                potVal = adc_read(CHAN_POT_DAY);
             case POT_YEAR:
                potVal2 = adc_read(CHAN_POT_YEAR);
        }
    }
    switch (potnum)     // then read adc value
    {
        case POT_DAY:
             return potVal;
        case POT_YEAR:
             return potVal2;
        default:
            return 0;
    }
}

/*! \brief check the PIR sensor and turn light LED_ON or DIM depending on motion
 *   Only active during night time state
 *
 * Inputs: none
 *
 * Outputs: Bit value showing if movement is detected or not
 */
unsigned pir(void){

    static unsigned last = NO_MOVE;
    static unsigned led_on = LED_OFF;

    unsigned char setting;

    setting = (setting_bits1 & 0b00001100) >> 2;
    if(setting == 0b11 && (setting_timeout > 0))
    {

        // when movement occurs, reset 5 second timer
        // if movement has continued for more than five seconds, turn the light on

        if (PIN_PIR == 1)
        {
            if (last == NO_MOVE)
            {
                pirLowTime = 0;
            }
            else if(pirLowTime > 5)
            {
                led(LED_ON);
                led_on = LED_ON;
                pirTime = 0;
            }
            last = MOVE;
        }

        // when there movement stops, and if the light is on (check for speed)
        // reset the 10 minute timer
        // after 10 minutes, turn the light back to DIM
        // 10 min timer is only reset if motion is detected for more than 5 seconds

        else if(led_on == LED_ON)   //
        {
            if(pirTime >= setting_timeout * (unsigned char)60 )
            {
                led(LED_DIM);
                led_on = LED_OFF;
            }
            last = NO_MOVE;
            pirLowTime = 0;
        }
        return last;
    }
    else if(setting == 0) led(OFF);
    else if(setting == 1) led(DIM);
    else led(ON); // setting = 2
    return false;
}

unsigned char limits(void)
{
    // If any limit switch is pressed - stop all motors
    if(PIN_LIMIT_DOWN || PIN_LIMIT_UP || PIN_LIMIT_EAST || PIN_LIMIT_WEST)
    {
        motor_move(STOP);
        if(PIN_LIMIT_DOWN)
        {
            motor_move(UP);// move up until no contact + a little
            return(LIMIT_DOWN);
        }
        else if(PIN_LIMIT_UP)
        {
            motor_move(DOWN);// move down until no contact + a little
            return(LIMIT_UP);
        }
        else if(PIN_LIMIT_EAST)
        {
            motor_move(WEST);// move west until no contact + a little
            return(LIMIT_EAST);
        }
        else if(PIN_LIMIT_WEST)
        {
            motor_move(EAST);// move east until no contact + a little
            return(LIMIT_WEST);
        }
        else
        {
            ;//error
        }
    }
    return(0);
}

