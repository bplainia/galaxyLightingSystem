
//#include <xc8.h>
#include <sensors.h>


void sensor_start(void){

    // Define sensor pins as inputs
    PINIO_POT_LEFTRIGHT = 1;
    PINIO_POT_UPDOWN = 1;
    PINIO_LIGHTLEVEL = 1;
    PINIO_YEARLEVEL = 1;
    PINIO_DAYLEVEL1 = 1;
    PINIO_DAYLEVEL2 = 1;
    PINIO_TEMP = 1;
    PINIO_PIR = 1;
    PINIO_LIMIT_DOWN = 1;
    PINIO_LIMIT_UP = 1;
    PINIO_LIMIT_EAST = 1;
    PINIO_LIMIT_WEST = 1;

    // Define inputs as digital (0) or analog (1)
    PINAD_POT_LEFTRIGHT = 1;
    PINAD_POT_UPDOWN = 1;
    PINAD_LIGHTLEVEL = 1;
    PINAD_YEARLEVEL = 1;
    PINAD_DAYLEVEL1 = 1;
    PINAD_DAYLEVEL2 = 1;
    PINAD_TEMP = 1;
    PINAD_LIMIT_DOWN = 0;
    PINAD_LIMIT_UP = 0;
    PINAD_LIMIT_EAST = 0;
    PINAD_LIMIT_WEST = 0;

    // Setup ADC

    // Setup inturrupts

}

unsigned short photo_value(unsigned char photonum){
    switch (photonum)
            case 1:
                return 0; //adc off PIN_LIGHTLEVEL
            break;
            case 2:
                return 0; //adc off PIN_DAYLEVEL1
            break;
            case 3:
                return 0; //adc off PIN_DAYLEVEL2
            break;
            case 4:
                return 0; //adc off PIN_YEARLEVEL
            break;
            default:
                return 0;
}

unsigned short temperature(void){
    return 0; //adc off PIN_TEMP
}

unsigned short rotational_postion(unsigned char potnum){
    switch (potnum)
            case 1:
                return 0; //adc off PIN_DAY_POT
            break;
            case 2:
                return 0; //adc off PIN_YEAR_POT
            break;
            default:
                return 0;
}

void pir_enable(unsigned char pironoff){
    if (pironoff == 1)
        ; // turn on motion sensor inturrupt pin = PIN_PIR
    else
        ; //turn off motion sensor inturrupt
}

