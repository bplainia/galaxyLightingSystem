#include <shared.h>
#include <hid.h>

/// Sets up the Human Interface Periphrials (keyboard, usb, and lcd) and their pins.
void hid_setup(void)
{
    // setup LCD pins: memory has already been started, so no need to setup
    TRISD &= 0b01110000;        // set the three pins to be outputs
    RPOR32_33bits.RPO32R = 0x9; // set RP32 to CCP5 for RED PWM
    RPOR34_35bits.RPO34R = 0x9; // set RP34 to CCP6 for GRN PWM
    RPOR36_37bits.RPO37R = 0x8; // set RP37 to CCP7 for BLU PWM

    
    // setup keypad port
    SPBRGH3         = 0x00; // TODO: initialize baud rate to 9600
    SPBRG3          = 0x00;
    TXSTA3bits.SYNC = 0;
    RCSTA3bits.SPEN = 1;
    RC3IE           = 0; // Make sure the keypad is not causing interrupts right now.
    RCSTA3bits.CREN = 1; // Enable
    RPINR4_5bits.U3RXR   = 0x7; // set USART3 to RP28 for input of keypad

    // setup USB
    
}

/// Execute the Maintainence Mode State Machine. Please see the [State Machine Page](/statemachine.html#mmode_sm "Maintainence Mode State Machine").
void hid_loop(void) // execute hid functions; is called from the main loop
{
    // check pins and flags for changes for the mmode state machine

    // execute the state machine
    switch (status.mmode)
    {
        case 0: // Off. Only check if something was plugged in or we got a byte from the keypad.
            break;
        case 1:
            break;
        case 2:
            break;
        case 3: // ERROR State
            break;
    }
}
void hid_execute(unsigned char)  // execute a command (usb and lcd can call this)
{

}

// LCD functions
void lcd_begin(void)             // Welcome and setup screen for menu system
{

    status.MMODE = 1; // Entered Maintenence Mode (LCD and Communications are now on)
}
void lcd_menu(unsigned char)     // display a menu item
{

}
void lcd_execute(unsigned char, unsigned char)  // execute a menu item
{

}
void lcd_usb(unsigned char)      // display USB connected. Press ? to disconnect.
{

}
void lcd_end(void)               // exit out of menu and return to normal
{

}

// Keypad functions
void key_isr(void)
{

}
void key_insertion(void)
{

}
void key_removal(void)
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