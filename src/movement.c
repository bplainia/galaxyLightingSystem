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
    move = move^0b00000001;
    /*!
     * Move up all the way (rec: uplimit)
     * Move right all the way (rec: eastlimit)
     * Move left all the way (rec: westlimit)
     * Move to the lr middle (eastlimit/2 + westlimit/2) (noon)
     * Mov down all the way (downlimit)
     * if photo_value(PHOTO_LEV) > cloudy
     *      season_adjust()
     *      daytime_move()
     * else
     *      move to updown middle (uplimit/2 + downlimit/2)
     *      move to lr time_estimate
     */
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
void dusk_moveback(void){      
    unsigned short year;
    unsigned short east;
    unsigned short west;
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

    if(adc_read(CHAN_PHOTO_LEV) < CLOUDY){
        return;}

    while(err > ERR)
    {
        yearLev = adc_read(CHAN_PHOTO_YEAR);
        eastLev = adc_read(CHAN_PHOTO_EAST);
        westLev = adc_read(CHAN_PHOTO_WEST);

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
 *
 * --> Test values (returns) used to show if function preformed was entered
 */
void maint_move(unsigned char mcomd){
    move = move^0b00010000;

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

