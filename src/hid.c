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
        case 1: // LCD Mode
            break;
        case 2: // USB Mode
            break;
        case 3: // Slave Mode
            break;
    }
}
void hid_execute(unsigned char command)  // execute a command (usb and lcd can call this)
{
    if(command < 150)
    {
        // Execute a function from the menu
        (menue[command][0]).function(); /// \todo FIXME: how shall we get the second argument for the array?
    }
    else
    {
        (usbe[command-150]).function(); // 255-150 = 105 commands left for usb
    }
}

// LCD functions
void lcd_begin(void)             // Welcome and setup screen for menu system
{
    if(status.mmode < 1) status.mmode = 1; // Entered Maintenence Mode (LCD and Communications are now on)
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
    status.mmode = 0; // maintainence mode off
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
    if()
}
void usb_removal(void)
{
    //TODO: must be cleared in this order according to pic18 manual pg. 526
 
    UCONbits.SUSPND = 0;
    UCONbits.USBEN = 0; 
}
void usb_begin(void)        //Ben, Im Experimenting w/code researched online
{
   /* if (!HIDRxHandleBusy(lastReceived))
{
    lastReceived = HIDRxPacket(HID_EP,(BYTE*)&hid_out, HID_OUT_LEN);
    if ((hid_out[0]==0xEF)&&(hid_out[1]==0xEF)&&(hid_out[2]==0xEF))
        LED_ON;
    else
        LED_OFF;
}

//If the last transmision is complete
if(!HIDTxHandleBusy(lastTransmission))
{
    lastTransmission = HIDTxPacket(HID_EP, (BYTE*)&hid_in, HID_IN_LEN);
}

return;
*/

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