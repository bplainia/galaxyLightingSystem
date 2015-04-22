/*! \page
 * Contains the functions for the menu items on the LCD.
 */

#include <lcd.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

typedef struct
{
    unsigned setting :1;
    unsigned ran :1;
    unsigned key : 5;
    unsigned newDisp : 1;
}specialChar;

// Local Variable

unsigned char lineData[21];

#define NUMMENUENTRIES 7
/// \todo TODO: Make the menues into a Constant that resides in program memory.
void lcd_setup()
{
    // setup LCD pins: memory has already been started, so no need to setup
    TRISEbits.TRISE6 = 0;
    TRISEbits.TRISE5 = 0;
    TRISCbits.TRISC2 = 0;
    LATCbits.LATC2 = 1; // reset off on lcd

    // Now Let's Initialize the LCD
    i2c_lcdInit(); // a special function that had to be made.
    lcd_display(0,NULL);
    strcpy(lineData,"Initializing");
    lcd_display(1,lineData);
    strcpy(lineData," PIC");
    lcd_display(2,lineData);
    __delay_ms(10);  __delay_ms(10);  __delay_ms(10);  __delay_ms(10);
    __delay_ms(10);  __delay_ms(10);  __delay_ms(10);  __delay_ms(10);
    __delay_ms(10);  __delay_ms(10);  __delay_ms(10);  __delay_ms(10);
    __delay_ms(10);  __delay_ms(10);  __delay_ms(10);  __delay_ms(10);
    __delay_ms(10);  __delay_ms(10);  __delay_ms(10);  __delay_ms(10);
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
    menu[3].text = "Josh's Menu";
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
        menu[4].entry[0].function = menu_seeTime;
        menu[4].entry[0].data = NULL;

        menu[4].entry[1].text = "Set Time";
        menu[4].entry[1].function = menu_setTime;

        menu[4].entry[2].text = "Go to Main Menu";
        menu[4].entry[2].function = menu_up;
    menu[4].numEntries = 3;

    // Misc. Options (Includes Power Control)
    menu[5].text = "Misc. Options";
        menu[5].entry[0].text = "Toggle AC/Battery";
        menu[5].entry[0].function = menu_setACBatt;           //allows user to switch power via interface
        menu[5].entry[0].data = NULL;

        menu[5].entry[1].text = "Go to Main Menu";
        menu[5].entry[1].function = menu_up;
    menu[5].numEntries = 2;

    // About
    menu[6].text = "About               ";
        menu[6].entry[5].text = FIRMWARE_VERSION;
        menu[6].entry[5].function = menu_up;
        menu[6].entry[5].data = NULL;
        
        menu[6].entry[1].text = "CEO: DJ S.";
        menu[6].entry[1].function = menu_up;
        menu[6].entry[1].data = NULL;
        
        menu[6].entry[2].text = "CFO: Ben Plain";
        menu[6].entry[2].function = menu_up;
        menu[6].entry[2].data = NULL;
        
        menu[6].entry[3].text = "CEvE: Josh Smith";
        menu[6].entry[3].function = menu_up;
        menu[6].entry[3].data = NULL;
        
        menu[6].entry[4].text = "CME: J Luke Williams";
        menu[6].entry[4].function = menu_up;
        menu[6].entry[4].data = NULL;
        
        menu[6].entry[0].text = "Patience, INC";
        menu[6].entry[0].function = menu_up;
        menu[6].entry[0].data = NULL;
        
        menu[6].entry[6].text = "Go to Main Menu";
        menu[6].entry[6].function = menu_up;
        menu[6].entry[6].data = NULL;
    menu[6].numEntries = 7;

    //Maintenance needed, these are read only
    menu[7].text = "Error Alerts      ";      //no response. "immediate attention needed"
        menu[7].entry[0].text = "Battery offline   ";       //
        menu[7].entry[0].function = NULL;
        menu[7].entry[0].data = NULL; // batt_live

        menu[7].entry[1].text = "Bulb dead       ";
        menu[7].entry[1].function = NULL; //bulb_detect;
        menu[7].entry[1].data = NULL;

        menu[7].entry[2].text = "Tracking motors off       ";
        menu[7].entry[2].function = NULL;
        menu[7].entry[2].data = NULL;

        menu[7].entry[3].text = "Sensors off range        ";
        menu[7].entry[3].function = NULL;
        menu[7].entry[3].data = NULL;

        menu[7].entry[4].text = "Comm Offline       ";
        menu[7].entry[4].function = NULL;
        menu[7].entry[4].data = NULL;

        menu[7].entry[5].text = "Go to Main Menu    ";
        menu[7].entry[5].function = menu_up;
        menu[7].entry[5].data = NULL;
    menu[7].numEntries = 6;         //TODO: Do not understand!! what goes in questionmark?

    
    // If you add another main menu entry, make sure to change NUMMENUENTRIES

    curFunct = NULL;
    mainMenuPtr = -1, subMenuPtr = -1;
    selectedPole=255;
    return;
}

