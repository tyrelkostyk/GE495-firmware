// adc.c
// Contains functions for reading and processing signal data from the analog-digital converter.

#include <tbd.h>
#include <pio.h>
#include <pmc.h>


/***************************************************************************************************
                                  PRIVATE DEFINITIONS AND GLOBALS
***************************************************************************************************/

#define ADC_SMOOTH_SAMPLE_SIZE			5

#define ADC_RESET_PWDN_DELAY_MS			100
#define ADC_OFFSET_CALIBRATION_DELAY_MS	802

#define ADC_SCLK_EDGE_DELAY_US			1
#define ADC_POWER_ON_SEQUENCE_DELAY_US	100

#define ADC_READ_SIZE_BITS	24


#if (BOARD==SAM4E_XPLAINED_PRO)

#define ADC_RETRIEVAL_PIO_ID	ID_PIOA
#define ADC_CONTROL_PIO_ID		ID_PIOE

#define ADC_DATA_PIN_BANK		PIOA
#define ADC_DATA_PIN			PIO_PA3		// EXT1 PIN 11
#define ADC_SCLK_PIN_BANK		PIOA
#define ADC_SCLK_PIN			PIO_PA4		// EXT1 PIN 12

#define ADC_PWDN_PIN_BANK		PIOA
#define ADC_PWDN_PIN			PIO_PA24	// EXT1 (PE0 used by LCD)
#define ADC_MUX0_PIN_BANK		PIOE
#define ADC_MUX0_PIN			PIO_PE1		// EXT3 PIN9
#define ADC_MUX1_PIN_BANK		PIOE
#define ADC_MUX1_PIN			PIO_PE2		// EXT2 PIN5
#define ADC_SPEED_PIN_BANK		PIOA
#define ADC_SPEED_PIN			PIO_PA25	// EXT1 (PE3 used by LCD)
#define ADC_GAIN0_PIN_BANK		PIOA
#define ADC_GAIN0_PIN			PIO_PA15	// EXT1 (PE5 used by LCD)
#define ADC_GAIN1_PIN_BANK		PIOA
#define ADC_GAIN1_PIN			PIO_PA16	// EXT1 (PE4 used by LCD)

#else // (BOARD==SAM4E_XPLAINED_PRO)

#define ADC_RETRIEVAL_PIO_ID	ID_PIOA
#define ADC_CONTROL_PIO_ID		ID_PIOE

#define ADC_DATA_PIN_BANK		PIOA
#define ADC_DATA_PIN			PIO_PA3
#define ADC_SCLK_PIN_BANK		PIOA
#define ADC_SCLK_PIN			PIO_PA4

#define ADC_PWDN_PIN_BANK		PIOE
#define ADC_PWDN_PIN			PIO_PE0
#define ADC_MUX0_PIN_BANK		PIOE
#define ADC_MUX0_PIN			PIO_PE1
#define ADC_MUX1_PIN_BANK		PIOE
#define ADC_MUX1_PIN			PIO_PE2
#define ADC_SPEED_PIN_BANK		PIOE
#define ADC_SPEED_PIN			PIO_PE3
#define ADC_GAIN0_PIN_BANK		PIOE
#define ADC_GAIN0_PIN			PIO_PE5
#define ADC_GAIN1_PIN_BANK		PIOE
#define ADC_GAIN1_PIN			PIO_PE4

#endif // (BOARD==SAM4E_XPLAINED_PRO)


typedef enum _adcSpeed_t {
	adcSpeedSlow = 0,
	adcSpeedFast = 1,
} adcSpeed_t;

typedef enum _adcGain_t {
	adcGain1	= 0,
	adcGain2	= 1,
	adcGain64	= 2,
	adcGain128	= 3,
} adcGain_t;


/***************************************************************************************************
                                       PRIVATE FUNCTION STUBS
***************************************************************************************************/

static void adcInitPins(void);
static void adcApplySclk(void);
static uint8_t adcReadBit(void);
static int32_t adcReadChannel(adcChannel_t channel);
static int32_t adcReadAndCalibrate(adcChannel_t channel);

static void adcSelectChannel(adcChannel_t channel);
static void adcSetSpeed(adcSpeed_t sampleSpeed);
static void adcSetGain(adcGain_t gain);

static void adcPowerOn(void);
static void adcReset(void);


/***************************************************************************************************
                                             PUBLIC API
***************************************************************************************************/

void adcInit(void)
{
	// initialize the digital pins used to control the ADC
	adcInitPins();

	// initialize ADC Speed
  adcSetSpeed(adcSpeedSlow);

	// initialize ADC Gain
	adcSetGain(adcGain128);

	// initialize ADC MUX
	adcSelectChannel(adcChannelZero);

	// power on the ADC
  adcPowerOn();
}


