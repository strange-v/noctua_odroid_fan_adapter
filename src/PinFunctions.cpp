#include <PinFunctions.h>

uint8_t getInputState()
{
  return PINB & (1 << PIN_INPUT);
}

void setInputLow()
{
  DDRB |= (1 << PIN_OUTPUT);   //mode OUTPUT
  PORTB &= ~(1 << PIN_OUTPUT); //state LOW
}

void setInputHigh()
{
  DDRB &= ~(1 << PIN_OUTPUT); //mode INPUT
}