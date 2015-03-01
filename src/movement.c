#include <movement.h>


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

    // Setup PWM for motors

}


void limit_test(void){          // move full range to find limits on pots
    move = move^0b00000001;
    /*
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

void daytime_move(void){        // move the panel to point at the sun, either from sensors or memory
    move = move^0b00000010;
}

void dusk_moveback(void){       // move back to the east for the next day
    move = move^0b00000100;
}

void season_adjust(void){     // move up or down for seasonal shift and at intialization
    move = move^0b00001000;
}

void maintenance_move(unsigned char){       // move as directed by maintenance panel
    move = move^0b00010000;
}
