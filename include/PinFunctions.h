#ifndef PIN_FUNCTIONS_h
#define PIN_FUNCTIONS_h
#include <Arduino.h>

#define PIN_INPUT PB2
#define PIN_OUTPUT PB4


uint8_t getInputState();
void setInputLow();
void setInputHigh();

#endif