/**
 * Take mass measurements from all three load cells and averages them
 * @return The average of the raw measurements
 */
int32_t adcReadAllSmooth(void)
{
	uint32_t dataTotal = 0;
	for (uint8_t i = 0; i < ADC_SMOOTH_SAMPLE_SIZE; i++)
	{
		dataTotal += adcReadAllChannels();
	}

	return dataTotal / ADC_SMOOTH_SAMPLE_SIZE;
}


/**
 * Take raw measurements from one load cell and averages them
 * @param mux The load cell to get measurements from
 * @param N the amount of measurements to make
 * @return The average of N raw voltage measurements
 */
int32_t adcReadChannelSmooth(adcChannel_t channel)
{
	int32_t dataTotal = 0;

	for (uint8_t i=0; i < ADC_SMOOTH_SAMPLE_SIZE; i++)
	{
		dataTotal += adcReadChannel(channel);
	}

	return dataTotal / ADC_SMOOTH_SAMPLE_SIZE;
}



/***************************************************************************************************
                                          PRIVATE FUNCTIONS
***************************************************************************************************/

static void adcInitPins(void)
{
	// Enable the clocks for these PIO banks
	pmc_enable_periph_clk(ADC_RETRIEVAL_PIO_ID);
	pmc_enable_periph_clk(ADC_CONTROL_PIO_ID);

	// Data
	pio_set_input(ADC_DATA_PIN_BANK, ADC_DATA_PIN, PIO_PULLUP);

	// Clock
	pio_set_output(ADC_SCLK_PIN_BANK, ADC_SCLK_PIN, LOW, DISABLE, DISABLE);

	// PWDN
	pio_set_output(ADC_PWDN_PIN_BANK, ADC_PWDN_PIN, LOW, DISABLE, DISABLE);

	// Channel MUX
	pio_set_output(ADC_MUX0_PIN_BANK, ADC_MUX0_PIN, LOW, DISABLE, DISABLE);
	pio_set_output(ADC_MUX1_PIN_BANK, ADC_MUX1_PIN, LOW, DISABLE, DISABLE);

	// Speed
	pio_set_output(ADC_SPEED_PIN_BANK, ADC_SPEED_PIN, LOW, DISABLE, DISABLE);

	// Gain
	pio_set_output(ADC_GAIN0_PIN_BANK, ADC_GAIN0_PIN, LOW, DISABLE, DISABLE);
	pio_set_output(ADC_GAIN1_PIN_BANK, ADC_GAIN1_PIN, LOW, DISABLE, DISABLE);

}


static void adcApplySclk(void)
{
	pio_set(ADC_SCLK_PIN_BANK, ADC_SCLK_PIN);
	delayFor(ADC_SCLK_EDGE_DELAY_US);
	pio_clear(ADC_SCLK_PIN_BANK, ADC_SCLK_PIN);
	delayFor(ADC_SCLK_EDGE_DELAY_US);
}


/**
 * Polls the data pin of the ADC and returns its value
 */
static uint8_t adcReadBit(void)
{
	uint8_t bit = 0;

	// Apply SCLK rising edge
	adcApplySclk();

	bit = pio_get(ADC_DATA_PIN_BANK, PIO_TYPE_PIO_INPUT, ADC_DATA_PIN);

	return bit;
}


/**
 * Retrieves 24-bit ADC data.
 * @return int32_t 24-bit ADC data
 */
static int32_t adcReadChannel(adcChannel_t channel)
{
	adcSelectChannel(channel);

	// TODO: delay ?

	int32_t data = 0;
	// wait for data line to go low then high then low again
	while (adcReadBit() == 0){};
	while (adcReadBit() == 1){};

	for (int i = ADC_READ_SIZE_BITS - 1; i>=0; i--) {
		int32_t bit = adcReadBit();
		if (i == ADC_READ_SIZE_BITS - 1)
			data = data - (bit << i); // get signed output
		else
			data = data + (bit << i);
	}
	// one additional SCLK ensures that the DRDY/DOUT line stays high after data
	// is received
	adcApplySclk();

	return data;
}


/**
 * Retrieves 24-bit ADC data and calibrates the ADC.
 * @return int32_t 24-bit ADC data
 */
static int32_t adcReadAndCalibrate(adcChannel_t channel)
{
	int32_t data = adcReadChannel(channel);
	adcApplySclk();

	// TODO: delay?

	return data;
}


/**
 * Take measurements from all three load cells and sums them
 * @return int32_t 24-bit ADC data summed from three load cells
 */
