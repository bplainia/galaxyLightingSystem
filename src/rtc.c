#include <shared.h>
#include <rtc.h>

/// Real-time Clock/Calendar Module setup
void rtc_setup()
{
    //! \todo TODO: Check to see if the rtcc is setup already
    RTCCON1bits.RTCEN = 1; // Enable RTC Clock
    status.rtcInit = 0; // real time clock has not been setup since reset!
    return;
}

/*! \brief Set the time on the RTCC
 *
 * Inputs: a `datetime` structure containing the year, month, day, hour, minute, and second.
 * Outputs: A single bit. Probably will never return failure.
 * Detailed Description:
 * Note: Will convert the numbers to BCD endcoding before returning them
 */
unsigned rtc_set(datetime time)
{
    EECON2 = 0x55;           // Perform unlock sequence for special things
    EECON2 = 0xAA;
    RTCCON1bits.RTCWREN = 1; // Enable write to real time clock
    RTCCON1bits.RTCPTR = 0b11; // Let's start with writing the year
    RTCVALL = char2bcd(time.year);// Write Year
    RTCVALH = char2bcd(time.month); // write month (rtcptr at 10)
    RTCVALL = char2bcd(time.day);
    RTCVALH = weekday(time.year,time.month,time.day); // write weekday (already only one digit)
    RTCVALL = char2bcd(time.hour); // write hours (remember 24 hr format
    RTCVALH = char2bcd(time.minute); // write minutes
    RTCVALL = char2bcd(time.second); // write seconds (most likely 0)
    RTCCON1bits.RTCWREN = 0; // Finished. Disable write to rtc.
    status.rtcInit = 1;
    return false;
}

/*! \brief gives the current time as a `datetime` structure.
 */
unsigned rtc_get(datetime *time)
{
    RTCCON1bits.RTCPTR = 0b11;           // Let's start with the year
    (*time).year    = bcd2char(RTCVALL); // Get the Year
    (*time).month   = bcd2char(RTCVALH); // Get the month (rtcptr at 0b10)
    (*time).day     = bcd2char(RTCVALL); // Get the day of the month
    (*time).weekday = RTCVALH;           // Get the weekday
    (*time).hour    = bcd2char(RTCVALL); // Get the hour (remember 24 hr format)
    (*time).minute  = bcd2char(RTCVALH); // Get the minute
    (*time).second  = bcd2char(RTCVALL); // Get the second (most likely 0)
    return false;
}

// Alarm things
/*! \todo TODO: Documentation for the alarm needs done. sign LNB.
 * \brief Set the alarm date/time
 *
 * Input: an `alarmtime` structure containing the time, date, repetitions (if applicable) and frequency
 * Output: A single bit specifying success (false). If something is invalid, the function will return true.
 *
 * Please see section 17 in the datasheet, particularly figure 17-5, for more detail.
 */
unsigned alarm_set(alarmtime time)
{
    alarm_disable(); // prevent false alarms
    ALRMCFGbits.ALRMPTR = 0b10;

    // if the frequency is incorrect, error
    if(time.frequency < 0b1001)
    {
        return true;
    }

    // Set the month
    if(time.frequency == 0b1001) // yearly
    {
        ALRMVALH = char2bcd(time.month); // set the month
    }
    else
    {
        ALRMVALH = 0; // set the month
    }

    // set the day
    if(time.frequency >= 0b1000) // monthly
    {
        ALRMVALL = char2bcd(time.day); // set the day
    }
    else
    {
        ALRMVALL = 0; // set the day
    }

    // set the weekday
    if(time.frequency == 0b0111) // weekly
    {
        if(time.day > 6)
        {
            return true;
        }
        ALRMVALH = time.day; // set the weekday
    }
    else
    {
        ALRMVALH = 0; // set the weekday to nothing
    }

    // set the hour
    if(time.frequency >= 0b0110) // daily
    {
        ALRMVALL = char2bcd(time.hour); // set the hour
    }
    else
    {
        ALRMVALL = 0; // set the hour to nothing
    }

    // set the minute
    if(time.frequency >= 0b0101) // hourly
    {
        ALRMVALH = char2bcd(time.minute);
    }
    else if(time.frequency == 0b0100) // every 10 min
    {
        ALRMVALH = time.minute % 10;
    }
    else
    {
        ALRMVALH = 0;
    }

    // set the second
    if(time.frequency >= 0b0011) // every minute
    {
        ALRMVALL = char2bcd(time.second);
    }
    else if(time.frequency == 0b0010) // every 10 seconds
    {
        ALRMVALL = time.second % 10;
    }
    else
    {
        ALRMVALL = 0;
    }

    // the last two items are 1=second and 0=half second.
    if(time.repeat > 2)
    {
        return true; // if we were in another language, this would be an exeption
    }
    if(time.repeat > 0)
    {
        ALRMRPT = time.times;
    }
    ALRMCFGbits.CHIME = time.repeat >> 1; // b1 is the chime bit
    ALRMCFGbits.AMASK = time.frequency; // now set the frequency of the alarm

    status.alarmInit = 1; // we now have setup an alarm :)
    return false; // success: nothing bad happened.
}

/*! \brief Enables the alarm. Make sure to setup the alarm first!
 *
 * Output: A bit specifying whether the alarm was enabled correcty.
 *         If the RTCC is off, the RTCC is not setup, or the alarm has not been setup, it will return true.
 */
unsigned alarm_enable()
{
    if(!status.rtcInit || !status.alarmInit) return true; // failed
    ALRMCFGbits.ALRMEN = 1;
    return false;
}

/*! \brief Disable the alarm.
 *
 * Output: Returns false if the alarm was set before. If it was not, returns true.
 */
unsigned alarm_disable()
{
    if(!ALRMCFGbits.ALRMEN) return true;
    ALRMCFGbits.ALRMEN = 0;
    return false;
}

/// Convert a byte into a BCD encoded byte
static unsigned char char2bcd(unsigned char in)
{
    // Will only return the last 2 digits (will ignore the 1 or 2 if it is there)
    // Make sure to use input validation of some sort.
    return in % 10 | ((in/10)%10 << 4); // byte to 2-decimal BCD
    //! \todo  TODO[Review]: Can someone check the RTC stuff?
}

/// Convert a character from BCD encoded to a normal byte
static unsigned char bcd2char(unsigned char in)
{
    return (((in >> 4)*10) + (in & 0x0F));
}

/// What day of the week is it? (0=sunday)
static unsigned char weekday(unsigned char year, unsigned char month, unsigned char day)
{
    // Calculation retrieved from http://www.timeanddate.com/date/doomsday-weekday.html (Feb 6, 2015)
    signed char mod, diff, div;
    mod = (year % 12);
    diff = year - (mod*12);
    if(diff < 0) diff = -diff; // absolute function
    div = diff / 4;
    return (mod+diff+div) % 7; // returns weekday 0 (sunday) to 6 (saturday)
}