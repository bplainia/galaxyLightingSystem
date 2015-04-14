#include <sensors.h>

#include "rtc.h"

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
    unsigned int photoVal = 0;

    if(update)
    {
        switch (photonum)
        {
            case PHOTO_LEV:
                adc_update(CHAN_PHOTO_LEV);
                break;
            case PHOTO_YEAR:
                adc_update(CHAN_PHOTO_YEAR);
                break;
            case PHOTO_EAST:
                adc_update(CHAN_PHOTO_EAST);
                break;
            case PHOTO_WEST:
                adc_update(CHAN_PHOTO_WEST);
                break;
        }
    }

    switch (photonum)
    {
        case PHOTO_LEV:
            photoVal = adc_read(CHAN_PHOTO_LEV);
            break;
        case PHOTO_YEAR:
            photoVal = adc_read(CHAN_PHOTO_YEAR);
            break;
        case PHOTO_EAST:
            photoVal = adc_read(CHAN_PHOTO_EAST);
            break;
        case PHOTO_WEST:
            photoVal = adc_read(CHAN_PHOTO_WEST);
            break;
    }

    return(photoVal);
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
unsigned int rotational_postion(unsigned potnum, unsigned update){
    unsigned int potVal = 0;
    
    if(update)
    {
        switch (potnum)
        {
             case POT_DAY:
                adc_update(CHAN_POT_DAY);
             case POT_YEAR:
                adc_update(CHAN_POT_YEAR);
        }
    }
    switch (potnum)
    {
         case POT_DAY:
            potVal = adc_read(CHAN_POT_DAY);
         case POT_YEAR:
            potVal = adc_read(CHAN_POT_YEAR);
    }

    return potVal;
}

/*! \brief check the PIR sensor and turn light ON or DIM depending on motion
 *   Only active during night time state
 *
 * Inputs: none
 *
 * Outputs: Bit value showing if movement is detected or not
 */
unsigned pir(void){

    static unsigned last = NO_MOVE;
    static unsigned led_on = OFF;
    static unsigned char ten_min_min = 60;
    static unsigned char ten_min_sec = 60;
    static unsigned char five_sec = 60;
    datetime currenttime;

    rtc_get(&currenttime);
        
    if (PIN_PIR == MOVE)
    {
        if (last == NO_MOVE)
        {
            five_sec = currenttime.second;
            if (five_sec < 5)
            {
                five_sec = five_sec + 60;
            }
            five_sec = five_sec - 5;
        }
        else if(currenttime.second <= five_sec)
        {
            led(ON);
            led_on = ON;
        }
        last = MOVE;
    }
    else if(led_on = ON)
    {

        if (last == MOVE)
        { 
            ten_min_min = currenttime.minute;
            ten_min_sec = currenttime.second;
            if(ten_min_min < 10)
            {
                ten_min_min = ten_min_min + 60;
            }
            ten_min_min = ten_min_min - 10;
        }
        else if((currenttime.minute <= ten_min_min) && (currenttime.second <= ten_min_sec))
        {
            led(DIM);
            led_on = OFF;
        }
        last = NO_MOVE;
    }
    return last;
}

unsigned char limits(void)
{
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

