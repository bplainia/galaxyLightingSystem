/* 
 * File:   shared.h
 * Author: 118506
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
    unsigned    RTCINIT : 1;
    unsigned    MMODE   : 2;
    unsigned    STATE   : 3;
    unsigned            : 2;
}status;

#endif	/* SHARED_H */

