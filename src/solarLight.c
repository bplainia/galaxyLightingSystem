#include <solarLight.h>


void led_setup(void){

    // Define pin as output
    LED_IO = 0;
light = light^0b00000001;
    // setup pwm
}


void led_light(void){
    light = light^0b00000010;
//    if photo_value(1) < daylevel
//       if time_since_motion < 10
//            LED_OUT = 0;
//       else
//            LED_OUT = PWM
//    else
//       LED_OUT = 1;
    
}


void led_maintenance(unsigned char lightState){

    switch (lightState)
        case AUTO:
            led_light();
        break;
        case ON:
            LED_OUT = 0;
        break;
        case DIM:
            // LED_OUT = pwm
            light = light^0b00000100;
        break;
        case OFF:
            LED_OUT = 1;
        break;
        default:
            led_light();

}
