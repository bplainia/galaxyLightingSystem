#ifndef SOLARLIGHT_H
#define SOLARLIGHT_H

// Include all the headers
#include <shared.h>
#include <communication.h>
#include <eeprom.h>
#include <hid.h>
#include <lcd.h>
#include <movement.h>
#include <powerman.h>
#include <rtc.h>
#include <sensors.h>
#include <usb/usb_device_cdc.h>

#define LED_PIN   TRISCbits.TRISC7
#define LED_OUT   PORTCbits.RC7

#define LED_CHAN 7
#define LED_DUTY_OFF 100
#define LED_DUTY_DIM 14
#define LED_DUTY_ON 0


// Functions
// void main() is in the associated C file
void setup(void); // setup things without any interrupts
void loop(void);  // do this stuff forever. Interrupts enabled.
void interrupt low_priority isr_low(void);
void interrupt high_priority isr_high(void);
void led_setup(void);
void led(unsigned char);



#endif