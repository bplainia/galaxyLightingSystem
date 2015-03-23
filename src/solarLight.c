/*! \file
 * \brief This is the main file for the project (project name is "solarLight").
 *
 * It contains the main, setup, and loop functions, which are setup similar to an arduino.
 *
 * Created by benjamin Plain
*/

// PIC18F67J94 Configuration Bit Settings

// 'C' source line config statements

#include <xc.h>

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

// CONFIG1L
#pragma config STVREN = OFF     // Stack Overflow/Underflow Reset (Disabled)
#pragma config XINST = OFF       // Extended Instruction Set (Enabled)

// CONFIG1H
#pragma config BOREN = ON       // Brown-Out Reset Enable (Controlled with SBOREN bit, disabled in Deep Sleep)
#pragma config BORV = 0         // Brown-out Reset Voltage (2.0V)
#pragma config CP0 = OFF        // Code Protect (Program memory is not code-protected)

// CONFIG2L
#pragma config FOSC = PRIPLL    // Oscillator (Primary Oscillator with PLL module (XTPLL,HSPLL,ECPLL))
#pragma config SOSCSEL = LOW    // T1OSC/SOSC Power Selection Bits (Low Power T1OSC/SOSC circuit selected)
#pragma config CLKOEN = OFF     // Clock Out Enable Bit (CLKO output disabled on the RA6 pin)
#pragma config IESO = ON        // Internal External Oscillator Switch Over Mode (Enabled)

// CONFIG2H
#pragma config PLLDIV = DIV4   // 96MHz PLL

// CONFIG3L
#pragma config POSCMD = HS      // Primary Oscillator Select (HS oscillator mode selected(10 MHz - 40 MHz))
#pragma config FSCM = CSECME    // Clock Switching and Monitor Selection Configuration bits (Clock switching is enabled, fail safe clock monitor is enabled)

// CONFIG3H

// CONFIG4L
#pragma config WPFP = WPFP255   // Write/Erase Protect Page Start/End Boundary (Write Protect Program Flash Page 255)

// CONFIG4H
#pragma config WPDIS = WPDIS    // Segment Write Protection Disable (Disabled)
#pragma config WPEND = WPENDMEM // Segment Write Protection End Page Select (Write Protect from WPFP to the last page of memory)
#pragma config WPCFG = WPCFGDIS // Write Protect Configuration Page Select (Disabled)

// CONFIG5L
#pragma config T5GSEL = T5G     // TMR5 Gate Select bit (TMR5 Gate is driven by the T5G input)
#pragma config CINASEL = DEFAULT// CxINA Gate Select bit (C1INA and C3INA are on their default pin locations)

// CONFIG5H
#pragma config IOL1WAY = OFF    // IOLOCK One-Way Set Enable bit (the IOLOCK bit can be set and cleared using the unlock sequence)
#pragma config LS48MHZ = SYSX2  // USB Low Speed Clock Select bit (Divide-by-2 (System clock must be 12 MHz))
#pragma config MSSPMSK2 = MSK7  // MSSP2 7-Bit Address Masking Mode Enable bit (7 Bit address masking mode)
#pragma config MSSPMSK1 = MSK7  // MSSP1 7-Bit Address Masking Mode Enable bit (7 Bit address masking mode)

// CONFIG6L
#pragma config WDTWIN = PS25_0  // Watch Dog Timer Window (Watch Dog Timer Window Width is 25 percent)
#pragma config WDTCLK = SOSC    // Watch Dog Timer Clock Source (Always use SOSC)
#pragma config WDTPS = 32768    // Watchdog Timer Postscale (1:32768)

// CONFIG6H
#pragma config WDTEN = OFF      // Watchdog Timer Enable (WDT disabled in hardware; SWDTEN bit disabled)
#pragma config WINDIS = WDTSTD  // Windowed Watchdog Timer Disable (Standard WDT selected; windowed WDT disabled)
#pragma config WPSA = 128       // WDT Prescaler (WDT prescaler ratio of 1:128)

