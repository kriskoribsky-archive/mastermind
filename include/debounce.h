#ifndef _DEBOUNCE_H
#define _DEBOUNCE_H

#include <stdbool.h>
#include <stdint.h>

// custom debounce module for multiple
// simultaneous pin readings

// debounce reference: 
// https://docs.arduino.cc/built-in-examples/digital/Debounce


uint8_t debounce_read(uint8_t pin);


#endif
    