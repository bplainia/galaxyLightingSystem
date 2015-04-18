/*! \page
 * Contains the functions for the menu items on the LCD.
 */

#include <lcd.h>
#include <stdio.h>
#include <string.h>

// Local Variable
char mainMenuPtr = 0, subMenuPtr = 0;

#define NUMMENUENTRIES 7
/// \todo TODO: Make the menues into a Constant that resides in program memory.
void lcd_setup()
{
    // setup LCD pins: memory has already been started, so no need to setup
    TRISEbits.TRISE6 = 0;
    TRISEbits.TRISE5 = 0;

    // Now Let's Initialize the LCD
    i2c_lcdInit(); // a special function that had to be made.
    lcd_display("Initializing        "," PIC               ");
    /// \todo TODO: Remove "Initializing PIC" from the screen once it is done.


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

    // Comm Status
    menu[3].entry[0].text = "Comm Status              ";
    menu[3].entry[1].text = "Master/Slave Mode       ";       // 0=master, 1=slave
    menu[3].entry[2].text = "Comm to Master         ";
    menu[3].entry[3].text = "Pole ID#:              ";
    menu[3].entry[3].data = &myAddr;       //my addr (find this)
    menu[3].entry[4].text = "Go to Main Menu    ";
    menu[3].entry[0].function = menu_up;
    menu[3].numEntries = 1;

    // Clock Options
    menu[4].text          = "Clock Options       ";
    menu[4].entry[0].text = "Show Time           ";
    menu[4].entry[1].text = "Set Time            ";
    menu[4].entry[2].text = "Go to Main Menu    ";
    menu[4].entry[2].function = menu_up;
    menu[4].numEntries = 3;

    // Misc. Options (Includes Power Control)
    menu[5].text = "Misc. Options       ";
    menu[5].entry[0].text = "Toggle AC/Battery   ";
    menu[5].entry[1].function = power_switch;           //allows user to switch power via interface
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

    //Maintenance needed, these are read only
    menu[7].text = "Error Alerts      ";      //no response. "immediate attention needed"
    menu[7].entry[0].text = "Battery offline   ";       //
    menu[7].entry[0].data = batt_live;
    menu[7].entry[1].text = "Bulb dead       ";
    menu[7].entry[1].data = bulb_detect;
    menu[7].entry[2].text = "Tracking motors off       ";
    menu[7].entry[3].text = "Sensors off range        ";
    menu[7].entry[4].text = "Comm Offline       ";
    menu[7].entry[5].text = "Go to Main Menu    ";
    menu[7].entry[6].function = menu_up;
    menu[7].numEntries = 7;         //TODO: Do not understand!! what goes in questionmark?

    
    // If you add another main menu entry, make sure to change NUMMENUENTRIES
}

/// display USB connected on lcd. Press the special key to disconnect.
/// 
void lcd_usb(unsigned char enable)      
{
    /// Set Color to Blue
    lcd_background(0,0,255);
    /// Then display USB Connected
    lcd_display("      USB Mode      ", "       Active       ");
}

unsigned char lcd_display(char line1[21], char line2[21])
{
/*! # Research into the ST7036
 * found a link (https://www.newhavendisplay.com/app_notes/NHD-C0220BiZ.txt)[https://www.newhavendisplay.com/app_notes/NHD-C0220BiZ.txt]
 */
    char firstLine = 0x80, secondLine = 0xC0;
    unsigned char i;

    // Write to the first line
    if(line1 != NULL)
    {
        i2c_tx(0x78,0,0x00,0,&firstLine,1); // DDRAM address set to 0;
        i = 0;
        while(i <= 20) // find the null character in the string
        {
            if(line1[i] == '\0') break;
            ++i;
        }
        i2c_tx(0x78,0,0x40,0,line1,i);
    }

    // write to the second line
    if(line2 != NULL)
    {
        i2c_tx(0x78,0,0x00,0,&secondLine,1); // DDRAM address set to 0x40 - the second line.
        i = 0;
        while(i <= 20) // find the null character in the string
        {
            if(line2[i] == '\0') break;
            ++i;
        }
        i2c_tx(0x78,0,0x40,0,line2,i);
    }
    return false;
}

/// Change the color of the LCD backlight
void lcd_background(unsigned char red, unsigned char green, unsigned char blue)
{
    //static unsigned char oldred,oldgreen,oldblue;

    /// \todo FIXME: Change channels to correct numbers
    pwm_set(4,red);
    pwm_set(5,green);
    pwm_set(6,blue);
}

// ************************ Menu Functions ******************************* //

/// Change the pointer to which menu item we are displaying.
void menu_up(unsigned char none)
{
    if(subMenuPtr >= 0)
    {
        subMenuPtr = -1;
    }
    /// TODO: What if we are in the main menu?
}

/// Change the pointer to next number or come around. Called by the down button.
void menu_next(unsigned char none)
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
void menu_prev(unsigned char none)
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
void menu_enter(unsigned char none)
{
    if(subMenuPtr < 0) // we are in the main screen
    {
        subMenuPtr = 0; // go to the main screen of the submenu
    }
    else // otherwise we are in a submenu and should execute the entry's function.
    {
        menu[mainMenuPtr].entry[subMenuPtr].function(0);
    }
}

// Generates a lcdEntry and displays it. This is the main function.
static void menu_display()
{
    char *line1, *line2;

    if(subMenuPtr < 0) // we are in a main menu.
    {
        line1 = "     Main Menu      ";
        line2 = menu[mainMenuPtr].text;
    }
    else
    {
        line1 = menu[mainMenuPtr].text;
        if(menu[mainMenuPtr].entry[subMenuPtr].data != NULL)
        {
            line2 = sprintf(menu[mainMenuPtr].entry[subMenuPtr].text,menu[mainMenuPtr].entry[subMenuPtr].data);
        }
        else
        {
            line2 = menu[subMenuPtr].text;
        }
    }
    
    // now display the screen on the lcd
    lcd_display(line1,line2);
}
