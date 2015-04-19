/*! \page
 * Contains the functions for the menu items on the LCD.
 */

#include <lcd.h>
#include <stdio.h>
#include <string.h>

// Local Variable
char mainMenuPtr = 0, subMenuPtr = 0;
unsigned char selectedPole=255;

#define NUMMENUENTRIES 7
/// \todo TODO: Make the menues into a Constant that resides in program memory.
void lcd_setup()
{
    // setup LCD pins: memory has already been started, so no need to setup
    TRISEbits.TRISE6 = 0;
    TRISEbits.TRISE5 = 0;

    // Now Let's Initialize the LCD
    i2c_lcdInit(); // a special function that had to be made.
    lcd_display("Initializing"," PIC");
    delay(3);
    /// \todo TODO: Remove "Initializing PIC" from the screen once it is done.


    /// \todo TODO: Add functions to the menues. this is for EVERYONE to do.
    /// Contains each entry for the menu. This is a constant.
    menu[0].text          = "Exit M-Mode";
        menu[0].entry[0].text = "Cancel";
        menu[0].entry[0].function = menu_up;
        menu[0].entry[0].data = NULL;
        
        menu[0].entry[1].text = "Confirm Exit";
        menu[0].entry[1].function = lcd_end;
        menu[0].entry[1].data = NULL;
    menu[0].numEntries = 2;

    // Light Settings
    menu[1].text          = "Light Settings";
        menu[1].entry[0].text = "Set Light Mode (Auto)";
        menu[1].entry[0].data = NULL;
        menu[1].entry[0].function = menu_setLightMode;
        
        menu[1].entry[1].text = "Set Dim Timeout: %2d";
        menu[1].entry[1].data = &setting_timeout;
        
        menu[1].entry[2].text = "Set Dim Level: %2d%%";
        menu[1].entry[2].data = &setting_lightDim;
        
        menu[1].entry[3].text = "Go to Main Menu";
        menu[1].entry[3].function = menu_up;
        menu[1].entry[3].data = NULL;
    menu[1].numEntries = 4;

    // Solar Settings
    // TODO: Need to set these according to the settings
    menu[2].text = "Solar Settings";
        menu[2].entry[0].text = "Move X-Axis (auto)";
        menu[2].entry[0].function = menu_setXaxis;
        menu[2].entry[0].data = NULL;
        
        menu[2].entry[1].text = "Move Y-Axis (auto)";
        menu[2].entry[1].function = menu_setYaxis;
        menu[2].entry[1].data = NULL;
        
        menu[2].entry[2].text = "Lock Movement";
        menu[2].entry[2].data = NULL;
        menu[2].entry[2].function = menu_setPanelMode;
        
        menu[2].entry[3].text = "Activate Hurri-Mode";
        menu[2].entry[3].data = NULL;
        menu[2].entry[3].function = menu_setHurricaneMode;
        
        menu[2].entry[4].text = "Go to Main Menu";
        menu[2].entry[4].function = menu_up;
        menu[2].entry[4].data = NULL;
    menu[2].numEntries = 5;

    // Comm Status
    menu[3].entry[0].text = "Comm Status";
    menu[3].entry[1].text = "Master/Slave Mode";       // 0=master, 1=slave
    menu[3].entry[2].text = "Comm to Master ";
    menu[3].entry[3].text = "Pole ID#:%3d";
    menu[3].entry[3].data = &myAddr;       //my addr (find this)
    menu[3].entry[4].text = "Go to Main Menu";
    menu[3].entry[0].function = menu_up;
    menu[3].numEntries = 1;

    // Clock Options
    menu[4].text          = "Clock Options";
        menu[4].entry[0].text = "Show Time";
        menu[4].entry[0].function = menu_seeTime();
        menu[4].entry[0].data = NULL;

        menu[4].entry[1].text = "Set Time";
        menu[4].entry[1].function = menu_setTime();

        menu[4].entry[2].text = "Go to Main Menu";
        menu[4].entry[2].function = menu_up;
    menu[4].numEntries = 3;

    // Misc. Options (Includes Power Control)
    menu[5].text = "Misc. Options";
        menu[5].entry[0].text = "Toggle AC/Battery";
        menu[5].entry[0].function = menu_ACBatt;           //allows user to switch power via interface
        menu[5].entry[0].data = NULL;

        menu[5].entry[1].text = "Go to Main Menu";
        menu[5].entry[1].function = menu_up;
    menu[5].numEntries = 2;

    // About
    menu[6].text = "About               ";
        menu[6].entry[5].text = FIRMWARE_VERSION;
        menu[6].entry[5].function = menu_up;
        menu[6].entry[5].data = NULL;
        
        menu[6].entry[1].text = "CEO: DJ S."
        menu[6].entry[1].function = menu_up;
        menu[6].entry[1].data = NULL;
        
        menu[6].entry[2].text = "CFO: Ben Plain"
        menu[6].entry[2].function = menu_up;
        menu[6].entry[2].data = NULL;
        
        menu[6].entry[3].text = "CEvE: Josh Smith"
        menu[6].entry[3].function = menu_up;
        menu[6].entry[3].data = NULL;
        
        menu[6].entry[4].text = "CME: J Luke Williams"
        menu[6].entry[4].function = menu_up;
        menu[6].entry[4].data = NULL;
        
        menu[6].entry[0].function = "Patience, INC"
        menu[6].entry[0].function = menu_up;
        menu[6].entry[0].data = NULL;
        
        menu[6].entry[6].text = "Go to Main Menu";
        menu[6].entry[6].function = menu_up;
        menu[6].entry[6].data = NULL;
    menu[6].numEntries = 7;

    //Maintenance needed, these are read only
    menu[7].text = "Error Alerts      ";      //no response. "immediate attention needed"
        menu[7].entry[0].text = "Battery offline   ";       //
        menu[7].entry[0].data = batt_live;

        menu[7].entry[1].text = "Bulb dead       ";
        menu[7].entry[1].function = bulb_detect;

        menu[7].entry[2].text = "Tracking motors off       ";

        menu[7].entry[3].text = "Sensors off range        ";

        menu[7].entry[4].text = "Comm Offline       ";

        menu[7].entry[5].text = "Go to Main Menu    ";

        menu[7].entry[6].function = menu_up;
    menu[7].numEntries = 7;         //TODO: Do not understand!! what goes in questionmark?

    
    // If you add another main menu entry, make sure to change NUMMENUENTRIES

    curFunct = NULL;
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
    char firstLine = 0x80, secondLine = 0xC0, clear = 0x01;
    unsigned char i;

    i2c_tx(0x78,0,0,0,&clear,1);
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
    curFunct = NULL;
    if(subMenuPtr >= 0)
    {
        subMenuPtr = -1;
    }
    /// TODO: What if we are in the main menu?
}

