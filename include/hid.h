#ifndef HID_H
#define HID_H

// Variables
   //none

// HID (keyboard, usb, and lcd) Functions
void hid_setup(void); // setup the HID functions
void hid_loop(void);  // simple call to what to do in the loop
void hid_execute(unsigned char);  // execute a command (usb and lcd can call this)

// LCD functions
void lcd_begin(void);             // Welcome and setup screen for menu system
void lcd_menu(unsigned char);     // display a menu item
void lcd_execute(unsigned char, unsigned char);  // execute a menu item
void lcd_usb(unsigned char);      // display USB connected. Press ? to disconnect.
void lcd_end(void);               // exit out of menu and return to normal

// Keypad functions
void key_isr(void); // handle an interrupt due to a byte coming in.
void key_insertion(void);
void key_removal(void);

// USB functions
void usb_isr(void);
void usb_insertion(void);         // will disable lcd
void usb_removal(void);
void usb_begin(void);
void usb_oneStatus();
void usb_myStatus();
void usb_allStatus(); // this one will halt all functions until download is done
void usb_end(void);

#endif