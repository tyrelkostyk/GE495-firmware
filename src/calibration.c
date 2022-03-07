// calibration.c
// Contains device weight calibration functions.

#include <tbd.h>


/***************************************************************************************************
                                  PRIVATE DEFINITIONS AND GLOBALS
***************************************************************************************************/

static int32_t channelOffset[LOAD_CELLS_PER_TANK] = { 0 };

static double voltageToMassFactor = 1;
static double mass1 = 0.0;
static double mass2 = 0.0;
static int32_t voltage1 = 0;
static int32_t voltage2 = 0;

static uint8_t offsetFlag = 0;


/***************************************************************************************************
                                             PUBLIC API
***************************************************************************************************/

/**
 * Get the current calibrated offset for a channel's voltage readings.
 * @param channel The channel to obtain the offset for.
 */
int32_t calibrationOffset(adcChannel_t channel)
{
	return channelOffset[channel];
}


/**
 * Set the current output of a load cell to zero.
 * @param channel Load cell channel to tare
 * @param offset The value to offset the load cell's output by
 */
void calibrationTare(adcChannel_t channel, int32_t offset)
{
	channelOffset[channel] = offset;
}


/**
 * Tare all load cell outputs to zero.
 */
void calibrationTareAllLoadCells(void)
{
	for (adcChannel_t channel = adcChannelZero; channel < LOAD_CELLS_PER_TANK; channel++)
	{
		int32_t data = adcReadChannelSmooth(channel);
		calibrationTare(channel, data);
	}
}


/**
 * Get the first mass and voltage value for calibration step one.
 * @param double mass The mass of the first point for calibration
 */
void calibrationObtainMassOne(double mass)
{
	mass1 = mass;
	voltage1 = adcReadAllChannels();
}


/**
 * Get the second mass and voltage value for calibration step two.
 * @param double mass The mass of the second point for calibration
 */
void calibrationObtainMassTwo(double mass)
{
	mass2 = mass;
	voltage2 = adcReadAllChannels();
}


/**
 * Create the factor to convert the measured voltage from a load cell to a mass value.
 * @param mass1 The mass value for the first point of reference for calibration
 * @param voltage1 The voltage value for the first point of reference for calibration
 * @param mass2 The mass value for the second point of reference for calibration
 * @param voltage2 The voltage value for the second point of reference for calibration
 */
void calibrationGetConversionFactor(double mass1, int32_t voltage1, double mass2, int32_t voltage2)
{
	voltageToMassFactor = (mass1 - mass2) / (voltage1 - voltage2);
}
