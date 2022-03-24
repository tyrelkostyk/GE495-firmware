// i2c.c
// Contains functions for retrieving raw data from the analog-digital converter via TWI.

#include "defs.h"

#include <Arduino.h>

/**
 * Applies one SCLK cycle
 */
void applySCLK()
{
  digitalWrite(CLOCK_PIN, HIGH);
  delayMicroseconds(1);
  digitalWrite(CLOCK_PIN, LOW);
  delayMicroseconds(1);
}

/**
 * Sets SCLK high
 */
void setSCLKHigh()
{
  digitalWrite(CLOCK_PIN, HIGH);
}

/**
 * Sets SCLK low
 */
void setSCLKLow()
{
  digitalWrite(CLOCK_PIN, LOW);
}
