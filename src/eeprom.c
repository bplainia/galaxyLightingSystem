#include <shared.h>
#include <eeprom.h>
#include <rtc.h>

/// Check the memory. Certain things are checksumed.
unsigned char mem_check()
{
    unsigned char memStatus = 0;
    /// \todo TODO: check the POR, BOR, and VBATT bits to see where we returned from.
    /// For now, this function merely checks to see if the EEPROM exists
    /// \todo TODO: set myAddr, the address of the pole, which is saved in EEPROM
    if(i2c_check(0x50)) memStatus |= 0b00001000; // EEPROM Does not exist
    mem_read(0x0000,&myAddr,1);
    return memStatus; // this should always be something other than zero since it will always no have returned from zero
}

/// Save special bytes that are in ram to eeprom.
void mem_push()
{
    return;
}

/*! \brief saves an event to the log space.
 *
 * Inputs: An error code specifying what happened
 *
 * Description: Takes the error code and saves it with the current time stamp from the RTCC module.
 *
 * Ouput: A single byte specifying success (0=success). If there is no space, or the eeprom is not responding, this will return a number.
 *
 * Code | Reason
 * ---- | -----------------------
 * 0    | Everything was OK
 * 1    | No EEPROM or not responding
 * 2    | Not enough space left in LOG space
 *
 * \todo TODO: Finish log appending function
 * \todo TODO: How should we store the date and time efficiently?
 * \todo TODO: Add checksum for error checking of eeprom data
 */
unsigned char mem_append_log(unsigned char err)
{
    datetime currenttime;
    rtc_get(datetime currenttime);
    unsigned char* LOG[256];
    unsigned char errorNum;
    int i;
    int j;

    errorNum = err;
    i = 5;
    j = 0x10;

    LOG[0] = currenttime.year;
    LOG[1] = currenttime.month;
    LOG[2] = currenttime.day;
    LOG[3] = currenttime.hour;
    LOG[4] = currenttime.minute;
    LOG[i] = errorNum;


    mem_write(j, &LOG[0], 1);
    j++;
    mem_write(j, &LOG[1], 1);
    j++;
    mem_write(j, &LOG[2], 1);
    j++;
    mem_write(j, &LOG[3], 1);
    j++;
    mem_write(j, &LOG[4], 1);
    j++;
    mem_write(j, &LOG[i], 1);
    i++;
    j++;

if(j == 0x5F)
{
    j = 0x10;
}
else

continue;

    return 0; // default return is no error
}

/*! \brief Write an array of bytes (Private)
 *
 * Inputs: `addr`: The 16-bit address of the starting location
 *         `data`: A pointer to an array of bytes that contains `length` number of bytes of data
 *         `length`: Number of bytes to write to EEPROM
 *
 * Outputs: A single bit specifying Success
 */
static unsigned mem_write(unsigned short addr, unsigned char* data, unsigned char length)
{
    return i2c_tx(0x50,1,addr & 0x00FF,addr >> 8,data,length);
}

/*! \brief Write an array of bytes (Private)
 *
 * Inputs: `addr`: The 16-bit address of the starting location
 *         `data`: A pointer to an array of bytes that you want to put the data in
 *         `length`: Number of bytes to write to EEPROM
 *
 * Outputs: A array of data (`length` long). Will return the null pointer if there was an error.
 */
static unsigned mem_read(unsigned short addr, unsigned char *data, unsigned char length)
{
    // Create packet and byte array for the i2c function to write to
    return i2c_rx(0x50,1,addr & 0x00FF,addr >> 8,data,length);
}