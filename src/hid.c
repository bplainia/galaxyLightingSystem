#include <shared.h>
#include <hid.h>
#include <lcd.h>
#include <stdlib.h>

#include <communication.h>

static struct fifo *keypadIn, *keypadOut;
static unsigned char keypadNum;

/// Sets up the Human Interface Periphrials (keyboard, usb, and lcd) and their pins.
void hid_setup(void)
{
    // setup keypad
    SPBRGH3         = 0x00; //! \todo  TODO: initialize baud rate to 9600
    SPBRG3          = 0x00;
    TXSTA3bits.SYNC = 0;
    RCSTA3bits.SPEN = 1;
    RC3IE           = 0; // Make sure the keypad is not causing interrupts right now.
    RC3IP           = 0; // Low priority interrupt for the keypad
    RCSTA3bits.CREN = 1; // Enable
    RPINR4_5bits.U3RXR   = 0x7; // set USART3 to RP28 for input of keypad
    keypadIn = keypadOut = (struct fifo *) NULL; // initialize the keypad pointers

    // Setup LCD
    lcd_setup();
    
    // setup USB
    /// \todo TODO: Josh. Put your stuff to setup USB here.
}

/// Execute the Maintainence Mode State Machine. Please see the [State Machine Page](/statemachine.html#mmode_sm "Maintainence Mode State Machine").
void hid_loop(void) // execute hid functions; is called from the main loop
{
    unsigned char temp;
    static bit newScreen; // if 1, update the LCD
    static menuFunct *lastFunct = NULL;
    // check pins and flags for changes for the mmode state machine

    // execute the state machine
    switch (status.mmode)
    {
        case 0b00: // Off. Only check if something was plugged in or we got a byte from the keypad.
            if((COMSTAT.STATE == 0) && (keypad_pull() == ENTERKEY)) // enter was pressed. now enter password...
            {
                lcd_begin(); // returns true if password was wrong or function was canceled
            }
            else if((COMSTAT.STATE > 0b100)||(COMSTAT.STATE == 0b111))
            break;
        case 0b01: // LCD Mode
            if(COMSTAT.STATE == 0b100 && curFunct == NULL)
            {
                temp = keypad_pull();
                if(temp == ENTERKEY) // enter was pressed - execute menu entry
                {
                    menu_enter(0);
                    newScreen = 1;
                }
                else if(temp == UPKEY)
                {
                    menu_next(0);
                    newScreen = 1;
                }
                else if(temp == DOWNKEY)
                {
                    menu_prev(0);
                    newScreen = 1;
                }
                else if(temp == CANCEL || temp == SECONDKEY)
                {
                    menu_up(0);
                    newScreen = 1;
                }
            }
            //else if(COMSTAT.STATE == 0b101 && COMSTAT.STATE == 0b110)
            // cannot do anything while it is in these two states
            // we are transmitting and recieving data.

            if(curFunct != lastFunct && curFunct == NULL) // we just came back from a menu function so we need to update.
            {
                lastFunct = NULL;
                newScreen = 1;
            }

            if(newScreen)
            {
                menu_display();
                newScreen = 0;
            }
            break;
        case 0b10: // USB Mode
            // TODO: USB UNIMPLEMENTED
            break;
        case 0b11: // Slave Mode
            break;
    }
}

// LCD functions
unsigned lcd_begin(void)             // Welcome and setup screen for menu system
{
    unsigned char password[] = {5,3,1}; // hidden deep in the code is the password
    unsigned char i = 0;
    unsigned char command;

    if(status.mmode == 0) status.mmode = 1; // Entered Maintenence Mode (LCD and Communications are now on)
    else return; // already in master mode
    COMSTAT.STATE = 0b100; // I am now a lonely idling master
    lcd_background(255,255,255);
    lcd_display("Welcome! Please     ", "enter passkey:      ");
    while(i<3)
    {
        while((command = keypad_pull()) == 0xFF); /// \todo FIXME: See if you can try to not have a potentially infinite loop within the main loop.
        if(command == CLEAR) // if there was an error or clear
        {
            status.mmode = 0; // Entered Maintenence Mode (LCD and Communications are now on)
            COMSTAT.STATE = 0b000; // I am now a lonely idling master
            lcd_background(0,0,0);
            return true;
        }
        else if(command < 10) // is a number
        {
            if(command == password[i])
            {
                ++i; // next key
            }
            else
            {
                status.mmode = 0; // Entered Maintenence Mode (LCD and Communications are now on)
                COMSTAT.STATE = 0b000; // I am now a lonely idling master
                lcd_display("Wrong Password      ", "                    ");
                delay(3);
                lcd_background(0,0,0);
                return true;
            }
        }
        // reset our pointers for the menu display
        curFunct = NULL;
        subMenuPtr = 0;
        mainMenuPtr = 0;
        return false;
    }

}