/// display USB connected on lcd. Press the special key to disconnect.
/// 
void lcd_usb(unsigned char enable)      
{
    /// Set Color to Blue
    lcd_background(0,0,255);
    /// Then display USB Connected
    lcd_display(0,NULL);
    strcpy(lineData,"      USB Mode");
    lcd_display(1,lineData);
    strcpy(lineData,"       Active");
    lcd_display(2,lineData);
    return;
}

unsigned char lcd_display(unsigned char line, unsigned char data[21])
{
/*! # Research into the ST7036
 * found a link (https://www.newhavendisplay.com/app_notes/NHD-C0220BiZ.txt)[https://www.newhavendisplay.com/app_notes/NHD-C0220BiZ.txt]
 */
    char firstLine = 0x02, secondLine = 0xC0, clear = 0x01;
    unsigned char i;

    if(line == 0 || data == NULL)
    {
        //i2c_lcdInit();
        i2c_tx(0x78,0,0x00,0,&clear,1);
        __delay_ms(10); // wait for it to clear.
    }
    // Write to the first line
    else if(line == 1)
    {
        i2c_tx(0x78,0,0x00,0,&firstLine,1); // DDRAM address set to 0;
        i = 0;
        while(i < 20) // find the null character in the string
        {
            if(data[i] == '\0') break;
            ++i;
        }
        i2c_tx(0x78,0,0x40,0,data,i);
    }

    // write to the second line
    else if(line == 2)
    {
        i2c_tx(0x78,0,0x00,0,&secondLine,1); // DDRAM address set to 0x40 - the second line.
        i = 0;
        while(i < 20) // find the null character in the string
        {
            if(data[i] == '\0') break;
            ++i;
        }
        i2c_tx(0x78,0,0x40,0,data,i);
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
    return;
}

// ************************ Menu Functions ******************************* //

/// Change the pointer to which menu item we are displaying.
void menu_up(unsigned char none)
{
    curFunct = NULL;
    if(subMenuPtr != -1)
    {
        subMenuPtr = -1;
    }
    /// TODO: What if we are in the main menu?
    return;
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
    return;
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
        if((mainMenuPtr != 0) && (mainMenuPtr != 6)) menu_getSelection();

        subMenuPtr = 0; // go to the main screen of the submenu
    }
    else // otherwise we are in a submenu and should execute the entry's function.
    {
        curFunct = menu[mainMenuPtr].entry[subMenuPtr].function;
        curFunct(selectedPole);
    }
}

// Generates a lcdEntry and displays it. This is the main function.
void menu_display()
{
    char line1[25], line2[25];

    if(mainMenuPtr == -1 || status.mmode == 0)
    {
        return;
    }
    else if(subMenuPtr == -1 && mainMenuPtr != -1) // we are in a main menu.
    {
        strcpy(line1,"   --Main Menu--");
        strcpy(line2,menu[mainMenuPtr].text);
    }
    else
    {
        strcpy(line1,menu[mainMenuPtr].text);
        if(menu[mainMenuPtr].entry[subMenuPtr].data != NULL)
        {
             sprintf(line2,menu[mainMenuPtr].entry[subMenuPtr].text,menu[mainMenuPtr].entry[subMenuPtr].data);
        }
        else
        {
            strcpy(line2, menu[mainMenuPtr].entry[subMenuPtr].text);
        }
    }
    
    // now display the screen on the lcd
    lcd_display(0,NULL);
    lcd_display(1,line1);
    lcd_display(2,line2);
}