int32_t adcReadAllChannels(void)
{
	adcSelectChannel(adcChannelZero);
	int32_t data0 = adcReadChannel(adcChannelZero) - calibrationOffset(adcChannelZero);

	adcSelectChannel(adcChannelOne);
	int32_t data1 = adcReadChannel(adcChannelOne) - calibrationOffset(adcChannelOne);

	adcSelectChannel(adcChannelTwo);
	int32_t data2 = adcReadChannel(adcChannelTwo) - calibrationOffset(adcChannelTwo);

	int32_t dataTotal = data0 + data1 + data2;

	return dataTotal;
}


/**
 * Picks which input to retrieve data from
 * @param uint8_t muxSelect The input port to receive data from
 */
static void adcSelectChannel(adcChannel_t channel)
{
	//digitalWrite(MUX_PIN0, muxSelect & 1);
	//digitalWrite(MUX_PIN1, muxSelect & 2);

	switch (channel)
	{
	case (adcChannelZero):
		pio_clear(ADC_MUX0_PIN_BANK, ADC_MUX0_PIN);
		pio_clear(ADC_MUX1_PIN_BANK, ADC_MUX1_PIN);
		break;

	case (adcChannelOne):
		pio_set(ADC_MUX0_PIN_BANK, ADC_MUX0_PIN);
		pio_clear(ADC_MUX1_PIN_BANK, ADC_MUX1_PIN);
		break;

	case (adcChannelTwo):
		pio_clear(ADC_MUX0_PIN_BANK, ADC_MUX0_PIN);
		pio_set(ADC_MUX1_PIN_BANK, ADC_MUX1_PIN);
		break;

	case (adcChannelThree):
		pio_set(ADC_MUX0_PIN_BANK, ADC_MUX0_PIN);
		pio_set(ADC_MUX1_PIN_BANK, ADC_MUX1_PIN);
		break;

	default:
		break;
	}
}


/**
 * Runs the power up sequence that must be applied when the ADC is
 * initially powered on.
 */
static void adcPowerOn(void)
{
	pio_clear(ADC_PWDN_PIN_BANK, ADC_PWDN_PIN);
	delayFor(ADC_POWER_ON_SEQUENCE_DELAY_US);
	pio_set(ADC_PWDN_PIN_BANK, ADC_PWDN_PIN);
	delayFor(ADC_POWER_ON_SEQUENCE_DELAY_US);
	pio_clear(ADC_PWDN_PIN_BANK, ADC_PWDN_PIN);
	delayFor(ADC_POWER_ON_SEQUENCE_DELAY_US);
	pio_set(ADC_PWDN_PIN_BANK, ADC_PWDN_PIN);
}


/**
 * Holds the PWDN pin low to power down and reset the ADC
 */
static void adcReset(void)
{
	pio_clear(ADC_PWDN_PIN_BANK, ADC_PWDN_PIN);

	delayFor(ADC_POWER_ON_SEQUENCE_DELAY_US);

  adcPowerOn();
}


/**
 * Set the ADC samples per second
 * @param sampleSpeed 0 (slow) = 10 samples/sec, 1 (fast) = 80 samples/sec
 */
static void adcSetSpeed(adcSpeed_t sampleSpeed)
{
	if (sampleSpeed == adcSpeedFast)
	{
		pio_set(ADC_SPEED_PIN_BANK, ADC_SPEED_PIN);
	}
	else
	{
		pio_clear(ADC_SPEED_PIN_BANK, ADC_SPEED_PIN);
	}
}


/**
 * Set the Gain of the ADCs internal PGA.
 * @param gain The internal gain factor.
 */
static void adcSetGain(adcGain_t gain)
{
  //digitalWrite(SPEED_PIN, sampleSpeed);

	switch (gain)
	{
	case (adcGain1):
		pio_clear(ADC_GAIN0_PIN_BANK, ADC_GAIN0_PIN);
		pio_clear(ADC_GAIN1_PIN_BANK, ADC_GAIN1_PIN);
		break;

	case (adcGain2):
		pio_set(ADC_GAIN0_PIN_BANK, ADC_GAIN0_PIN);
		pio_clear(ADC_GAIN1_PIN_BANK, ADC_GAIN1_PIN);
		break;

	case (adcGain64):
		pio_clear(ADC_GAIN0_PIN_BANK, ADC_GAIN0_PIN);
		pio_set(ADC_GAIN1_PIN_BANK, ADC_GAIN1_PIN);
		break;

	case (adcGain128):
		pio_set(ADC_GAIN0_PIN_BANK, ADC_GAIN0_PIN);
		pio_set(ADC_GAIN1_PIN_BANK, ADC_GAIN1_PIN);
		break;

	default:
		break;
	}
}