/// display "exiting", then turn off the led, and exit maintainence mode
void lcd_end(unsigned char NA)               // exit out of menu and return to normal
{
    curFunct = NULL;
    if(status.mmode == 1) 
    {
        status.mmode = 0; // Entered Maintenence Mode (LCD and Communications are now on)
        lcd_display("                    ", "                    ");
    }
    else return;
}

// Keypad Functions
/// Push a keypad press to the keypad command FIFO
unsigned keypad_push(char keypress)
{
    unsigned char command = NOKEY; // command to store into FIFO: if greater than 10, then is a command, otherwise is a keypress
    struct fifo *newAddr;
    switch(keypress)
    {
        // number keys
        case 'C':
            command = 1;
            break;
        case 'D':
            command = 2;
            break;
        case 'E':
            command = 3;
            break;
        case '8':
            command = 4;
            break;
        case '9':
            command = 5;
            break;
        case 'A':
            command = 6;
            break;
        case '4':
            command = 7;
            break;
        case '5':
            command = 8;
            break;
        case '6':
            command = 9;
            break;
        case '1':
            command = 0;
            break;
        // end of the numbers

        case '0': // clear key (Cancel)
            command = CANCEL;
            break;
        case '2': // help key
            command = HELPKEY;
            break;
        case '3': // enter key
            command = ENTERKEY;
            break;
        case '7': // 2nd key
            command = SECONDKEY;
            break;
        case 'B': // down key
            command = DOWNKEY;
            break;
        case 'F': //up key
            command = UPKEY;
    }

    // now that the keypress has been translated, we will make a spot for it.
    if(command != NOKEY && keypadNum < 50) // limited to 50 keypresses
    {
        newAddr = (struct fifo *) malloc(sizeof(struct fifo));
        if(newAddr == (struct fifo *) NULL)
        {
            return true;
        }
        if(keypadOut==NULL) // if the queue is empty
        {
            keypadOut = newAddr; // put in the new address
        }
        if(keypadIn != NULL) // if the queue is not empty
        {
            keypadIn->nextAddr = newAddr;
        }
        newAddr->byte = command;  // set the command inside our new array
        newAddr->nextAddr = NULL; // ensure the pointer is null so that we know it is the end
        keypadIn = newAddr;       // change the pointer finally
        ++keypadNum;
        return false;
    }
    return true;
}

/*! \brief Get the last command from the keypad
 * Attribution: Code from pages 630-633 of "A First Book of ANSI C" by Bronson.
 */
unsigned char keypad_pull()
{
    struct fifo *nextAddr;
    unsigned char command;

    if(keypadOut == NULL) // handle an empty FIFO
    {
        return NOKEY;
    }

    command = keypadOut->byte; // get the byte out of the FIFO
    nextAddr = keypadOut->nextAddr; // get the next address

    free(keypadOut); // free the memory up - No Memory Leaks allowed!
    
    keypadOut = nextAddr; // move the pointer to the next bit of data
    --keypadNum;
    return command;
}

/*! \brief Give the size of the dynamic keypad FIFO
 * 
 *  Purpose: Size management, Debug
 * 
 *  Output: the number of entries there are being used.
 */
unsigned char keypad_size(void)
{
    return keypadNum;
}

/*! \brief Flush the keypad FIFO
 * 
 */
unsigned keypad_flush(void)
{
    if(keypad_pull() == NOKEY)
    {
        return true; // already empty
    }
    while(keypad_pull() != NOKEY);
    return false; // had something in it
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