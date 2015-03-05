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

typedef struct lcdEntry
{
    char *line1;
    char *line2;
};

typedef void (*menuFunction)();

typedef struct menuEntry
{
    const char text[20]; // twenty characters long plus \0
    const menuFunction function;
};

typedef struct usbEntry
{
    const menuFunction function;
};

/// Contains each entry for the menu. This is a constant.
const menuEntry menuEntries[] = {{"Exit M-Mode        ",lcd_end}, // calls lcd_end();
                             "Light Settings     "  // shows menu1
                            };
const usbEntry usbEntries[] = {{lcd_end}};


// Functions that are for the above menu entries.
void menu1()
{
    lcd_menu(2);
}
void menu2()
{
    light_toggle();
}