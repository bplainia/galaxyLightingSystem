#include <shared.h>
#include <hid.h>
#include <lcd.h>

/// Sets up the Human Interface Periphrials (keyboard, usb, and lcd) and their pins.
void hid_setup(void)
{
    // setup keypad port
    SPBRGH3         = 0x00; //! \todo  TODO: initialize baud rate to 9600
    SPBRG3          = 0x00;
    TXSTA3bits.SYNC = 0;
    RCSTA3bits.SPEN = 1;
    RC3IE           = 0; // Make sure the keypad is not causing interrupts right now.
    RCSTA3bits.CREN = 1; // Enable
    RPINR4_5bits.U3RXR   = 0x7; // set USART3 to RP28 for input of keypad

    // Setup LCD
    lcd_setup();
    
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