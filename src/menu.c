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
    char line1[20]; // twenty characters long plus \0
    char line2[20];
};

/// Contains each entry for the menu. This is a constant.
const char* menuEntries[] = {"Exit M-Mode        ", // calls lcd_end();
                             "Light Settings     "  // shows menu1
                            };


void menu1()
{
    lcd_menu(2);
}
void menu2()
{
    light_toggle();
}