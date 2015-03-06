// Author: Benjamin Plain
// Menu Listings and functions

/*
 * The menu structure of the maintenence controller goes in categories and commands.
 * Menu Structure:
 * Exit Maintenence Mode
 * Light Settings
 *
 *
 */

#include <hid.h>
#include <sensors.h>
#include <string.h>

typedef struct lcdEntry
{
    char *line1;
    char *line2;
};

typedef struct
{
    const char *text; // twenty characters long plus \0
    void (*function)(void);
}menuEntry;

typedef struct 
{
    void (*function)(void);
}usbEntry;

menuEntry menue[150];
usbEntry usbe[85];

// Functions that are for the above menu entries.
void menu1();
void menu2();
void menu1()
{
    lcd_menu(2);
}
void menu2()
{
   // light_toggle();
    menu1();
}
void menu_setup()
{
    
    /// Contains each entry for the menu. This is a constant.
    menue[0].text = "Exit M-Mode        ";
    menue[0].function = lcd_end; // calls lcd_end();
    menue[1].text = "Light Settings     ";
    menue[1].function = menu1;  // shows menu1
    usbe[0].function = lcd_end;
}