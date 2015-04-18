#ifndef POWERMAN_H
#define POWERMAN_H

#include <shared.h>

unsigned battin;
void power_setup();
void power_loop();      //Check for main grid power if battery is getting low.
void power_switch();
void bulb_detect();


#endif