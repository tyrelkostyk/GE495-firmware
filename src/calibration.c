// calibration.c
// Contains device weight calibration functions.

#include <tbd.h>


/***************************************************************************************************
                                  PRIVATE DEFINITIONS AND GLOBALS
***************************************************************************************************/

static int32_t channelOffset[LOAD_CELLS_PER_TANK] = { 0 };

static float voltageToMassFactor = 1;
static float mass1 = 0.0;
static float mass2 = 0.0;
static int32_t voltage1 = 0;
static int32_t voltage2 = 0;


/***************************************************************************************************
                                       PRIVATE FUNCTION STUBS
***************************************************************************************************/

static void calibrationUpdateConversionFactor(void);


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
		adcReadAndCalibrate(channel);
		int32_t data = adcReadChannelSmooth(channel);
		calibrationTare(channel, data);
	}
}


/**
 * Get the first mass and voltage value for calibration step one.
 * @param double mass The mass of the first point for calibration
 */
void calibrationObtainMassOne(float mass)
{
	mass1 = mass;
	voltage1 = adcReadAllSmooth();
}


/**
 * Get the second mass and voltage value for calibration step two.
 * @param double mass The mass of the second point for calibration
 */
void calibrationObtainMassTwo(float mass)
{
	mass2 = mass;
	voltage2 = adcReadAllSmooth();
	calibrationUpdateConversionFactor();
}


/**
 * Getter function for the voltage to mass slope factor.
 */
float calibrationConversionFactor(void)
{
	return voltageToMassFactor;
}


/***************************************************************************************************
                                         PRIVATE FUNCTIONS
***************************************************************************************************/

/**
 * Create the factor to convert the measured voltage from a load cell to a mass value.
 * @param mass1 The mass value for the first point of reference for calibration
 * @param voltage1 The voltage value for the first point of reference for calibration
 * @param mass2 The mass value for the second point of reference for calibration
 * @param voltage2 The voltage value for the second point of reference for calibration
 */
static void calibrationUpdateConversionFactor(void)
{
	// obtain the difference in reported mass (rise)
	float massDifference = mass1 - mass2;
		
	// obtain the difference in measured voltage (run)
	int32_t voltageDifference = voltage1 - voltage2;
	if (voltageDifference == 0)
		voltageDifference = 1;
	
	// calculate new voltage to mass scale (slope = rise/run)
	voltageToMassFactor = massDifference / voltageDifference;
	
	// ensure the slope factor is never negative
	//if (voltageToMassFactor < 0)
		//voltageToMassFactor = (-1) * voltageToMassFactor;
}
