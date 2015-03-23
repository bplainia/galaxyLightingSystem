#include <sensors.h>

/*! \brief setup sensors
 */
void sensor_setup(void){

    // Define sensor pins as inputs
    PINIO_POT_LEFTRIGHT = 1;
    PINIO_POT_UPDOWN = 1;
    PINIO_LIGHTLEVEL = 1;
    PINIO_YEARLEVEL = 1;
    PINIO_DAYLEVEL1 = 1;
    PINIO_DAYLEVEL2 = 1;
    PINIO_TEMP = 1;
    PINIO_PIR = 1;
    PINIO_LIMIT_DOWN = 1;
    PINIO_LIMIT_UP = 1;
    PINIO_LIMIT_EAST = 1;
    PINIO_LIMIT_WEST = 1;

    // Define inputs as digital (0) or analog (1)
    PINAD_POT_LEFTRIGHT = 1;
    PINAD_POT_UPDOWN = 1;
    PINAD_LIGHTLEVEL = 1;
    PINAD_YEARLEVEL = 1;
    PINAD_DAYLEVEL1 = 1;
    PINAD_DAYLEVEL2 = 1;
    PINAD_TEMP = 1;
    PINAD_LIMIT_DOWN = 0;
    PINAD_LIMIT_UP = 0;
    PINAD_LIMIT_EAST = 0;
    PINAD_LIMIT_WEST = 0;

}

/*! \brief Check the analog value of a particular photoresistor
 *
 * Inputs: designator of photoresistor to be checked (1, 2, 3, or 4)
 *
 * Outputs: analog value off that photoresistor in binary
 *
 * --> Test values (returns) used to show if function preformed as desired
 */
unsigned short photo_value(unsigned char photonum){
    switch (photonum)
    {
        case 1:
            return 0; //adc off PIN_LIGHTLEVEL
        case 2:
            return 0; //adc off PIN_DAYLEVEL1
        case 3:
            return 0; //adc off PIN_DAYLEVEL2
        case 4:
            return 0; //adc off PIN_YEARLEVEL
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
char temperature(void){
    char temp = 25;
   
    //adc off PIN_TEMP

    // temp = f(adc)

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
unsigned short rotational_postion(unsigned char potnum){
    unsigned short potval;
    switch (potnum)
    {
            case 1:
                return 0; //adc off PIN_DAY_POT
            case 2:
                return 0; //adc off PIN_YEAR_POT
    }
    return potval;
}

/*! \brief check the PIR sensor and turn light ON or DIM depending on motion
 *   only active during night time
 *
 * Inputs: none
 */
unsigned char last = NO_MOVE;

unsigned char pir(void){

//    if (PIR_LEVEL == MOVE)
//    {
//        if (last == NO_MOVE)
//        {
//            start short_timer;
//        }
//        else
//        {
//            if (short_timer > 5sec)
//            {
//                led(ON);
//                reset/start long_timer;
//            }
//        }
//        last = MOVE;
//    }
//    else
//    {
//        reset/stop short_timer;
//        if (long_timer > 10min)
//        {
//            led(DIM);
//            stop long_timer
//        }
//        last = NO_MOVE;
//    }
    return last;
}

void limits(void)
{
    if(PIN_LIMIT_DOWN || PIN_LIMIT_UP || PIN_LIMIT_EAST || PIN_LIMIT_WEST)
    {
        //Stop all motors
        if(PIN_LIMIT_DOWN)
        {
            ;// move up until no contact + a little
        }
        else if(PIN_LIMIT_UP)
        {
            ;// move down until no contact + a little
        }
        else if(PIN_LIMIT_EAST)
        {
            ;// move west until no contact + a little
        }
        else if(PIN_LIMIT_WEST)
        {
            ;// move east until no contact + a little
        }
        else
        {
            ;//error
        }
    }
}

