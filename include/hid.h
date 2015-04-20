#ifndef HID_H
#define HID_H


#define CANCEL 10
#define UPKEY 11
#define DOWNKEY 12
#define HELPKEY 13
#define SECONDKEY 14
#define ENTERKEY 15
#define NOKEY 20

// Variables


// HID (keyboard, usb, and lcd) Functions
void hid_setup(void); // setup the HID functions
void hid_loop(void);  // simple call to what to do in the loop
void hid_execute(unsigned char);  // execute a command (usb and lcd can call this)

// LCD functions
unsigned lcd_begin(void);             // Welcome and setup screen for menu system
void lcd_end(unsigned char);               // exit out of menu and return to normal

// Keypad functions
unsigned keypad_push(unsigned char);
unsigned char keypad_pull(void);
unsigned char keypad_size(void);
unsigned keypad_flush(void);

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