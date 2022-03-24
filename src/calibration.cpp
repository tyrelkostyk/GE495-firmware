// calibration.c
// Contains device weight calibration functions.

#include "defs.h"

extern int32_t dataOffset[3];

extern double voltageToMassFactor;

extern double mass1;
extern int32_t voltage1;

extern double mass2;
extern int32_t voltage2;


/**
 * Set the current output of a load cell to zero.
 * @param uint8_t mux Load cell to tare
 * @param int32_t offset The value to offset the load cell's output by
 */
void tare(uint8_t mux, int32_t offset)
{
    if (mux >= NUM_LOAD_CELLS) return;
    dataOffset[mux] = offset;
}

/**
 * Tare all load cell outputs to zero.
 */
void tareAllLoadCells()
{
  for (int mux=0; mux<NUM_LOAD_CELLS; mux++)
  {
    int32_t data = getNRawMeasurements(mux, SAMPLE_SIZE);
    tare(mux, data);
  }
}

/**
 * Get the first mass and voltage value for calibration step one.
 * @param double mass The mass of the first point for calibration
 */
void calibrateMass1(double mass)
{
  mass1 = mass;
  voltage1 = readThreeLoadCells();
}

/**
 * Get the second mass and voltage value for calibration step two.
 * @param double mass The mass of the second point for calibration
 */
void calibrateMass2(double mass)
{
  mass2 = mass;
  voltage2 = readThreeLoadCells();
}

/**
 * Create the factor to convert the measured voltage from a load cell to a mass value.
 * @param mass1 The mass value for the first point of reference for calibration
 * @param voltage1 The voltage value for the first point of reference for calibration
 * @param mass2 The mass value for the second point of reference for calibration
 * @param voltage2 The voltage value for the second point of reference for calibration
 */
void calculateVoltageToMassFactor()
{
  double massDifference = mass1 - mass2;
  int32_t voltageDifference = voltage1 - voltage2;

  if (voltageDifference == 0)
    voltageDifference = 1;
  
  voltageToMassFactor = massDifference / voltageDifference;

  if (voltageToMassFactor < 0)
    voltageToMassFactor = voltageToMassFactor * -1;
}
