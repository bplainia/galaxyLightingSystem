#ifndef POWERMAN_H
#define POWERMAN_H

#include <shared.h>

unsigned battin;

void power_setup(void);
void power_loop(void);      //Check for main grid power if battery is getting low.
void power_switch(unsigned char);
void bulb_detect();
void batt_live(unsigned char);

//definitions


#endif