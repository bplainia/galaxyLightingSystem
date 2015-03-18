/* 
 * File:   rtc.h
 * Author: Benjaminp
 *
 * Created on February 6, 2015, 11:29 PM
 */

#ifndef RTC_H
#define	RTC_H

#define REFDAY 2 // Tuesday is the reference day for 2000-2099 which is all this rtc can do.

// frequencies for AMASK<3:0>
#define HALFSECOND 0
#define ONESECOND  1
#define TENSECOND  2
#define MINUTE     3
#define TENMINUTES 4
#define HOURLY     5
#define DAILY      6
#define WEEKLY     7
#define MONTHLY    8
#define YEARLY     9 // If you set this and it is february 29th, it will go only on leap years.

/// structure to save the date and time into a single variable in normal binary format (Not BCD!)
typedef struct
{
    unsigned char year;
    unsigned char month;
    unsigned char weekday; // this one is only set in the get function.
    unsigned char weekofyear;
    unsigned char day;
    unsigned char hour;
    unsigned char minute;
    unsigned char second;
}datetime;

typedef struct
{
    unsigned char month;
    unsigned char day; // will be used for either day or day of week
    unsigned char hour;
    unsigned char minute;
    unsigned char second;
    unsigned char times;
    unsigned char frequency;
    unsigned char repeat; // 0=none, 1=number of `times`, 2=infinite
}alarmtime;

// Functions

void rtc_setup(void);
unsigned rtc_set(datetime);
unsigned rtc_get(datetime*);

unsigned alarm_set(alarmtime);
unsigned alarm_enable();
unsigned alarm_disable();

static unsigned char char2bcd(unsigned char);
static unsigned char bcd2char(unsigned char);
static unsigned char weekday(unsigned char, unsigned char, unsigned char);

#endif	/* RTC_H */

