#ifndef SOLARLIGHT_H
#define SOLARLIGHT_H

// Functions
// void main() is in the associated C file
void setup();
void loop();
void interrupt low_priority isr_low();
void interrupt high_priority isr_high();

#endif