void menu_getSelection() // BLOCKING
{
    unsigned int number; // keypad number entry
    unsigned char numberStr[13];
    unsigned char key;
    unsigned char second=0;
    lcd_display(0,NULL);
    strcpy(lineData,"Please enter pole #:");
    lcd_display(1,lineData);
    strcpy(lineData,"(Enter for Local) ");
    lcd_display(2,lineData);
    while(1) // loop for errors
    {
        while(1) // loop for first digit or local
        {
            key=keypad_pull();
            if(key < 10)
            {
                number = key;
                sprintf(numberStr,"Pole: %3d         ",number);
                lcd_display(2,numberStr);
                break;
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
                    strcpy(numberStr,"(Enter for Global)");
                    lcd_display(2,numberStr);
                }
                else
                {
                    second = 0;
                    strcpy(numberStr,"(Enter for Local) ");
                    lcd_display(2,numberStr);
                }
            }
        }
        while(1) // loop for second digit
        {
            key=keypad_pull();
            if(key < 10)
            {
                number = key + (number*10);
                sprintf(numberStr,"Pole: %3d",number);
                lcd_display(2,numberStr);
                break;
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
                sprintf(numberStr,"Pole: %3d",number);
                if(number > 249)
                {
                    lcd_display(0,NULL);
                    strcpy(lineData,"Please enter valid");
                    lcd_display(1,lineData);
                    strcpy(lineData,"pole below 250!");
                    lcd_display(2,lineData);
                    delay(8);
                    strcpy(lineData,"Please enter pole #:");
                    lcd_display(1,lineData);
                    
                    strcpy(numberStr,"(Enter for Local) ");
                    lcd_display(2,numberStr);
                    second = 0;
                    number = 0;
                    break;
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
        lcd_display(0,NULL);
        strcpy(lineData,"Change Light Mode");
        lcd_display(1,lineData);
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
        lcd_display(0,NULL);
        strcpy(lineData,"Canceled Changing");
        lcd_display(1,lineData);
        strcpy(lineData,"Light Mode");
        lcd_display(2,lineData);
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
            strcpy(lineData,"Off             ");
            break;
        case 1:
            strcpy(lineData,"Dim             ");
            break;
        case 2:
            strcpy(lineData,"High            ");
            break;
        case 3:
            strcpy(lineData,"Auto            ");
            break;
        case 4:
            strcpy(lineData,"Cancel");
    }
    lcd_display(2,lineData);
}

/// Menu Function for activating hurricane mode
void menu_setHurricaneMode(unsigned char id)
{
    static unsigned swtch = 0;
    unsigned char setting, key;
    setting = (setting_bits1 | 0b00000010) >> 1;
    if(swtch == 0)
    {
        lcd_display(0,NULL);
        if(setting==0)
        {
            strcpy(lineData,"Confirm Activation");
            lcd_display(1,lineData);
            strcpy(lineData,"ENTER/CLEAR");
            lcd_display(2,lineData);    
        }
        else
        {
            strcpy(lineData,"Unlock Hurricane");
            lcd_display(1,lineData);
            strcpy(lineData,"(CLEAR cancels)");
            lcd_display(2,lineData);    
        }
        swtch = 1;
    }
    key = keypad_pull();
    if(key == CANCEL)
    {
        lcd_display(0,NULL);
        strcpy(lineData,"Canceled Changing");
        lcd_display(1,lineData);
        strcpy(lineData,"Hurricane Mode");
        lcd_display(2,lineData);
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
void menu_setPanelMode(unsigned char id) 
{
    static specialChar x = {0,1,NOKEY,1};
    if(x.ran==0)
    {
        x.setting = setting_bits1 | 0b00000001;
        x.ran=1;
        if(x.setting == 1)
        {
            strcpy(lineData,"Confirm Unlock Panel");
        }
        else
        {
            strcpy(lineData,"Confirm Lock Panel");
        }
        lcd_display(0,NULL);
        lcd_display(1,lineData);
        strcpy(lineData,"Enter or Cancel?");
        lcd_display(2,lineData);
    }
    x.key = keypad_pull();
    if(x.key == CANCEL || x.key == SECONDKEY)
    {
        lcd_display(0,NULL);
        strcpy(lineData,"Canceled Changing");
        lcd_display(1,lineData);
        strcpy(lineData,"Panel Mode");
        lcd_display(2,lineData);
        delay(8);
        curFunct=NULL;
        return;
    }
    else if(x.key == ENTERKEY)
    {
        if(x.setting > 0)
        {
            setting_bits1 |= 0b00000001; // lock the panel with a 1
            menu[2].entry[2].text = "Unlock Panel";
        }
        else
        {
            setting_bits1 &= 0b11111110; // unlock the panel with a 0
            menu[2].entry[2].text = "Lock Panel";
        }
        x.ran=0;
        curFunct=NULL;
        return;
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
        lcd_display(0,NULL);
        strcpy(lineData,"Canceled changing");
        lcd_display(1,lineData);
        strcpy(lineData,"the Power Source");
        lcd_display(2,lineData);
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
            lcd_display(0,NULL);
            strcpy(lineData,"Select Powersource");
            lcd_display(1,lineData);
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
                strcpy(lineData,"Reset         ");
                break;
            case 1:
                strcpy(lineData,"AC Only       ");
                break;
            case 2:
                strcpy(lineData,"Battery Only  ");
                break;
            case 3:
                strcpy(lineData,"Auto (Batt/AC)");
                break;
            case 4:
                strcpy(lineData,"Cancel        ");
        }
        lcd_display(2,lineData);
        newDisp = 0;
    }
}

/// AC/Battery switch
/// \todo TODO: If we ever get time, we need to make these functions non-blocking
void menu_seeTime(unsigned char na)
{
    unsigned char curTimeString[21];
    static unsigned char lastSecond = 61; // force the update first time into the loop with an impossible value
    static unsigned ran = 0;
    datetime curTime;
    if(ran == 0)
    {
        lcd_display(0,NULL);
        strcpy(lineData,"Current Time:");
        lcd_display(1,lineData);
        ran = 1;
    }
    rtc_get(&curTime);
    if(lastSecond != curTime.second)
    {
        lastSecond = curTime.second;
        sprintf(curTimeString,"%2d/%2d/20%2d %2d:%2d:%2d",curTime.month,curTime.day,curTime.year,curTime.hour,curTime.minute,curTime.second);
        lcd_display(2,curTimeString);
    }
    if(keypad_pull() != NOKEY) // wait for a key to be pressed to leave the function
    {
        lastSecond = 61;
        curFunct = NULL;
        ran = 0;
        return;
    }
}

void menu_setXaxis(unsigned char id)
{
    static bit ran = 0;
    if(ran==0)
    {
        ran=1;
        strcpy(lineData,"Not Implemented");
        lcd_display(0,NULL);
        lcd_display(1,lineData);
        timeoutInit();
    }

    if(timeoutCheck(16))
    {
        curFunct = NULL;
        ran = 0;
    }
    return;
}

void menu_setYaxis(unsigned char id)
{
    static bit ran = 0;
    if(ran==0)
    {
        ran=1;
        strcpy(lineData,"Not Implemented");
        lcd_display(0,NULL);
        lcd_display(1,lineData);
        timeoutInit();
    }

    if(timeoutCheck(16))
    {
        curFunct = NULL;
        ran = 0;
    }
    return;
}

void menu_setTime(unsigned char na)
{
    static datetime curTime;
    unsigned char curTimeString[21];
    static unsigned char step = 0;
    static unsigned ran = 0;
    static bit blinker;
    static bit newDisp;
    unsigned char key;
    key=keypad_pull();
    if(ran==0)
    {
        ran=1;
        blinker = 0;
        newDisp=1;
        rtc_get(&curTime);
        timeoutInit();
        curTime.second = 0; // always start from zero
        strcpy(curTimeString,"Set Time:");
        lcd_display(0,NULL);
        lcd_display(1,curTimeString);
    }
    if(timeoutCheck(3)==1)
    {
        timeoutInit();
        blinker = !blinker;
        newDisp=1;
    }

    if(newDisp==1)
    {
        switch(step)
        {
            case 0: // year
                if(blinker==0 && curTime.year < 10) sprintf(curTimeString,"%2d/%2d/200%1d %2d:%2d",curTime.month,curTime.day,curTime.year,curTime.hour,curTime.minute);
                else if(blinker==0) sprintf(curTimeString,"%2d/%2d/20%2d %2d:%2d",curTime.month,curTime.day,curTime.year,curTime.hour,curTime.minute);
                else sprintf(curTimeString,"%2d/%2d/____ %2d:%2d",curTime.month,curTime.day,curTime.hour,curTime.minute);
                break;
            case 1: // month
                if(blinker==0) sprintf(curTimeString,"%2d/%2d/20%2d %2d:%2d",curTime.month,curTime.day,curTime.year,curTime.hour,curTime.minute);
                else sprintf(curTimeString,"__/%2d/20%2d %2d:%2d",curTime.day,curTime.year,curTime.hour,curTime.minute);
                break;
            case 2: // day
                if(blinker==0) sprintf(curTimeString,"%2d/%2d/20%2d %2d:%2d",curTime.month,curTime.day,curTime.year,curTime.hour,curTime.minute);
                else sprintf(curTimeString,"%2d/__/20%2d %2d:%2d",curTime.month,curTime.year,curTime.hour,curTime.minute);
                break;
            case 3: // hour
                if(blinker==0) sprintf(curTimeString,"%2d/%2d/20%2d %2d:%2d",curTime.month,curTime.day,curTime.year,curTime.hour,curTime.minute);
                else sprintf(curTimeString,"%2d/%2d/20%2d __:%2d",curTime.month,curTime.day,curTime.year,curTime.minute);
                break;
            case 4: // minute
                if(blinker==0) sprintf(curTimeString,"%2d/%2d/20%2d %2d:%2d",curTime.month,curTime.day,curTime.year,curTime.hour,curTime.minute);
                else sprintf(curTimeString,"%2d/%2d/20%2d %2d:__",curTime.month,curTime.day,curTime.year,curTime.hour);
                break;
        }
        lcd_display(2,curTimeString);
    }

    if(key == CANCEL)
    {
        curFunct = NULL;
        ran = 0;
    }
    else if(key == ENTERKEY)
    {
        ++step;
        if(step > 4)
        {
            rtc_set(curTime);
            curFunct = NULL;
            ran = 0;
        }
    }
    else if(key == UPKEY)
    {
        switch(step)
        {
        case 0: // year
            if(curTime.year==99) curTime.year = 0;
            else ++(curTime.year);
            break;
        case 1: // month
            if(curTime.month==12) curTime.month = 1;
            else ++(curTime.month);
            if(curTime.day > daysInMonth(curTime.month,curTime.year)) curTime.day = 1;
            break;
        case 2: // day
            if(curTime.day==daysInMonth(curTime.month,curTime.year)) curTime.day = 1;
            else curTime.day++;
            break;
        case 3: // hour
            if(curTime.hour == 23) curTime.hour = 0;
            else curTime.hour++;
            break;
        case 4: // minute
            if(curTime.minute == 59) curTime.minute = 0;
            else curTime.minute++;
        }
    }
    else if(key == DOWNKEY)
    {
        switch(step)
        {
        case 0: // year
            if(curTime.year==0) curTime.year = 99;
            else --(curTime.year);
            break;
        case 1: // month
            if(curTime.month==1) curTime.month = 12;
            else --(curTime.month);
            if(curTime.day > daysInMonth(curTime.month,curTime.year)) curTime.day = 1;
            break;
        case 2: // day
            if(curTime.day==1) curTime.day = daysInMonth(curTime.month,curTime.year);
            else curTime.day--;
            break;
        case 3: // hour
            if(curTime.hour == 0) curTime.hour = 23;
            else curTime.hour--;
            break;
        case 4: // minute
            if(curTime.minute == 0) curTime.minute = 59;
            else curTime.minute--;
        }
    }
    return;
}

static unsigned char daysInMonth(unsigned char month, unsigned char year)
{
    if(month==2) // february
    {
        if(year != 0 && (year%4 == 0))
        {
            return 28;
        }
        return 27;
    }
    else if(month == 3 || month == 6 || month == 9 || month == 11) // april, june, september, november
    {
        return 30;
    }
    return 31;
}
