/*! \file
 * This file containes the functions that are used in more than one file.
 * This includes the ADC, I2C, and PWM.
 * Author: Benjamin Plain
 */

#include <shared.h>


/// Setup the ADC module
void adc_setup()
{
    // Input Configuration:
    // AN4 = Battery, AN2,3 = Pots, 8,19,18,17 = Photocells, AN0 = Temp
    ANCON1 = 0b10101101; // photo pin3, batt, pot pin21, pot pin22, temp
    ANCON2 = 0b00110000; // photo pin5, photo pin4
    ANCON3 = 0b01000000; // photo pin6

    // configuring the module: ___________________________________
    // output resolution, voltage reference, clock select, how will sampling be done, output format/destination, readings per interrupt
    ADCON1Hbits.MODE12 = 1;      // use 12-bit resolution
    ADCON1Lbits.SSRC   = 0b0111; // auto sample clock
    ADCON3H            = 0x1F;
    ADCON3L            = 0x02;
    ADCON2H            = 0;
    ADCON3L            = 0;
    ADCON1Hbits.ADON   = 1; // converter is now on!
}

 /// Read adc value out of the ADC buffer for the passed channel
/// The only channels we need are 0,2,3,4,8,17,18,19.
unsigned int adc_read(unsigned char channel)
{
        // Each pin is attached to an adc channel
        // Depending on which device (pin) is desired, that channel is requested
        // when an adc is performed the result is put into the buffer for that channel
        // for this code a pointer is created that points at the low byte of the buffer
        // an int is used to grab the data in the byte, which should grab the low and high byte

    ADCHS0Lbits.CH0NA = 0; // AVss
    ADCHS0Lbits.CH0SA = channel; // choose the correct channel
    ADCON1Lbits.SAMP = 1; // begin sampling
    while(!ADCON1Lbits.DONE) continue; // wait til it is done
    return ADCBUF0; // return the buffer
}

/// Setup TMR2 for use by all the CCP modules
void pwm_setup() 
{
    PR2   = 0XFF;          //
    // T2CON = 0b00000111; // 1:16 prescaler from instruction = 750kHz

    CCP3CON = 0b00111100; //???????
    T2CONbits.T2CKPS = 0b11;            //1:16 prescaler (now we have a 1MHz clock coming into TMRx)
    T2CONbits.TMR2ON = 1;
    // our minimum frequency is 1953.125Hz at 32MHz.
    // giving a resolution of 10 bits
    // CCPTMRS0 do not need modified. already set to TMR1/2 (PWM uses 2)

    // Some reminders:
    // PWM Period = (PR2 + 1) * 4 * (1/<Primary Oscilator>) * <TMR2 Prescale>
    // Max Resolution = log_2 (Fosc/Fpwm)

    // PWM Channels Setup
    TRISAbits.TRISA1 = 0;
    RPOR0_1bits.RPO1R = 0x9; // set RP1 to CCP9 for the Light
    CCP9CONbits.CCP9M = 0b1100;

    TRISEbits.TRISE4 = 0;
    RPOR32_33bits.RPO32R = 0x9; // set RP32 to CCP5 for RED PWM
    CCP5CONbits.CCP5M = 0b1100;

    TRISEbits.TRISE5 = 0;
    RPOR34_35bits.RPO34R = 0x9; // set RP34 to CCP6 for GRN PWM
    CCP6CONbits.CCP6M = 0b1100;

    TRISEbits.TRISE6 = 0;
    RPOR36_37bits.RPO37R = 0x8; // set RP37 to CCP7 for BLU PWM
    CCP7CONbits.CCP7M = 0b1100;

    // delay timer
    TMR1H = 0xEF;
    TMR1L = 0xFF;       // set to increment `time` every 1/8th of a second
    TMR1IE = 1;         // enable interrupt for timer 1
    TMR1IP = 1;         // high priority
    T1CON = 0b10001011; // Use SOSC, no prescaler, Sync it, 16-bit r/w mode, enable.
    // pir/movement timer
    TMR3H = 0xEF;
    TMR3L = 0xFF;       // set to increment `time` every 1/8th of a second
    TMR3IE = 1;         // enable interrupt for timer 1
    TMR1IP = 1;         // high priority
    T3CON = 0b10001011; // Use SOSC, no prescaler, Sync it, 16-bit r/w mode, enable.
    return;
}

