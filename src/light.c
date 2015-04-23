// Author: Luke Williams
// Light Control code (includes motion detector code)

#include <light.h>
#include <sensors.h>

/*! \brief change the light as directed by maintenance panel
 *
 * Outputs: change state on light pin to control the light
 */
void led_maintenance(unsigned char lightState)
{
    switch (lightState)
    {
//        case AUTO:
//            led_auto();
//        break;
        case ON:
            led(ON);
        break;
        case DIM:
            led(DIM);
        break;
        case OFF:
            led(OFF);
//        break;
//        default:
//            led_auto();
    }
    return;
}

// Setup pin for LED as output
void led_setup(void)
{
    LED_PIN = OUTPUT;
    pwm_set(LED_CHAN, LED_DUTY_DIM);
}

// Function to modify the LED to turn either ON, OFF, or DIM
void led(unsigned char state)
{
    switch(state)
    {
        case OFF:
            pwm_set(LED_CHAN, LED_DUTY_OFF);
            break;
        case ON:
            pwm_set(LED_CHAN, LED_DUTY_ON);
            break;
        case DIM:
            pwm_set(LED_CHAN, setting_lightDim);
            break;
        default:
            ;
    }
}

//for the maintenance error alerts
void bulb_status(unsigned char none)
{
    //make a variable... then put it in header file. 
}