/// Change the pointer to next number or come around. Called by the down button.
void menu_next(unsigned char none)
{
    curFunct = NULL;
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
    curFunct = NULL; // only run once
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
    curFunct = NULL; // only run once
    if(subMenuPtr < 0) // we are in the main screen
    {
        /// \todo TODO: Get the pole ID if the main menu items are not 0, 4, or 6.

        // Get Pole ID menu.
        menu_getSelection();

        subMenuPtr = 0; // go to the main screen of the submenu
    }
    else // otherwise we are in a submenu and should execute the entry's function.
    {
        curFunct = menu[mainMenuPtr].entry[subMenuPtr].function;
        curFunct(selectedPole);
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

menu_getSelection() // BLOCKING
{
    unsigned char number; // keypad number entry
    const unsigned char numberStr;
    unsigned char key;
    unsigned char second=0;
    lcd_display("Please enter pole #:","(Enter for Local)");
    while(1) // loop for errors
    {
        while(1) // loop for first digit or local
        {
            key=keypad_pull();
            if(key < 10)
            {
                number = key;
                sprintf(numberStr,"Pole: %1d",number);
                lcd_display("Please enter pole #:",numberStr);
            }
            else if(key == ENTERKEY)
            {
                if(second==0)
                {
                    selectedPole = 255;
                }
                else
                {
                    selectedPole = 250;
                }
                return;
            }
            else if(key == SECONDKEY)
            {
                if(second==0)
                {
                    second = 1;
                    lcd_display("Please enter pole #:","(Enter for Global)");
                }
                else
                {
                    second = 0;
                    lcd_display("Please enter pole #:","(Enter for Local)");
                }
            }
        }
        while(1) // loop for second digit
        {
            key=keypad_pull();
            if(key < 10)
            {
                number = key + (number*10);
                sprintf(numberStr,"Pole: %1d",number);
                lcd_display("Please enter pole #:",numberStr);
            }
            else if(key == ENTERKEY)
            {
                selectedPole = number;
                return;
            }
        }
        while(1) // loop for third digit
        {
            key=keypad_pull();
            if(key < 10)
            {
                number = key + (number*10);
                sprintf(numberStr,"Pole: %1d",number);
                if(number > 249)
                {
                    lcd_display("Please enter valid","pole below 250!");
                    delay(8);
                    lcd_display("Please enter pole #:","(Enter for Local)");
                    second = 0;
                    number = 0;
                }
                else
                {
                    selectedPole = number;
                    return;
                }
            }
            else if(key == ENTERKEY)
            {
                selectedPole = number;
                return;
            }
        }
    }
}

//*****************************************************************************/
// Functions for the menu entries that need further user input
/// Set the light mode (full,dim,auto,off)
void menu_setLightMode(unsigned char id)
{
    static unsigned char setting;
    static unsigned swtch = 0;
    unsigned char key;
    if(swtch==0) // swtch is the variable that keeps this from importing the current setting every time.
    {
        setting = (setting_bits1 | 0b00001100) >> 2;
        swtch = 1;
    }
    key = keypad_pull();
    if(key == UPKEY)
    {
        if(setting == 4) setting = 0;
        else ++setting;
    }
    else if(key == DOWNKEY)
    {
        if(setting == 0) setting = 4;
        else --setting;
    }
    else if(key == CANCEL)
    {
        lcd_display("Canceled Changing","Light Mode");
        delay(10);
        curFunct = NULL;
        swtch = 0;
        return;
    }
    else if(key == ENTERKEY)
    {
        if(setting < 4) 
        {
            setting_bits1 &= 0b11110011; // reset the bits
            setting_bits1 |= setting << 2; // then write the bits.
            switch(setting)
            {
                case 0:
                    menu[1].entry[0].text = "Light Mode (OFF)";
                    break;
                case 1:
                    menu[1].entry[0].text = "Light Mode (Dim)";
                    break;
                case 2:
                    menu[1].entry[0].text = "Light Mode (Full)";
                    break;
                case 3:
                    menu[1].entry[0].text = "Light Mode (Auto)";
            }
        }
        curFunct = NULL;
        swtch = 0;
        return;
    }
    switch(setting)
    {
        case 0:
            lcd_display("Change Light Mode","Off");
            break;
        case 1:
            lcd_display("Change Light Mode","Dim");
            break;
        case 2:
            lcd_display("Change Light Mode","High");
            break;
        case 3:
            lcd_display("Change Light Mode","Auto");
            break;
        case 4:
            lcd_display("Change Light Mode","Cancel");
    }
}

/// Menu Function for activating hurricane mode
void menu_setHurricaneMode(unsigned char id)
{
    static unsigned swtch = 0;
    unsigned char setting, key;
    setting = (setting_bits1 | 0b00000010) >> 1;
    if(swtch == 0)
    {
        if(setting==0)
        {
                lcd_display("Confirm Activation","ENTER/CLEAR");
        }
        else
        {
                lcd_display("Unlock Hurricane","(CLEAR cancels)");
        }
        swtch = 1;
    }
    key = keypad_pull();
    if(key == CANCEL)
    {
        lcd_display("Canceled Changing","Hurricane Mode");
        delay(10);
        swtch = 0;
        curFunct = NULL;
    }
    else if(key == ENTERKEY)
    {
        //lcd_display("Moving Panel","");
        if(setting == 0) 
        {
            setting_bits1 |= 0b00000011; // Activate hurricane mode and the lock
            menu[2].entry[3].text = "Deactivate H-Mode";
            menu[2].entry[2].text = "Pan Locked by H-Mode";
            menu[2].entry[2].function = NULL; // disable unlocking
        }
        else 
        {
            setting_bits1 &= 0b11111101; // deactivate hurricane mode
            menu[2].entry[3].text = "Activate H-Mode";
            menu[2].entry[2].text = "Unlock Panel";
            menu[2].entry[2].function = menu_setPanelMode; // return back to its normal state
        }
        swtch = 0;
        curFunct = NULL;
    }
}

///  Lock the solar panel (Auto/Locked)
/// Menu Function for activating hurricane mode
void menu_setPanelMode(unsigned char id)  ///////////////////BLOCKING!
{
    unsigned char setting, key;
    setting = setting_bits1 | 0b00000001;
    while(1)
    {
        if(setting==0)
        {
                lcd_display("Panel Movement set:","Locked/Manual");
        }
        else
        {
                lcd_display("Panel Movement set:","Auto Mode");
        }
        while((key = keypad_pull()) == NOKEY) continue;
        if(key == UPKEY | key == DOWNKEY)
        {
            setting  = !setting;
        }
        else if(key == CANCEL)
        {
            lcd_display("Canceled Changing","Panel Mode");
            delay(8);
            break;
        }
        else if(key == ENTERKEY)
        {
            if(setting > 0)
            {
                setting_bits1 |= 0b00000001;
            }
            else
                setting_bits1 &= 0b11111110;
        }
    }
}

/// AC/Battery switch
void menu_setACBatt(unsigned char id)
{
    static unsigned char setting;
    static unsigned swtch = 0, newDisp=1;
    unsigned char key;
    if(swtch==0)
    {
        setting = (setting_bits1 | 0b00110000) >> 4; // import current value once
        swtch = 1;
        newDisp=1;
    }
    key = keypad_pull();
    if(key == UPKEY)
    {
        if(setting == 4) setting = 0;
        else ++setting;
        newDisp=1;
    }
    else if(key == DOWNKEY)
    {
        if(setting == 0) setting = 4;
        else --setting;
        newDisp=1;
    }
    else if(key == CANCEL)
    {
        lcd_display("Canceled changing","the Power Source");
        delay(8);
        curFunct = NULL;
        newDisp = 1;
        swtch = 0;
        return;
    }
    else if(key == ENTERKEY)
    {
        if(setting < 4)
        {
            setting_bits1 &= 0b11001111; // reset the bits
            setting_bits1 |= (setting%4) << 4; // then write the bits.
        }
        curFunct = NULL;
        swtch = 0;
        switch(setting)
        {
            case 0:
                menu[5].entry[0].text = "Power Reset";
                break;
            case 1:
                menu[5].entry[0].text = "Power on AC ONLY";
                break;
            case 2:
                menu[5].entry[0].text = "Power on BAT ONLY";
                break;
            case 3:
                menu[5].entry[0].text = "Power on Auto";
        }
        return;
    }
    if(newDisp)
    {
        switch(setting)
        {
            case 0:
                lcd_display("Select Powersource","Reset");
                break;
            case 1:
                lcd_display("Select Powersource","AC Only");
                break;
            case 2:
                lcd_display("Select Powersource","Battery Only");
                break;
            case 3:
                lcd_display("Select Powersource","Auto (Batt/AC)");
                break;
            case 4:
                lcd_display("Select Powersource","Cancel");
        }
        newDisp = 0;
    }
}

/// AC/Battery switch
/// \todo TODO: If we ever get time, we need to make these functions non-blocking
void menu_seeTime(unsigned char na)
{
    const unsigned char *curTimeString;
    static unsigned char lastSecond = 61; // force the update first time into the loop with an impossible value
    datetime curTime;
    rtc_get(curTime);
    if(lastSecond != curTime.second)
    {
        lastSecond = curTime.second;
        sprintf(curTimeString,"%2d/%2d/20%2d %2d:%2d:%2d",curTime.month,curTime.day,curTime.year,curTime.hour,curTime.minute,curTime.second);
        lcd_display("Current Time:",curTimeString);
    }
    if(keypad_pull() != NOKEY) // wait for a key to be pressed to leave the function
    {
        lastSecond = 61;
        curFunct = NULL;
    }
}
