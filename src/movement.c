#include <movement.h>

//! \todo TODO: For Luke: Change the functions according to our format (e.g. move_setup(), move_maint(), etc.)

/*! \brief setup motors
 */
void move_setup(void){

    // Define motor pins as outputs
    PINIO_MOVE_LEFTRIGHT_1 = OUTPUT;
    PINIO_MOVE_LEFTRIGHT_2 = OUTPUT;
    PINIO_MOVE_LEFTRIGHT_3 = OUTPUT;
    PINIO_MOVE_LEFTRIGHT_4 = OUTPUT;
    PINIO_MOVE_UPDOWN_1 = OUTPUT;
    PINIO_MOVE_UPDOWN_2 = OUTPUT;
    PINIO_MOVE_UPDOWN_3 = OUTPUT;
    PINIO_MOVE_UPDOWN_4 = OUTPUT;

    //! \todo  TODO -> Setup PWM for motors

}

/*! \brief Move the panel to each limit to find values on pots
 *
 * --> Test value (global var) used to show if function was entered
 */
void limit_test(void){       
    static unsigned int limit_pot_up = 0;
    static unsigned int limit_pot_down = 0;
    static unsigned int limit_pot_east = 0;
    static unsigned int limit_pot_west = 0;
    static unsigned int pot_noon = 0;
    unsigned int potTimeEst;
    datetime currenttime;

        // Find UP limit
    while(limits() != LIMIT_UP)
    {
        motor_move(UP);
    }
    limit_pot_up = rotational_postion(POT_YEAR, 1);

        // Find EAST limit
    while(limits() != LIMIT_EAST)
    {
        motor_move(EAST);
    }
    limit_pot_east = rotational_postion(POT_DAY, 1);

        // Find WEST limit
    while(limits() != LIMIT_WEST)
    {
        motor_move(WEST);
    }
    limit_pot_west = rotational_postion(POT_DAY, 1);

    pot_noon = (limit_pot_west + limit_pot_east)/2;

        // Move to middle (noon)
    day_pos_move(POT_DAY, pot_noon);


    // Find DOWN limit
    while(limits() != LIMIT_DOWN)
    {
        motor_move(DOWN);
    }
    limit_pot_down = rotational_postion(POT_YEAR, 1);


        // move halfway up (Auto Optimal Position)
    year_pos_move((limit_pot_up + limit_pot_down)/2);


        // figure out if it is day or night
    rtc_get(&currenttime);
    if (currenttime.hour < 6 || currenttime.hour > 18)
    {
        dusk_moveback(limit_pot_east);
        return;
    }

    if(photo_value(PHOTO_LEV, 1) < CLOUDY) // if not able to track sun, estimate position based on time of day
    {
        potTimeEst = pot_noon + (((unsigned int)currenttime.hour - 12) * abs(limit_pot_east - limit_pot_west)/6);
        if(potTimeEst < limit_pot_east || potTimeEst > limit_pot_west)      // check if value is within limits
        {
           dusk_moveback(limit_pot_east);
        }
        else
        {
            day_pos_move(potTimeEst);
        }
    }
    else            // auto mode
    {
        daytime_move();
    }
}

/*! \brief Move the panel to point at the sun, either from sensors or memory
 */
void daytime_move(void){ 

//    if((adc(level) > cloudy) && ((adc(east)-adc(west) < ERR))

        season_adjust();
}

/*! \brief Move back to the east for the next day
 *
 * --> Test value (global var) used to show if function was entered
 */
void dusk_moveback(unsigned int eastLimit)
{      
    day_pos_move(eastLimit);
}


void motor_move(char direction){
    switch(direction)
    {
        case UP:
            PIN_MOVE_UP_1 = 1;
            PIN_MOVE_UP_2 = 1;
            PIN_MOVE_DOWN_1 = 0;
            PIN_MOVE_DOWN_2 = 0;
        break;
        case DOWN:
            PIN_MOVE_UP_1 = 0;
            PIN_MOVE_UP_2 = 0;
            PIN_MOVE_DOWN_1 = 1;
            PIN_MOVE_DOWN_2 = 1;
        break;
        case EAST:
            PIN_MOVE_EAST_1 = 1;
            PIN_MOVE_EAST_2 = 1;
            PIN_MOVE_WEST_1 = 0;
            PIN_MOVE_WEST_2 = 0;
        break;
        case WEST:
            PIN_MOVE_EAST_1 = 0;
            PIN_MOVE_EAST_2 = 0;
            PIN_MOVE_WEST_1 = 1;
            PIN_MOVE_WEST_2 = 1;
        break;
        case STOP:
            PIN_MOVE_UP_1 = 0;
            PIN_MOVE_UP_2 = 0;
            PIN_MOVE_DOWN_1 = 0;
            PIN_MOVE_DOWN_2 = 0;
            PIN_MOVE_EAST_1 = 0;
            PIN_MOVE_EAST_2 = 0;
            PIN_MOVE_WEST_1 = 0;
            PIN_MOVE_WEST_2 = 0;
        break;
    }
}

/*! \brief Adjust the panel (if needed) based on the season
 *
 * Outputs: commands to the motors
 *
 * --> Test value (global var) used to show if function was entered
 */
void season_adjust(void){     // move up or down for seasonal shift and at intialization

    unsigned int yearLev;
    unsigned int eastLev;
    unsigned int westLev;
    unsigned int err = ERR + 1;

    if( photo_value(PHOTO_LEV, 0) < CLOUDY)
    {
        return;
    }

    while(err > ERR)
    {
        yearLev = photo_value(PHOTO_YEAR, 1);
        eastLev = photo_value(PHOTO_EAST, 1);
        westLev = photo_value(PHOTO_WEST, 1);

        if((eastLev/2 + westLev/2) > yearLev)
            motor_move(DOWN);
        else
            motor_move(UP);
        err = (eastLev/2 + westLev/2) - yearLev;
    }
    motor_move(STOP);
}

/*! \brief Move as directed by maintenance panel
 *
 * Inputs: desired operation of the panel (1-8)
 *
 * Outputs: commands to the motors
 */
void maint_move(unsigned char mcomd){

    /*! Cases
     *
     * SETTINGS: will be permenant until changed by maintenance
     *
     * auto = function as normal
     *
     * hurricane = fixed horizantal
     *
     * fixed = move to "optimal" position, day flat, year 30ish
     *
     * fixed seasonal = only move day to day
     *
     * stop = do not move, adjust physically
     *
     * TESTS: will default back to auto when maintenance mode exitted
     *
     * auto = function as normal
     *
     * thurricane = fixed horizantal
     *
     * tfixed = move to optimal position, day flat, year 30ish
     *
     * limits = invoke limit_test()
     *
     */
}

void day_pos_move(unsigned int desPos)
{
    while(abs(rotational_postion(POT_DAY, 1) - desPos) < ERR)
    {
        if (rotational_postion(POT_DAY, 0) < desPos)
        {
            motor_move(WEST);
        }
        else
        {
            motor_move(EAST);
        }
    }
    motor_move(STOP);
}

void year_pos_move(unsigned int desPos)
{
    while(abs(rotational_postion(POT_YEAR, 1) - desPos) < ERR)
    {
        if (rotational_postion(POT_YEAR, 0) < desPos)
        {
            motor_move(UP);
        }
        else
        {
            motor_move(DOWN);
        }
    }
    motor_move(STOP);
}

unsigned int abs(unsigned int num)
{
    if(num < 0)
    {
        num = num * -1;
    }
    return(num);
}

