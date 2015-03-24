/*! \page
 * Contains the functions for the menu items on the LCD.
 */

#include <menu.h>
#include <stdio.h>
#include <string.h>

// Local Variable
char mainMenuPtr = 0, subMenuPtr = 0;

#define NUMMENUENTRIES 6
/// \todo TODO: Make the menues into a Constant that resides in program memory.
void menu_setup()
{
    /// \todo TODO: Add functions to the menues. this is for EVERYONE to do.
    /// Contains each entry for the menu. This is a constant.
    menu[0].text          = "Exit M-Mode        ";
    menu[0].entry[0].text = "Cancel             ";
    menu[0].entry[0].function = menu_up;
    menu[0].entry[1].text = "Confirm Exit       ";
    menu[0].entry[1].function = lcd_end;
    menu[0].numEntries = 2;

    // Light Settings
    menu[1].text          = "Light Settings     ";
    menu[1].entry[0].text = "Set Light Mode: %1d  "; /// \todo FIXME: Can I use sprintf `%d`?
    menu[1].entry[0].data = &setting_lightMode;
    menu[1].entry[1].text = "Set Dim Timeout: %2d";
    menu[1].entry[1].data = &setting_timeout;
    menu[1].entry[2].text = "Set Dim Level: %2d%% ";
    menu[1].entry[3].text = "Go to Main Menu    ";
    menu[1].entry[3].function = menu_up;
    menu[1].numEntries = 4;

    // Solar Settings
    menu[2].text = "Solar Settings      ";
    menu[2].entry[0].text = "Move X-Axis         ";
    menu[2].entry[1].text = "Move Y-Axis         ";
    menu[2].entry[2].text = "Lock Movement       ";
    menu[2].entry[3].text = "Hurricane Mode: %1d   ";
    menu[2].entry[3].data = &setting_hurricane;
    menu[2].entry[4].text = "Go to Main Menu    ";
    menu[2].entry[4].function = menu_up;
    menu[2].numEntries = 5;

    // Status
    menu[3].text = "Status              ";
    menu[3].entry[0].text = "Go to Main Menu    ";
    menu[3].entry[0].function = menu_up;
    menu[3].numEntries = 1;

    // Clock Options
    menu[4].text          = "Clock Options       ";
    menu[4].entry[0].text = "Show Time           ";
    menu[4].entry[1].text = "Set Time            ";
    menu[4].entry[2].text = "Go to Main Menu    ";
    menu[4].entry[2].function = menu_up;
    menu[4].numEntries = 3;

    // Misc. Options
    menu[5].text = "Misc. Options       ";
    menu[5].entry[0].text = "Toggle AC/Battery   ";
    menu[5].entry[1].text = "Restart             ";
    menu[5].entry[2].text = "Go to Main Menu    ";
    menu[5].entry[2].function = menu_up;
    menu[5].numEntries = 3;

    // About
    menu[6].text = "About               ";
    menu[6].entry[0].text = FIRMWARE_VERSION;
    menu[6].entry[0].function = menu_up;
    menu[6].entry[1].function = menu_up;
    menu[6].entry[2].function = menu_up;
    menu[6].entry[3].function = menu_up;
    menu[6].entry[4].function = menu_up;
    menu[6].entry[5].function = menu_up;
    menu[6].entry[6].text = "Go to Main Menu    ";
    menu[6].entry[6].function = menu_up;
    menu[6].numEntries = 7;
    // If you add another main menu entry, make sure to change NUMMENUENTRIES
}

/// Change the pointer to which menu item we are displaying.
void menu_up()
{
    if(subMenuPtr >= 0)
    {
        subMenuPtr = -1;
    }
    /// TODO: What if we are in the main menu?
}

/// Change the pointer to next number or come around. Called by the down button.
void menu_next()
{
    if(subMenuPtr < 0) // if we are in a submenu
    {
        if(++mainMenuPtr > NUMMENUENTRIES)
        {
            mainMenuPtr = 0;
        }
    }
    else // otherwise, we are in a sub-menu
    {
        if(++subMenuPtr >= menu[mainMenuPtr].numEntries)
        {
            subMenuPtr = 0; // set back to zero if it is too big
        }
    }
    menu_display();
}

/// Go to the previous menu entry. Called by the up button.
void menu_prev()
{
    if(subMenuPtr < 0) // if we are in the main menu
    {
        if(--mainMenuPtr < 0)
        {
            mainMenuPtr = NUMMENUENTRIES;
        }
        
    }
    else // otherwise, we are in a sub-menu
    {
        if(--subMenuPtr < 0)
        {
            subMenuPtr = menu[mainMenuPtr].numEntries; // set back to zero if it is too big
        }
    }
}

/// This function is run by pressing the enter key on the keypad.
void menu_enter()
{
    if(subMenuPtr < 0) // we are in the main screen
    {
        subMenuPtr = 0; // go to the main screen of the submenu
    }
    else // otherwise we are in a submenu and should execute the entry's function.
    {
        menu[mainMenuPtr].entry[subMenuPtr].function();
    }
}

// Generates a lcdEntry and displays it. This is the main function.
static void menu_display()
{
    lcdEntry myScreen;

    if(subMenuPtr < 0) // we are in a main menu.
    {
        myScreen.line1 = "     Main Menu      ";
        myScreen.line2 = menu[mainMenuPtr].text;
    }
    else
    {
        myScreen.line1 = menu[mainMenuPtr].text;
        if(menu[mainMenuPtr].entry[subMenuPtr].data != NULL)
        {
            myScreen.line2 = sprintf(menu[mainMenuPtr].entry[subMenuPtr].text,menu[mainMenuPtr].entry[subMenuPtr].data);
        }
        else
        {
            myScreen.line2 = menu[subMenuPtr].text;
        }
    }
    
    // now display the screen on the lcd
    /// \todo TODO: Display the generated screen on the LCD. (I2C)
}