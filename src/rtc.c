#include <shared.h>
#include <rtc.h>

// Private Functions
unsigned char char2bcd(unsigned char);
unsigned char weekday(unsigned char, unsigned char, unsigned char);


void rtc_start()
{
    RTCCON1bits.RTCEN = 1; // Enable RTC Clock
    rtcSetup = 0; // real time clock has not been setup since reset!
}

unsigned char rtc_set(unsigned char year, unsigned char month, unsigned char day,
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
}

unsigned char char2bcd(unsigned char in)
{
    // Will only return the last 2 digits (will ignore the 1 or 2 if it is there)
    // Make sure to use input validation of some sort.
    return in % 10 | ((in/10)%10 << 4); // byte to 2-decimal BCD
    // TODO[Review}: Can someone check the RTC stuff?
}

unsigned char weekday(unsigned char year, unsigned char month, unsigned char day)
{
    // Calculation retrieved from http://www.timeanddate.com/date/doomsday-weekday.html (Feb 6, 2015)
    char mod, diff, div;
    mod = (year % 12);
    diff = year - (mod*12);
    if(diff < 0) diff = -diff; // absolute function
    div = diff / 4;
    return (mod+diff+div) % 7; // returns weekday 0 (sunday) to 6 (saturday)
}