/*! \brief Sets a specific pwm channel to a certain duty cycle in %
 *
 *  Inputs: Channel - which PWM channel to use, duty - the duty cycle (`duty`/256).
 *  Channels 1 and 2 are the motor drivers (using the 2 ECCP modules in full bridge mode)
 *  Channel 3 is for the light (using normal CCP PWM module)
 *  Channel 4,5,6 are for the LCD Backlight (Using PWM modules)
 *
 *  Outputs: A single bit specifying whether or not the command succeded (1 = success)
 *
 *
 * DutyCycle = ((CCPR4L:CCP4CON<5:4>) * TOSC * (TMR2 Prescale Value)
 * DutyCycle = Value * 64000000/ 4 / 16
 * Value = 1000000/dutyCycle
 */
unsigned pwm_set(unsigned char channel, unsigned int duty) // Set pin to duty cycle
{
    if(duty > 256) duty = 256;
    duty = 4 * duty; // ten bytes is 1024. with 256 sections, we
    if(duty != 0) --duty; // since 1024 is 0x100_0000_0000, we need to knock it down one

    switch(channel)
    {
        case 0:
        case 1:
        case 2:
            return true; // error
        case 3: // light - ccp9 - ON RP1/RA1
            CCPR9L = duty & 0x00FF;
            CCP9CONbits.DC9B = duty >> 8;
            break;
        case 4: // red - ccp5
            CCPR5L = duty & 0x00FF;
            CCP5CONbits.DC5B = duty >> 8;
            break;
        case 5: // green - ccp6
            CCPR6L = duty & 0x00FF;
            CCP6CONbits.DC6B = duty >> 8;
            break;
        case 6: // blue - ccp7
            CCPR7L = duty & 0x00FF;
            CCP7CONbits.DC7B = duty >> 8;
    }
    return false; // no error
}

/*! \brief Takes a packet and transmits it according to its contents. Returns true if successful.
 *
 *  Inputs: a i2c transmission packet (consists of address and array of data)
 *
 *  Outputs: a single bit specifying failure or success
 *
 *  Description: This function takes the packet and transmits the `packet.addr` byte first.
 *  It then transmits `packet.dataLength` bytes from the `packet.data` array.
 *  If at any time a byte is not acknowleged, the function will return 0 (false). 
 */
unsigned i2c_tx(unsigned char addr, unsigned char reg16, unsigned char regl, unsigned char regh, unsigned char *data, unsigned int dataLength)
{
    unsigned int i = 0;
    if(dataLength == 0) return true; // Do nothing if there is a zero data length.
    i2c_start();
    i2c_send(addr & 0b11111110); // write mode; address
    if(reg16 > 0) i2c_send(regh); // Address High Byte
    i2c_send(regl); // Address Low Byte
    for(i = 0; i < dataLength; ++i)
    {
        i2c_send(data[i]);
    }
    i2c_stop(); // generate stop bit
    /// \todo TODO: Get the ack bit and return that from the address at least.
    return false;
}

/*! \brief Checks to see if there is something at a certain address. Returns the acknowledge bit (0=ack)
 *
 * Inputs: The address of the device on the i2c bus
 */
unsigned i2c_check(unsigned char addr)
{
    i2c_start();
    i2c_send(addr | 1); // We will use the read mode address and then stop.
    i2c_stop();
    return SSP1CON2bits.ACKSTAT;
}

/*! \brief Gets Data from a slave at a specific address. Returns a char array pointer.
 *
 * Inputs: `packet`: A packet consisting of an address, a number for quantity, and an array to fill with the quantity.
 * Outputs: None formally. Modifies the `data` portion of the packet pointer that was given.
 */
unsigned i2c_rx(unsigned char addr, unsigned char reg16, unsigned char regl, unsigned char regh, unsigned char *data, unsigned int dataLength) // recieve data from address
{
    unsigned char i = 0;
    if(dataLength == 0) return true; // Do nothing if there is a zero data length.
    
    // write address
    i2c_start();
    i2c_send(addr & 0b11111110); // write mode at address
    if(reg16) i2c_send(regh); // Address High Byte
    i2c_send(regl); // Address Low Byte
    i2c_restart(); // exit write mode
    i2c_send(addr | 0b00000001); // read mode at address
    do
    {
        data[i] = i2c_recv(0); // Ack
    }
    while (++i < dataLength);
    data[i] = i2c_recv(1); // NOAck
    i2c_stop();
    return false;
}

