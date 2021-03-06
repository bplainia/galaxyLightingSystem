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

// #pragma config statem3ents should precede project file includes.
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
    
    // Setup Oscillator
    OSCCON2bits.SOSCGO = 1;
    OSCCON4 = 0b01100000;

    INTCONbits.GIEH = 0; // disable all interrupts
    INTCONbits.GIEL = 0; // enable periphrial interrupts (disabled by GIE currently)
    RCONbits.IPEN = 1; // Priority interrupts
    
    setup();             // setup everything

    INTCONbits.GIEH = 1; // enable all interrupts
    INTCONbits.GIEL = 1;
    
    while(1) loop();     // execute loop forever
}

/// Run all the setup routines. Initialize stuff here without any interrupts.
void setup()
{
    // Variables
    static unsigned char memStatus;

    power_setup();
    i2c_setup();  // Initialize I2C
    //! \todo  TODO: Check to see if the chip started after a POR, BOR, or is from VBATT
    memStatus = mem_check(); // checks where it started up from, and if memory is ok
//    if(RCON3bits.VBAT == 0) // we have not returned from VBATT (first time startup essentially)
//    {
        rtc_setup();
//    }
    pwm_setup(); // initializes timers
    hid_setup();
    adc_setup();
    move_setup();
    sensor_setup();
    if(memStatus & 0b00001111) // if we returned from ?, don't waste time doing the limit test.
    {
      //  limit_test();           // Calibration for position pots
    }
    lcd_display(0,NULL); // clear the screen
    LATAbits.LATA4 = 1;
}

/*! /brief The MAIN loop that executes EVERYTHING!
 *
 * No inputs or outputs besides what goes in and out of the pins.
 */
void loop()
{


    // Put things that you need to process here. Dont' spend too much time
    // in your process. Others want to do stuff too...
                switch(status.state)
                {
                    case 1: // Daytime Mode
                        //daytime_move();
                        led(OFF);
                        break;
                    case 2: // Nighttime mode
                        pir();
                        break;
                    case 3: // ERROR mode
                        // light off
                        ;
                }
    // Dusk event
    //  if(photo_value(1, PHOTO_LEV) < DUSK) // && time = after 6ish
    //  {
    //      
    //      dusk_moveback(limit_pot_east);
    //      led(DIM);
    //      status.state = 2;
    //  }

    hid_loop(); // Maintainence Mode State Machine.
    power_loop();
    pir();

    // TODO: Sleep when we can to save power.
    if(status.state == 1 && status.mmode) // If daytime and not in maintainence mode
    {
        ;// do a full sleep
    }
    else if(status.state == 2)
    {
        ;// do an idle "sleep"
    }



}
/*! \brief Low Priority Interrupt Service Routine
 * 
 * Items that are dealt with in this ISR:
 *  - UART 1 RX Interrupt
 *
 * NOTE from manual: Interrupt flag bits are set when an interrupt condition occurs regardless
 * of the state of its corresponding enable bit or the Global Interrupt Enable bit. User
 * software should ensure the appropriate interrupt flag bits are clear prior to enabling
 * an interrupt. This feature allows for software polling.
 */
void interrupt low_priority isr()
{
    volatile unsigned char temp, temp2;
    if(RC1IE && RC1IF)
    {
        RC1IF = 0; // reset the flag
        temp2 = RCSTA1bits.RX9D;
        temp = RCREG1;
        
        // If we recieve master active, comm state machine goes to slave mode
        if(COMSTAT.STATE == 0b000) // there is no master and I am not the master
        {
            if(temp2 == 1 && temp == 254) // if was master active address...
            {
                if(masterAddr!=250) // check to see if master address is set.
                {
                    COMSTAT.TERROR = 1; // token error if it was already
                }
            }
            else 
            {
                masterAddr = temp;
                COMSTAT.STATE = 0b001;
                status.mmode = 0b10; // slave mode
                RCSTA1bits.ADDEN = 1; // back to address mode
            }
        }
        else if(COMSTAT.STATE == 0b001) // Slave is idle
        {
            if(temp == 253) // master now inactive. Bye Maintainence Man.
            {
                    COMSTAT.STATE = 0b000; // inactive bus
                    masterAddr = 250; // reset to global address
                    RCSTA1bits.ADDEN = 1;
            }
            else if(temp == myAddr)
            {
                COMSTAT.STATE = 0b010; // now listening since it is my address
                // no address mode for this since we are waiting for data.
            }
            else if(temp == 250)
            {
                COMSTAT.STATE = 0b111;
                // global address. waiting for data...
            }
        }
        else if(COMSTAT.STATE == 0b010 || COMSTAT.STATE == 0b111) // Slave is listening state
        {
            if(temp2 == 0)
            {
                rxBuff[rxPtrIn++] = temp;
                if(rxPtrIn >= RXBUFFSIZE) rxPtrIn = 0;
                if(rxPtrIn == rxPtrOut) COMSTAT.RXOVER = 1; // overflow
            }
            else if(temp2 == 1) // My turn to execute!
            {
                if(temp == 252) // if slave active byte
                {
                    COMSTAT.STATE = 0b011; // slave talking now
                }
                else
                {
                    COMSTAT.TERROR = 1;
                    comm_flush(); // clear the command buffer
                    COMSTAT.STATE  = 0b001; // back to just listening
                }
            }
        }
    } // end of uart1 recieve

    else if(RC3IE && RC3IF) // uart3 recieve (keypad entry)
    {
        keypad_push(RCREG3);
        RC3IF = 0; // reset the flag
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
    if(TMR1IE && TMR1IF) // This is the delay counter
    {
        TMR1IF = 0;
        TMR1H  = 0xF7;
        TMR1L  = 0xFF;       // set to increment `time` every 1/8th of a second
        ++time;
        ++delayTime;
    }
    if(TMR3IE && TMR3IF) // This is the delay counter
    {
        TMR3IF = 0;
        TMR3H  = 0xDF;
        TMR3L  = 0xFF;       // set to increment `time` every second
        ++pirTime;
        ++pirLowTime;
    }
}

