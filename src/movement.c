#include <movement.h>
#include <lcd.h>


/* Global variable matrix for analog limits on potentiometers
 * This is READ ONLY for all funtions. DO NOT WRITE TO THIS MATRIX
 * The ONLY function that may modify these values is limit_test()
 */
static unsigned int limitPot[5] = 0;

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
    return;
}

/*! \brief Move the panel to each limit to find values on pots
 *
 * --> Test value (global var) used to show if function was entered
 */
void limit_test(void){
    unsigned int potTimeEst;
    datetime currenttime;

        // Find UP limit
    while(limits() != LIMIT_UP)
    {
        motor_move(UP);
    }
    limitPot[UP] = ang_pos(POT_YEAR, 1);

        // Find EAST limit
    while(limits() != LIMIT_EAST)
    {
        motor_move(EAST);
    }
    limitPot[EAST] = ang_pos(POT_DAY, 1);

        // Find WEST limit
    while(limits() != LIMIT_WEST)
    {
        motor_move(WEST);
    }
    limitPot[WEST] = ang_pos(POT_DAY, 1);

    limitPot[NOON] = (limitPot[WEST] + limitPot[EAST])/2;

        // Move to middle (noon)
    day_pos_move(limitPot[NOON]);


    // Find DOWN limit
    while(limits() != LIMIT_DOWN)
    {
        motor_move(DOWN);
    }
    limitPot[DOWN] = ang_pos(POT_YEAR, 1);


        // move halfway up (Auto Optimal Position)
    year_pos_move((limitPot[UP] + limitPot[DOWN])/2);


        // figure out if it is day or night
    rtc_get(&currenttime);
    if (currenttime.hour < 6 || currenttime.hour > 18)
    {
        dusk_moveback();
        return;
    }

    if(photo_value(PHOTO_LEV, 1) < CLOUDY) // if not able to track sun, estimate position based on time of day
    {
        potTimeEst = limitPot[NOON] + (((unsigned int)currenttime.hour - 12) * abs(limitPot[EAST] - limitPot[WEST])/6);

        if(potTimeEst < limitPot[EAST] || potTimeEst > limitPot[WEST])      // check if value is within range of motion
        {
           dusk_moveback();
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
    static unsigned char last_adjust = 0;
    static unsigned char last_record = 0;
    static unsigned char last_move = 0;
    unsigned int potAvg = 0;
    unsigned int potTimeEst;
    static unsigned int track[3][24][12] = 0;          // day, hour, minute
    datetime currenttime;

    rtc_get(&currenttime);

    if(photo_value(PHOTO_LEV, 0) > CLOUDY)      // if bright, move from sensors
    {
        if(abs(photo_value(PHOTO_EAST, 1) - photo_value(PHOTO_WEST, 1)) > ERR)
        {
            if(photo_value(PHOTO_EAST, 0) < photo_value(PHOTO_WEST, 0))
            {
                motor_move(WEST);
            }
            else
            {
                motor_move(EAST);
            }
        }
        else
        {
            motor_move(STOP);
        }
        // check if time to do seasonal adjust
        // if between 1200-1259, not already done today, and close to middle
        if((currenttime.hour == 12) && (currenttime.day != last_adjust))
        {
            motor_move(STOP);       // to make sure panel is not moving on two axis at once
            season_adjust();
            last_adjust = currenttime.day;
        }

        // Record position every five minutes for a cloudy day
        if((currenttime.minute%5 == 0) && (currenttime.minute != last_record))
        {               // move previous values to ealier
            track[2][currenttime.hour][currenttime.minute/5]  = track[1][currenttime.hour][currenttime.minute/5];
            track[1][currenttime.hour][currenttime.minute/5]  = track[0][currenttime.hour][currenttime.minute/5];

                        // record current position
            track[0][currenttime.hour][currenttime.minute/5] = ang_pos(POT_DAY, 1);

            last_record = currenttime.minute;
        }
    }
    else                // move from memory
    {
        if((currenttime.minute%5 == 0) && (currenttime.minute != last_move))
        {
            if(track[0][currenttime.hour][currenttime.minute/5] == 0)       // if no data to draw from, estimate from time of day
            {
                if(currenttime.minute == 0)
                    {
                    potTimeEst = limitPot[NOON] + (((unsigned int)currenttime.hour - 12) * abs(limitPot[EAST] - limitPot[WEST])/6);
                    if(potTimeEst < limitPot[EAST] || potTimeEst > limitPot[WEST])      // check if value is within range of motion
                    {
                       dusk_moveback();
                    }
                    else
                    {
                        day_pos_move(potTimeEst);
                    }
                }
            }
            else
            {           // average data 
                if(track[1][currenttime.hour][currenttime.minute/5] == 0)  // check if data exists
                {
                    potAvg = track[0][currenttime.hour][currenttime.minute/5];
                }
                else if(track[2][currenttime.hour][currenttime.minute/5] == 0)
                {
                    potAvg = (track[0][currenttime.hour][currenttime.minute/5] + track[1][currenttime.hour][currenttime.minute/5])/2;
                }
                else
                {
                    potAvg = (track[0][currenttime.hour][currenttime.minute/5] + track[1][currenttime.hour][currenttime.minute/5] + track[2][currenttime.hour][currenttime.minute/5])/3;
                }
                day_pos_move(potAvg);
            }
            last_move = currenttime.minute;
        }
    }
}

/*! \brief Move back to the east for the next day
 */
void dusk_moveback(void)
{      
    day_pos_move(limitPot[EAST]);
}

/*! \brief Command the motors to move in the direction specified
 */
void motor_move(unsigned char direction){
    switch(direction)
    {
        case UP:
            PIN_MOVE_UP_1 = 1;
            PIN_MOVE_UP_2 = 1;
            PIN_MOVE_DOWN_1 = 0;
            PIN_MOVE_DOWN_2 = 0;
            PIN_MOVE_EAST_1 = 0;
            PIN_MOVE_EAST_2 = 0;
            PIN_MOVE_WEST_1 = 0;
            PIN_MOVE_WEST_2 = 0;
        break;
        case DOWN:
            PIN_MOVE_UP_1 = 0;
            PIN_MOVE_UP_2 = 0;
            PIN_MOVE_DOWN_1 = 1;
            PIN_MOVE_DOWN_2 = 1;
            PIN_MOVE_EAST_1 = 0;
            PIN_MOVE_EAST_2 = 0;
            PIN_MOVE_WEST_1 = 0;
            PIN_MOVE_WEST_2 = 0;
        break;
        case EAST:
            PIN_MOVE_UP_1 = 0;
            PIN_MOVE_UP_2 = 0;
            PIN_MOVE_DOWN_1 = 0;
            PIN_MOVE_DOWN_2 = 0;
            PIN_MOVE_EAST_1 = 1;
            PIN_MOVE_EAST_2 = 1;
            PIN_MOVE_WEST_1 = 0;
            PIN_MOVE_WEST_2 = 0;
        break;
        case WEST:
            PIN_MOVE_UP_1 = 0;
            PIN_MOVE_UP_2 = 0;
            PIN_MOVE_DOWN_1 = 0;
            PIN_MOVE_DOWN_2 = 0;
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

    if(photo_value(PHOTO_LEV, 0) < CLOUDY)
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
 * Inputs: desired operation of the panel 
 *
 * Outputs: commands to the motors
 */
void maint_move(unsigned char mcomd){

    /*! Cases
     *
     * SETTINGS: will be permenant until changed by maintenance
     *
     * auto = function as normal
     *      daytime_move();
     *      set do move flag
     *
     * hurricane = fixed horizantal
     *      day_pos_move(pot_noon);
     *      year_pos_move(limitPot[DOWN]);
     *      set no move flag
     *
     * fixed = move to "optimal" position, day flat, year 30ish
     *      day_pos_move(pot_noon);
     *      year_pos_move((2*limitPot[DOWN]+limitPot[UP])/3);
     *      set no move flag
     *
     * stop = do not move, adjust physically
     *      motor_move(STOP);
     *      set no move flag
     *
     * TESTS: will default back to auto when maintenance mode exitted
     *
     * auto = function as normal
     *      daytime_move();
     *
     * thurricane = fixed horizantal
     *      day_pos_move(pot_noon);
     *      year_pos_move(limitPot[DOWN]);
     *
     * tfixed = move to optimal position, day flat, year 30ish
     *      day_pos_move(pot_noon);
     *      year_pos_move((2*limitPot[DOWN]+limitPot[UP])/3);
     *
     * limits
     *      limit_test()
     *
     */
}

void day_pos_move(unsigned int desPos)
{
    while(abs(ang_pos(POT_DAY, 1) - desPos) < ERR)
    {
        if (ang_pos(POT_DAY, 0) < desPos)
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
    while(abs(ang_pos(POT_YEAR, 1) - desPos) < ERR)
    {
        if (ang_pos(POT_YEAR, 0) < desPos)
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

unsigned int abs(int num)
{
    if(num < 0)
    {
        num = num * -1;
    }
    return(num);
}

// FIXME: Luke, BEWARE USING INTS. Please try to remove them. They waste cpu. - Ben
// Please see lcd.c and eeprom.h for details on command stuff.

void mhurricane(void)
{
    while(limits() != PIN_LIMIT_DOWN)
    {
        if(keypad_pull() == CANCEL)
        {
            return;
        }

        motor_move(DOWN);
    }
}