/// Set up the i2c1 module
void i2c_setup() // Initialize the I2C pins
{
    SSP1CON1 = 0b00101000; // Enable MSSP in master i2c mode. Auto-configs the pins.
    //SSP1ADD  = 0x4F; // for 100kHz Clock at 32MHz primary clock.
    SSP1ADD  = 0x7F;
}

//! \brief Transmit a start bit on the i2c bus
static void i2c_start()
{
        SSP1CON2bits.SEN = 1; // do a start bit.
        while(SSP1CON2bits.SEN) continue; // Wait until SSP1IF is set
}
static void i2c_restart()
{
    SSP1CON2bits.RSEN = 1; // do a repeat start bit.
    while(SSP1CON2bits.RSEN) continue;
}

//! \brief Transmit a stop bit on the i2c bus
static void i2c_stop()
{
    SSP1CON2bits.PEN = 1;
    while(SSP1CON2bits.PEN) continue;
}

static void i2c_ack()
{
    SSP1CON2bits.ACKDT = 0;
    SSP1CON2bits.ACKEN = 1;
    while(SSP1CON2bits.ACKEN) continue;
}

static void i2c_nack()
{
    SSP1CON2bits.ACKDT = 1;
    SSP1CON2bits.ACKEN = 1;
    while(SSP1CON2bits.ACKEN) continue;
}

static void i2c_wait()
{
	while ((SSP1CON2 & 0x1F ) || ( SSP1STAT & 0x04 ) );
}

/*! \brief Transmit a specific byte onto I2C (Private function)
 *
 * Inputs: a byte to send.
 * Outputs: the ack bit or lack therof.
 */
static unsigned i2c_send(unsigned char byte)
{
    SSP1BUF = byte; // begin transmitting the byte
    while(SSP1STATbits.BF) continue; // wait for transmission to finish
    i2c_wait();
    return (SSP1CON2bits.ACKDT || SSP1CON2bits.ACKSTAT); // if not acknoleged or got NO-ACK
}

/*! \brief Recieve a byte from the I2C device
 */
static unsigned char i2c_recv(unsigned ack)
{
    SSP1CON2bits.RCEN = 1; // Enable recieve of bits.
    while(SSP1STATbits.BF) continue; // wait for recieving to finish
    if(ack == 1) i2c_ack();
    else i2c_nack();
    i2c_wait();
    return SSP1BUF;
}

void i2c_lcdInit()
{
    // this is from the LCD's datasheet.
    // What should I be doing with the R/W and RS bits?
    // Answer: These are virtual, but they should both be 0.
    i2c_start(); // send first and only start bit
    if(i2c_send(0x78) || i2c_send(0x00)) // Address and is a command
    {
        __delay_ms(10);
        return;
    }
    __delay_ms(10);

    if(i2c_send(0x38)) // second byte: N, DH, IS2, IS1
    {
        __delay_ms(10);
        return;
    }
    __delay_ms(10);

    if(i2c_send(0x39) | i2c_send(0x14) | i2c_send(0x78) | i2c_send(0x5E) |
            i2c_send(0x6D) |  i2c_send(0x0C) |  i2c_send(0x01) | i2c_send(0x06))
        // function set again, bias set, contrast set, power/icon/contrast control, follower control, display of/off control.
    {
        __delay_ms(10);
        return;
    }
    __delay_ms(10);
    i2c_stop();
    return;
}

/*! \brief delay(#): delay #/8 seconds
 *
 * To use the delay and timeout functions, take the time you want to wait in seconds
 * and divide it by 8. This is the number you put in (up to 65535 => 2.27 hours).
 * The delay function is simple to use, but blocks the code from running. The
 * timeout functions do not block. To use timeout functionality, use `timeoutInit()`
 * first, then call `timeoutCheck` like the delay. It will return false when the
 * timeout has been reached.
 */
void delay(unsigned int times)
{
    delayTime = 0;
    TMR1H = 0xF7;
    TMR1L = 0xFF;       // set to increment `time` every 1/8th of a second
    while(delayTime<times) continue;
}

/// Reset the timer variable and the timer counter so we can do a timeout.
void timeoutInit()
{
    // set the timer variable to zero and restart the timer
    time  = 0;
    TMR1H = 0xF7;
    TMR1L = 0xFF; // set to increment `time` every 1/8th of a second
}

/// Check if we have waited a certain ammount of time
unsigned timeoutCheck(unsigned int timeCheck)
{
    return time > timeCheck;
    // returns true if the time is up
}
