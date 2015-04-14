#ifndef MOVEMENT_H
#define MOVEMENT_H

// Headers
#include <shared.h>
#include <sensors.h>

// Defines

#define PINIO_MOVE_LEFTRIGHT_1 TRISBbits.TRISB1
#define PINIO_MOVE_LEFTRIGHT_2 TRISBbits.TRISB2
#define PINIO_MOVE_LEFTRIGHT_3 TRISBbits.TRISB3
#define PINIO_MOVE_LEFTRIGHT_4 TRISBbits.TRISB4
#define PINIO_MOVE_UPDOWN_1 TRISDbits.TRISD4
#define PINIO_MOVE_UPDOWN_2 TRISDbits.TRISD5
#define PINIO_MOVE_UPDOWN_3 TRISDbits.TRISD6
#define PINIO_MOVE_UPDOWN_4 TRISDbits.TRISD7

#define PIN_MOVE_UP_1 PORTDbits.PORTD7
#define PIN_MOVE_UP_2 PORTDbits.PORTD4
#define PIN_MOVE_DOWN_1 PORTDbits.PORTD6
#define PIN_MOVE_DOWN_2 PORTDbits.PORTD5

#define PIN_MOVE_EAST_1 PORTBbits.PORTB3
#define PIN_MOVE_EAST_2 PORTBbits.PORTB4
#define PIN_MOVE_WEST_1 PORTBbits.PORTB2
#define PIN_MOVE_WEST_2 PORTBbits.PORTB1

#define ERR 8


/// varibles for debug testing, will have bits toggled depending on function entered
unsigned char move = 0;

// Functions

void move_setup(void);
void limit_test(void);
void daytime_move(void);
void dusk_moveback(unsigned int);
void season_adjust(void);
void maint_move(unsigned char);
void motor_move(char);
void day_pos_move(unsigned int);
void year_pos_move(unsigned int);
unsigned int abs(unsigned int);

#endif