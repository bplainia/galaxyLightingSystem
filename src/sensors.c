


void sensor_start(void){

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

