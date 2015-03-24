#include <shared.h>
#include <hid.h>
#include <menu.h>

/// Sets up the Human Interface Periphrials (keyboard, usb, and lcd) and their pins.
void hid_setup(void)
{
    // setup LCD pins: memory has already been started, so no need to setup
    TRISD &= 0b01110000;        // set the three pins to be outputs
    RPOR32_33bits.RPO32R = 0x9; // set RP32 to CCP5 for RED PWM
    RPOR34_35bits.RPO34R = 0x9; // set RP34 to CCP6 for GRN PWM
    RPOR36_37bits.RPO37R = 0x8; // set RP37 to CCP7 for BLU PWM

    
    // setup keypad port
    SPBRGH3         = 0x00; //! \todo  TODO: initialize baud rate to 9600
    SPBRG3          = 0x00;
    TXSTA3bits.SYNC = 0;
    RCSTA3bits.SPEN = 1;
    RC3IE           = 0; // Make sure the keypad is not causing interrupts right now.
    RCSTA3bits.CREN = 1; // Enable
    RPINR4_5bits.U3RXR   = 0x7; // set USART3 to RP28 for input of keypad

    // setup USB
    /// \todo TODO: Josh. Put your stuff to setup USB here.
}

/// Execute the Maintainence Mode State Machine. Please see the [State Machine Page](/statemachine.html#mmode_sm "Maintainence Mode State Machine").
void hid_loop(void) // execute hid functions; is called from the main loop
{
    // check pins and flags for changes for the mmode state machine

    // execute the state machine
    switch (status.mmode)
    {
        //case 0b00: // Off. Only check if something was plugged in or we got a byte from the keypad.
        case 0b01: // LCD Mode
            break;
        case 0b10: // USB Mode
            break;
        case 0b11: // Slave Mode
            break;
    }
}
void hid_execute(unsigned char command)  // execute a command (usb and lcd can call this)
{

}

// LCD functions
void lcd_begin(void)             // Welcome and setup screen for menu system
{
    if(status.mmode == 0) status.mmode = 1; // Entered Maintenence Mode (LCD and Communications are now on)
    else return;
}
void lcd_menu(unsigned char)     // execute a menu item.
{

}

/// display USB connected on lcd. Press the special key to disconnect.
void lcd_usb(unsigned char)      
{

}

/// display "exiting", then turn off the led, and exit maintainence mode
void lcd_end(void)               // exit out of menu and return to normal
{

    if(status.mmode == 1) status.mmode = 0; // Entered Maintenence Mode (LCD and Communications are now on)
    else return;
}

// Keypad functions

void key_isr(void)
{

}

// USB functions
void usb_isr(void)
{

}
void usb_insertion(void)
{

}
void usb_removal(void)
{
    //TODO: must be cleared in this order according to pic18 manual pg. 526
 
    /*UCONbits.SUSPND = 0;
    UCONbits.USBEN = 0;
   */  
}
void usb_begin(void)
{

}
void usb_oneStatus()
{

}
void usb_myStatus()
{

}
void usb_allStatus()
{

}
void usb_end(void)
{

}