#ifndef MOVEMENT_H
#define MOVEMENT_H

// Varibles

unsigned int leftlimit;
unsigned int rightlimit;
unsigned int uplimit;
unsigned int downlimit;

// Defines

#define PINIO_MOVE_LEFTRIGHT 1 //TRISAbits.TRISA2
#define PINIO_MOVE_UPDOWN 1 //TRISAbits.TRISA3



// Functions

void movement_initialize(void);
void movement_test(void);
void daytime_move(void);
void dusk_moveback(void);
void seasonal_adjust(void);
void maintenance_move(unsigned char);

#endif