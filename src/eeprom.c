#include <shared.h>
#include <eeprom.h>
#include <rtc.h>

/// Check the memory. Certain things are checksumed.
unsigned char mem_check()
{
    return false;
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
 */
unsigned char mem_append_log(unsigned char err)
{

    return 0; // default return is no error
}

/*! \brief Write an array of bytes (Private)
 *
 * Inputs: `addr`: The 16-bit address of the starting location
 *         `data`: A pointer to an array of bytes
 *         `length`: Number of bytes to write to EEPROM
 *
 * Outputs: A single bit specifying Success
 */
static unsigned mem_write(unsigned short addr, unsigned char* data, unsigned char length)
{
    // Create packet


    return 0; // default return is no error
}

/*! \brief Write an array of bytes (Private)
 *
 * Inputs: `addr`: The 16-bit address of the starting location; `length`: Number of bytes to read from EEPROM
 *
 * Outputs: A array of data (`length` long). Will return the null pointer if there was an error.
 */
static unsigned char* mem_read(unsigned short addr, unsigned char length)
{
    // Create packet and byte array for the i2c function to write to
    unsigned char* data[50];
    i2cPacket packet; //! \todo FIXME: need to do the address of eeprom and memory correctly
    i2c_rx(&packet);

    return 0; // default return is no error
}