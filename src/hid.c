#include <shared.h>
#include <hid.h>

// HID (keyboard, usb, and lcd) Functions
void hid_setup(void)
{
    // setup LCD pins: memory has already been started, so no need to setup
    RPOR32_33bits.RPO32R = 0x9; // set RP32 to CCP5 for RED PWM
    RPOR34_35bits.RPO34R = 0x9; // set RP34 to CCP6 for GRN PWM
    RPOR36_37bits.RPO37R = 0x8; // set RP37 to CCP7 for BLU PWM
    // TODO: Check what to do with pins before setting to PPS-Lite functions

    //FIXME: Is Luke seting up tmr2 for pwm?
    
    // setup keypad port
    SPBRGH3         = 0x00; // TODO: initialize baud rate to 9600
    SPBRG3          = 0x00;
    RCSTA3bits.SYNC = 0;
    RCSTA3bits.SPEN = 1;
    RC3IE           = 0; // Make sure the keypad is not causing interrupts right now.
    RCSTA3bits.CREN = 1; // Enable
    // TODO: CHeck
    RPOR28_29bits.RPO28R = 0x2; // set RP28 to USART3_RX

    // setup USB
    
}
void hid_loop(void) // execute hid functions; is called from the main loop
{

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