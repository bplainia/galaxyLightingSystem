#include <shared.h>
#include <xc.h>
#include <solarLight.h>

// TODO: need config bits and the xc8 header

void main(void)
{
    // A very super-simple main function by Ben. Makes this coding similar
    // to programming an arduino. Ain't that nice? So don't put anything
    // else here now. This is a finished function.
    setup();
    while(1) loop();
} 

void setup()
{
    // Put initialization functions here.
    usb_start();
}

void loop()
{
    // Put things that you need to process here. Dont' spend too much time
    // in your process. Others want to do stuff too...
}

void interrupt low_priority isr_low()
{
   ;
}

void interrupt high_priority isr_high()
{
    ;
}