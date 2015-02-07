// Main file
// Created by benjamin Plain

#include <solarLight.h>

// TODO: need config bits and the xc8 header

void main(void)
{
    // A very super-simple main function by Ben. Makes this coding similar
    // to programming an arduino. Ain't that nice? So don't put anything
    // else here now. This is a finished function.
    INTCONbits.GIEH = 1; // enable interrupts
    INTCONbits.GIEL = 1;
    INTCONbits.PEIE = 1; // enable priority interrupts
    setup();             // setup everything
    INTCONbits.GIEH = 1; // enable interrupts
    INTCONbits.GIEL = 1;
    while(1) loop();     // execute loop forever
}


void setup() // Initialize stuff here without any interruption
{
    // Variables
    unsigned char status;

    mem_start(); // First priority is to check the memory
    // TODO: Check to see if the chip started after a POR, BOR, or is from VBATT
    status = mem_check(); // checks where it started up from, and if memory is ok
    hid_start();
    if(status < VBATT) rtc_start(); // if from POR, then nothing has been started, so start rtc
}

void loop()
{
    // Put things that you need to process here. Dont' spend too much time
    // in your process. Others want to do stuff too...
    ;
}

void interrupt low_priority isr_low()
{
   ;
}

void interrupt high_priority isr_high()
{
    ;
}