#ifndef SENSORS_H
#define SENSORS_H

// Varibles

// Defines

#define PIN_POT_LEFTRIGHT TRISAbits.TRISA2
#define PIN_POT_UPDOWN TRISAbits.TRISA3
#define PIN_LIGHTLEVEL TRISGbits.TRISG0
#define PIN_YEARLEVEL TRISGbits.TRISG1
#define PIN_DAYLEVEL1 TRISGbits.TRISG2
#define PIN_DAYLEVEL2 TRISGbits.TRISG3
#define PIN_TEMP TRISAbits.TRISA0
#define PIN_PIR TRISCbits.TRISC6
#define PIN_LIMIT_DOWN TRISGbits.TRISG4
#define PIN_LIMIT_UP TRISFbits.TRISF2
#define PIN_LIMIT_EAST TRISFbits.TRISF6
#define PIN_LIMIT_WEST TRISFbits.TRISF7

// Functions

void sensor_start(void);
unsigned short photo_value(unsigned char);
unsigned short temperature(void);
unsigned short rotational_postion(unsigned char);
void pir_enable(unsigned char);

#endif