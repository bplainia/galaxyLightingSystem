
//#include <xc8.h>
#include <movement.h>


void movement_initialize(void){

    // Define motor pins as outputs

    PINIO_MOVE_LEFTRIGHT = 0;
    PINIO_MOVE_UPDOWN = 0;
}


void movement_test(void)    // move full range to find limits on pots
{

}

void daytime_move(void)     // move the panel to point at the sun, either from sensors or memory
{

}

void dusk_moveback(void)    // move back to the east for the next day
{

}

void seasonal_adjust(void)  // move up or down for seasonal shift and at intialization
{

}

void maintenance_move(unsigned char) // move as directed by maintenance panel
{

}



