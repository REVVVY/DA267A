#ifndef PINS_H_
#define PINS_H_
#include <stdint.h>

/* initialise the 12th pin */

void initPin(); /* switches LED on if level!=0 or off if level==0*/

void setLED(uint8_t level); /* switches LED on if level!=0 or off if level==0*/

#endif