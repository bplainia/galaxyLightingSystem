#include <solarLight.h>

/*! \brief setup for light
 *
 * --> Test value (global var) used to show if function was entered
 */
void led_setup(void){

    // Define pin as output
    LED_IO = 0;
    light = light ^ 0b00000001;
}

/// Run all the setup routines
void setup() // Initialize stuff here without any interruption
{
    // Variables
    unsigned char memStatus;

    i2c_setup();  // Initialize I2C
    // TODO: Check to see if the chip started after a POR, BOR, or is from VBATT
    memStatus = mem_check(); // checks where it started up from, and if memory is ok
    hid_setup();
    rtc_setup();
}

/*! \brief change the light as directed by maintenance panel
 *
 * Outputs: change state on light pin to control the light
 *
 * --> Test value (global var) used to show if function was entered
 */
void led_maintenance(unsigned char lightState){

    switch (lightState)
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
// NOTE from manual: Interrupt flag bits are set when an interrupt condition occurs regardless
// of the state of its corresponding enable bit or the Global Interrupt Enable bit. User
// software should ensure the appropriate interrupt flag bits are clear prior to enabling
// an interrupt. This feature allows for software polling.

void interrupt high_priority isr_high()
{
    continue;
}