#ifndef SENSORS_H
#define SENSORS_H

// Varibles

// Defines

#define PINIO_POT_LEFTRIGHT TRISAbits.TRISA2
#define PINIO_POT_UPDOWN TRISAbits.TRISA3
#define PINIO_LIGHTLEVEL TRISGbits.TRISG0
#define PINIO_YEARLEVEL TRISGbits.TRISG1
#define PINIO_DAYLEVEL1 TRISGbits.TRISG2
#define PINIO_DAYLEVEL2 TRISGbits.TRISG3
#define PINIO_TEMP TRISAbits.TRISA0
#define PINIO_PIR TRISCbits.TRISC6
#define PINIO_LIMIT_DOWN TRISGbits.TRISG4
#define PINIO_LIMIT_UP TRISFbits.TRISF2
#define PINIO_LIMIT_EAST TRISFbits.TRISF6
#define PINIO_LIMIT_WEST TRISFbits.TRISF7

#define PINAD_POT_LEFTRIGHT ANCON1bits.ANSEL2
#define PINAD_POT_UPDOWN ANCON1bits.ANSEL3
#define PINAD_LIGHTLEVEL ANCON1bits.ANSEL7
#define PINAD_YEARLEVEL ANCON2bits.ANSEL12
#define PINAD_DAYLEVEL1 ANCON2bits.ANSEL13
#define PINAD_DAYLEVEL2 ANCON2bits.ANSEL14
#define PINAD_TEMP ANCON1bits.ANSEL0
#define PINAD_LIMIT_DOWN ANCON2bits.ANSEL15
#define PINAD_LIMIT_UP ANCON1bits.ANSEL6
#define PINAD_LIMIT_EAST ANCON2bits.ANSEL10
#define PINAD_LIMIT_WEST ANCON2bits.ANSEL11


// Functions

void sensor_start(void);
unsigned short photo_value(unsigned char);
unsigned short temperature(void);
unsigned short rotational_postion(unsigned char);
void pir_enable(unsigned char);

#endif