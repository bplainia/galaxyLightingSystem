#include <light.h>

/*! \brief setup for light
 *
 * --> Test value (global var) used to show if function was entered
 */
void led_setup(void){

    // Define pin as output
    LED_IO = 0;
    light = light ^ 0b00000001;
}

/*! \brief operate the light as normal
 *
 * Outputs: change state on light pin to control the light
 *
 * --> Test value (global var) used to show if function was entered
 */
void led_auto(void){
    light = light ^ 0b00000010;
//    if photo_value(1) < daylevel
//       if time_since_motion < 10
//            LED_OUT = 0;
//       else
//            LED_OUT = PWM
//    else
//       LED_OUT = 1;
    
}

/*! \brief change the light as directed by maintenance panel
 *
 * Outputs: change state on light pin to control the light
 *
 * --> Test value (global var) used to show if function was entered
 */
void led_maintenance(unsigned char lightState)
{
    switch (lightState)
    {
        case AUTO:
            led_auto();
        break;
        case ON:
            LED_OUT = 0;
        break;
        case DIM:
            // LED_OUT = pwm
            light = light ^ 0b00000100;
        break;
        case OFF:
            LED_OUT = 1;
        break;
        default:
            led_auto();
    }

}
