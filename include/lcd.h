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
#include <eeprom.h>
#include <light.h>
#include <movement.h>



typedef void (*menuFunct)(unsigned char); // menu functions. the char is for the pole ID.

typedef struct
{
    const char *text; // Main Menu Entry
    struct
    {
        const char *text; // Menu entry name
        menuFunct function; // function to call to execute command. The char is the pole number.
        unsigned char *data;
    }entry[7];
    char numEntries;
}menuEntry;


/// menu: The menu entries for the LCD.
/// Note that the 0th entry is the main menu.
menuEntry menu[8];
menuFunct curFunct; // if a function is selected
signed char mainMenuPtr = -1, subMenuPtr = -1;
unsigned char selectedPole;
// Functions that are for the menu system.
void menu_up(unsigned char);
void menu_next(unsigned char);
void menu_prev(unsigned char);
void menu_enter();
void menu_display();

// Functions that are for the LCD
void lcd_setup();
void lcd_usb(unsigned char);
unsigned char lcd_display(unsigned char, unsigned char*);
void lcd_background(unsigned char,unsigned char,unsigned char); // change the background color (PWM). Does not need return since it is internal.

void menu_getSelection(); // sets the pole number. asks when you enter a sub-menu.
void menu_setLightMode(unsigned char);
void menu_setHurricaneMode(unsigned char);
void menu_setPanelMode(unsigned char);
void menu_setACBatt(unsigned char);
void menu_seeTime(unsigned char);
void menu_setTime(unsigned char);
void menu_setXaxis(unsigned char);
void menu_setYaxis(unsigned char);
void menu_seeStatus(unsigned char);
void menu_setDimLevel(unsigned char);
void menu_alarm_battery(unsigned char);
void menu_alarm_light(unsigned char);
//void menu_alarm_motor(unsigned char);
//void menu_alarm_sensor(unsigned char);
void menu_alarm_comm(unsigned char);
void menu_setDimLength(unsigned char);
void menu_battVoltage(unsigned char);
static unsigned char daysInMonth(unsigned char, unsigned char);

/*! \page menu The LCD Menu System
 *
 * \section structure The Basic Structure of the system
 * <ol type="I">
 * <li>Exit M-Mode
 *     1. Cancel Exit
 *     2. Confirm Exit
 *
 * <li>Light Settings
 *     1. Manually Set Light (off/dim/on)
 *     2. Set Dimming Timeout (Always Bright(off) or increments of 1min.)
 *     3. Set Dimming Level (in %)
 *
 * <li>Solar Settings
 *     1. Move to x-axis location (Temporary until out of m-mode)
 *     2. Move to y-axis location (Temporary until out of m-mode)
 *     3. Lock Movement - confirmation required
 *     4. Activate Hurricane mode (if supported)
 *     5. Display Charging and Battery States
 *
 * <li>Status
 *     1. Need to be decided by comm committee
 *
 * <li>Clock Options
 *     1. Show Time
 *     2. Set Time (Sequence)
 * <li>Misc. Options
 *     1. Force AC or Toggle AC/Batt power.
 *     2. Restart Pole
 *
 * <li>About (it would be cool if this was a slideshow)
 *     1. Firmware Version
 *     2. CEO
 *     3. CFO
 *     4. A key person
 *     5. Mechanical Designer
 * </ol>
 *
 * \section use How to use the LCD Menu System
 *
 * ![The Keypad](../images/keypad.jpg "Retrieved from jameco.com")
 *
 * \subsection mainMenu The Main Menu
 *
 * In the main menu, you will use the up, down and enter keys.
 * On the top line of the LCD, "Main Menu" will be displayed.
 * On the bottom line of the LCD, the menu item will be displayed.
 * Press enter to go into the submenu. The arrow keys will navigate between menu items.
 * Most menu items will ask for the scope of the submenu (local, single, or global). The number buttons will be used to enter the scope.
 *
 * \subsection subMenu The Submenues
 *
 * In the submenus, the up, down, 2nd, and enter keys will be used.
 * Use the up/down keys to navigate through submenu items. Use the 2nd or clear button to go back to the main menu.
 * On the top line, the selected submenu will be displayed, with the currently selected item below.
 * Pressing enter will execute the menu item.
 *
 * Help system will be implemented in the charlie version.
 * \todo TODO: A help system will need to be documented and implemented.
 *
 * \section color Colors on the LCD
 * - Red: Error
 * - Green: Main Menu. Please pick a menu entry
 * - Yellow: Working... Please Wait
 * - White: Initializing...Please Wait
 * - Blue: USB Mode
 * - Other colors will show the scope of the command. <b>(These need to be decided in a better way)</b>
 *   - Green: Local
 *   - Pink: Single
 *   - Purple: Global
 *
 * For the slave state, button presses will either be ignored or will display a red error screen with "There is a master already"
 *
 *
 *
 * \section version Version:
 * This is version 1alpha. This was the first idea to be put onto paper/doxygen.
 * Will go to beta as soon as it is implemented in the code.
 */
