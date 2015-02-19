/* 
 * File:   shared.h
 * Author: Benjamin Plain
 *
 * Created on February 5, 2015, 7:53 PM
 */

#ifndef SHARED_H
#define	SHARED_H

#define VERSION1 0
#define VERSION2 0
#define COMPANY "Patience, Inc."

#include <xc.h>

// Global Variables (beware race conditions)
struct {
    unsigned    RTCINIT : 1; // has the rtc been initialized
    unsigned    MMODE   : 1; // maintenance mode
    unsigned    STATE   : 3; // night/day, ?, ? 
    unsigned            : 3;
}status1;

//enum {FIRSTRUN, BATTFAIL, VDDFAIL, MEMFAIL, RUNNING} powerStatus;

#endif	/* SHARED_H */

