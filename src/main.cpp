#include <avr/wdt.h>
#include <Arduino.h>
#include <PinFunctions.h>

#define FAN_MIN_RPM 500
#define ULONG_MAX

volatile bool isEmulation = true;
volatile uint16_t fanRpm = 0;
volatile uint8_t edgeCount = 0;
volatile unsigned long lastEdgeTime = 0;
volatile uint8_t stopCount = 0;
unsigned long lastStopTime = 0;
uint16_t pulseWidth = 0;
uint8_t pulse = 0;
unsigned long lastPulseTime = 0;

void isrPin()
{
  stopCount = 0;
  uint8_t pinState = getInputState();

  if (!isEmulation)
  {
    if (pinState)
      setInputHigh();
    else
      setInputLow();
  }

  if (pinState)
    return;

  if (edgeCount == 0)
  {
    edgeCount++;
    lastEdgeTime = micros();
  }
  else if (edgeCount == 1)
  {
    edgeCount++;
  }
  else
  {
    edgeCount = 0;
    unsigned long time = micros();
    unsigned long duration = time > lastEdgeTime
                                 ? time - lastEdgeTime
                                 : (ULONG_MAX - lastEdgeTime + time);
    fanRpm = 1000000 / duration * 60;
    isEmulation = fanRpm < FAN_MIN_RPM;
  }
}

void setup()
{
  wdt_enable(WDTO_1S);

  pinMode(PIN_INPUT, INPUT);
  pinMode(PIN_OUTPUT, INPUT);

  attachInterrupt(digitalPinToInterrupt(PIN_INPUT), isrPin, CHANGE);
}

void loop()
{
  unsigned long t = micros();

  wdt_reset();

  if (isEmulation && t - lastPulseTime >= pulseWidth)
  {
    lastPulseTime = t;
    pulseWidth = 60000000 / FAN_MIN_RPM / 4;
    if (pulse == 1)
    {
      setInputHigh();
      pulse = 0;
    }
    else
    {
      setInputLow();
      pulse = 1;
    }
  }

  if (t - lastStopTime > 100000 || t < lastStopTime)
  {
    lastStopTime = t;
    stopCount++;
  }

  if (stopCount >= 5)
  {
    isEmulation = false;
    fanRpm = 0;
    edgeCount = 0;
    stopCount = 0;
  }
}