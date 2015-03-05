#include <movement.h>

/*! \brief setup motors
 */
void movement_setup(void){

    // Define motor pins as outputs
    PINIO_MOVE_LEFTRIGHT_1 = 0;
    PINIO_MOVE_LEFTRIGHT_2 = 0;
    PINIO_MOVE_LEFTRIGHT_3 = 0;
    PINIO_MOVE_LEFTRIGHT_4 = 0;
    PINIO_MOVE_UPDOWN_1 = 0;
    PINIO_MOVE_UPDOWN_2 = 0;
    PINIO_MOVE_UPDOWN_3 = 0;
    PINIO_MOVE_UPDOWN_4 = 0;

    // TODO -> Setup PWM for motors

}

/*! \brief Move the panel to each limit to find values on pots
 *
 * --> Test value (global var) used to show if function was entered
 */
void limit_test(void){       
    move = move^0b00000001;
    /*!
     * Move down all the way (rec: downlimit)
     * Move right all the way (rec: eastlimit)
     * Move left all the way (rec: westlimit)
     * Move to the lr middle (eastlimit/2 + westlimit/2) (noon)
     * Mov up all the way (uplimit)
     * if photo_value(1) > cloudy
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
    move = move^0b00000010;
}

/*! \brief Move back to the east for the next day
 *
 * --> Test value (global var) used to show if function was entered
 */
void dusk_moveback(void){      
    move = move^0b00000100;
}

/*! \brief Adjust the panel (if needed) based on the season
 *
 * Outputs: commands to the motors
 *
 * --> Test value (global var) used to show if function was entered
 */
void season_adjust(void){     // move up or down for seasonal shift and at intialization
    move = move^0b00001000;
}

/*! \brief Move as directed by maintenance panel
 *
 * Inputs: desired operation of the panel (1-8)
 *
 * Outputs: commands to the motors
 *
 * --> Test values (returns) used to show if function preformed was entered
 */
void maintenance_move(unsigned char){    
    move = move^0b00010000;

    /*! Cases
     *
     * SETTINGS: will be permenant until changed by maintenance
     *
     * auto = function as normal
     *
     * hurricane = fixed horizantal
     *
     * fixed = move to optimal position, day flat, year 30ish
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
