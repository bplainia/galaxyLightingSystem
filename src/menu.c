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

typedef char menuEntry[19];


menuEntry menu_main[8];

menu[0].line1 = "> Exit Maint-Mode   ";
menu[0].line1 = "  Light Settings    ";


void menu1()
{
    lcd_menu(2);
}
void menu2()
{
    light_toggle();
}