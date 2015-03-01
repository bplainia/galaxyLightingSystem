/* 
 * File:   rtc.h
 * Author: Benjaminp
 *
 * Created on February 6, 2015, 11:29 PM
 */

#ifndef RTC_H
#define	RTC_H

#define REFDAY 2 // Tuesday is the reference day for 2000-2099 which is all this rtc can do.

// Functions

void rtc_start(void);
unsigned rtc_set(unsigned char, unsigned char, unsigned char, unsigned char,
        unsigned char, unsigned char);


#endif	/* RTC_H */

