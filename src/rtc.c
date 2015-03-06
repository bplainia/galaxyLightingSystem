#include <shared.h>
#include <rtc.h>

/// Real-time Clock/Calendar Module setup
void rtc_setup()
{
    //! \todo TODO: Check to see if the rtcc is setup already
    RTCCON1bits.RTCEN = 1; // Enable RTC Clock
    status.rtcInit = 0; // real time clock has not been setup since reset!
}

/*! \brief Set the time on the RTCC
 *
 * Inputs: a bunch of bytes specifying the year, month,day, hour, minutes, and seconds.
 * Outputs: A single bit. Probably will never return failure.
 * Detailed Description:
 * Note: Will convert the numbers to BCD endcoding before returning them
 */
unsigned rtc_set(unsigned char year, unsigned char month, unsigned char day,
        unsigned char hour, unsigned char minute, unsigned char second)
{
    EECON2 = 0x55;           // Perform unlock sequence for special things
    EECON2 = 0xAA;
    RTCCON1bits.RTCWREN = 1; // Enable write to real time clock
    RTCCON1bits.RTCPTR = 0b11; // Let's start with writing the year
    RTCVALL = char2bcd(year);// Write Year
    RTCVALH = char2bcd(month); // write month (rtcptr at 10)
    RTCVALL = char2bcd(day);
    RTCVALH = weekday(year,month,day); // write weekday (already only one digit)
    RTCVALL = char2bcd(hour); // write hours (remember 24 hr format
    RTCVALH = char2bcd(minute); // write minutes
    RTCVALL = char2bcd(second); // write seconds (most likely 0)
    RTCCON1bits.RTCWREN = 0; // Finished. Disable write to rtc.
    status.rtcInit = 1;
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
    char mod, diff, div;
    mod = (year % 12);
    diff = year - (mod*12);
    if(diff < 0) diff = -diff; // absolute function
    div = diff / 4;
    return (mod+diff+div) % 7; // returns weekday 0 (sunday) to 6 (saturday)
}