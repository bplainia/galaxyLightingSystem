#include <movement.h>

//! \todo TODO: For Luke: Change the functions according to our format (e.g. move_setup(), move_maint(), etc.)

/*! \brief setup motors
 */
void move_setup(void){

    // Define motor pins as outputs
    PINIO_MOVE_LEFTRIGHT_1 = 0;
    PINIO_MOVE_LEFTRIGHT_2 = 0;
    PINIO_MOVE_LEFTRIGHT_3 = 0;
    PINIO_MOVE_LEFTRIGHT_4 = 0;
    PINIO_MOVE_UPDOWN_1 = 0;
    PINIO_MOVE_UPDOWN_2 = 0;
    PINIO_MOVE_UPDOWN_3 = 0;
    PINIO_MOVE_UPDOWN_4 = 0;

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
 *
 * --> Test values (global var) used to show if function was entered
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

/*! \brief Adjust the panel (if needed) based on the season
 *
 * Outputs: commands to the motors
 *
 * --> Test value (global var) used to show if function was entered
 */
void season_adjust(void){     // move up or down for seasonal shift and at intialization
//    if(adc(level) < cloudy)
//        return;
//
//    unsigned short year;
//    unsigned short east;
//    unsigned short west;
//    unsigned short err = ERR + 1;
//
//    while(err > ERR)
//    {
//        year = adc(year);
//        east = adc(east);
//        west = adc(west);
//
//        if((east/2 + west/2) > year)
//            move(down);
//        else
//            move(up);
//        err = (east/2 + west/2) - year;
//    }
//    move(stop);
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
