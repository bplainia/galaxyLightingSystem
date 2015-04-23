#ifndef SENSORS_H
#define SENSORS_H

#include <shared.h>
#include <rtc.h>


// Defines
#define PINIO_POT_LEFTRIGHT TRISAbits.TRISA2
#define PINIO_POT_UPDOWN    TRISAbits.TRISA3
#define PINIO_LIGHTLEVEL    TRISGbits.TRISG0
#define PINIO_YEARLEVEL     TRISGbits.TRISG1
#define PINIO_DAYLEVEL1     TRISGbits.TRISG2
#define PINIO_DAYLEVEL2     TRISGbits.TRISG3
#define PINIO_TEMP          TRISAbits.TRISA0
#define PINIO_PIR           TRISCbits.TRISC6
#define PINIO_LIMIT_DOWN    TRISGbits.TRISG4
#define PINIO_LIMIT_UP      TRISFbits.TRISF2
#define PINIO_LIMIT_EAST    TRISFbits.TRISF6
#define PINIO_LIMIT_WEST    TRISFbits.TRISF7

#define OUTPUT 0
#define INPUT 1
#define DIGITAL 0
#define ANALOG 1

#define PINAD_POT_LEFTRIGHT ANCON1bits.ANSEL2
#define PINAD_POT_UPDOWN    ANCON1bits.ANSEL3
#define PINAD_LIGHTLEVEL    ANCON1bits.ANSEL7
#define PINAD_YEARLEVEL     ANCON2bits.ANSEL12
#define PINAD_DAYLEVEL1     ANCON2bits.ANSEL13
#define PINAD_DAYLEVEL2     ANCON2bits.ANSEL14
#define PINAD_TEMP          ANCON1bits.ANSEL0
#define PINAD_LIMIT_DOWN    ANCON2bits.ANSEL15
#define PINAD_LIMIT_UP      ANCON1bits.ANSEL6
#define PINAD_LIMIT_EAST    ANCON2bits.ANSEL10
#define PINAD_LIMIT_WEST    ANCON2bits.ANSEL11

#define PIN_PIR             LATCbits.LATC6

#define PIN_LIMIT_DOWN      LATGbits.LATG4
#define PIN_LIMIT_UP        LATFbits.LATF2
#define PIN_LIMIT_EAST      LATFbits.LATF6
#define PIN_LIMIT_WEST      LATFbits.LATF7

#define CHAN_POT_DAY    2   // 0b00010
#define CHAN_POT_YEAR   3   // 0b00011
#define CHAN_PHOTO_LEV  7   // 0b00111
#define CHAN_PHOTO_YEAR 12  // 0b01100
#define CHAN_PHOTO_EAST 13  // 0b01101
#define CHAN_PHOTO_WEST 14  // 0b01110
#define CHAN_TEMP       0   // 0b00000

#define POT_DAY 0
#define POT_YEAR 1

#define NO_MOVE 0
#define MOVE 1

// LED states
#define LED_OFF 0
#define LED_ON 1
#define LED_DIM 2

// light levels
#define CLOUDY 1400
#define DUSK 100

// motor and other directions
#define UP 1
#define DOWN 2
#define EAST 3
#define WEST 4
#define STOP 0
#define NOON 0

enum {PHOTO_LEV, PHOTO_YEAR, PHOTO_EAST, PHOTO_WEST};
enum {LIMIT_DOWN, LIMIT_UP, LIMIT_EAST, LIMIT_WEST};

//#define ALARM_FLAG PIR3bits.RTCCIF

// Variables
unsigned int pirTime = 0;
unsigned char pirLowTime = 0;

// Functions

void sensor_setup(void);
unsigned int photo_value(unsigned char, unsigned);
unsigned char temperature(void);
unsigned int angle(unsigned, unsigned);
unsigned pir(void);
unsigned char limits(void);
unsigned int ang_pos(unsigned, unsigned);

#endif