// CONFIG7L
#pragma config RETEN = ON       // Retention Voltage Regulator Control Enable (Retention controlled by SRETEN during Sleep)
#pragma config VBTBOR = ON      // VBAT BOR Enable (VBAT BOR is enabled)
#pragma config DSBOREN = ON     // Deep Sleep BOR Enable (BOR enabled in Deep Sleep)
#pragma config DSBITEN = ON     // DSEN Bit Enable bit (Deep Sleep is controlled by the register bit DSEN)

// CONFIG7H

// CONFIG8L
#pragma config DSWDTPS = DSWDTPS1A ///< Deep Sleep Watchdog Timer Postscale Select (1:2147483648 (19.2 Hours))

// CONFIG8H
#pragma config DSWDTEN = ON     ///< Deep Sleep Watchdog Timer Enable (DSWDT Enabled)
#pragma config DSWDTOSC = SOSC  ///< DSWDT Reference Clock Select (DSWDT uses SOSC as reference clock)


#include <solarLight.h>

/// Run setup and main loops.
void main(void)
{
    // A very super-simple main function by Ben. Makes this coding similar
    // to programming an arduino. Ain't that nice? So don't put anything
    // else here now. This is a finished function.
    INTCONbits.GIEH = 1; // enable interrupts
    INTCONbits.GIEL = 1;
    INTCONbits.PEIE = 1; // enable priority interrupts
    
    setup();             // setup everything

    INTCONbits.GIEH = 1; // enable interrupts
    INTCONbits.GIEL = 1;
    while(1) loop();     // execute loop forever
}

/// Run all the setup routines. Initialize stuff here without any interrupts.
void setup()
{
    // Variables
    unsigned char memStatus;

    i2c_setup();  // Initialize I2C
    //! \todo  TODO: Check to see if the chip started after a POR, BOR, or is from VBATT
    memStatus = mem_check(); // checks where it started up from, and if memory is ok
    pwm_setup();
    adc_setup();
    move_setup();
    led_setup();
    hid_setup();
    rtc_setup();
    sensor_setup();

    limit_test();           // Calibration for position pots
}

/*! /brief The MAIN loop that executes EVERYTHING!
 *
 * No inputs or outputs besides what goes in and out of the pins.
 */
void loop()
{


    // Put things that you need to process here. Dont' spend too much time
    // in your process. Others want to do stuff too...
    adc_updateAll(); // Update all the ADC buffers every loop
    switch(status.state)
    {
        case 1: // Daytime Mode
            daytime_move();
            led(OFF);
            break;
        case 2: // Nighttime mode
            led(ON);
            pir();
            break;
        case 3: // ERROR mode
            // light off
            ;
    }
    hid_loop(); // Maintainence Mode State Machine.

    if(status.state == 1) // If daytime
    {
        // do a full sleep
    }
    else if(status.state == 2)
    {
        // do an idle "sleep"
    }



}
/*! \brief Low Priority Interrupt Service Routine
 * 
 * Items that are dealt with in this ISR:
 *  - UART 1 TX/RX Interrupts
 *
 * NOTE from manual: Interrupt flag bits are set when an interrupt condition occurs regardless
 * of the state of its corresponding enable bit or the Global Interrupt Enable bit. User
 * software should ensure the appropriate interrupt flag bits are clear prior to enabling
 * an interrupt. This feature allows for software polling.
 */
void interrupt low_priority isr_low()
{
    if(TX1IE && TX1IF) // Comm Transmit Ready and able too
    {
        TX1IF = 0; // Reset TX1 Flag
        if(COMSTAT.TOKEN && (txPtrOut != txPtrIn))
        {
            TXREG1 = txBuff[txPtrOut++];
        }
        //! \todo  TODO: determine where the token and the transmit control will be.
    }
    if(RC1IE && RC1IF)
    {
        // If in slave mode, check the address.
    }
}

/*! \brief High Priority Interrupt Service Routine
 *
 * Items that are dealt with in this ISR:
 *  - none yet
 *
 * Make sure to enable priority interrupts before using this routine.
 */
void interrupt high_priority isr_high()
{
    ;
}

void led_setup(void)
{
    LED_PIN = 0;
}

void led(unsigned char state)
{
    switch(state)
    {
        case OFF:
            LED_OUT = LED_OFF;
            break;
        case ON:
            LED_OUT = LED_ON;
            break;
        case DIM:
            //pwm_set(1 2 /*channel duty*/);
            break;
        default:
            ;
    }
}