// adc.c
// Contains functions for reading and processing signal data from the analog-digital converter.

#include <tbd.h>


/***************************************************************************************************
                                  PRIVATE DEFINITIONS AND GLOBALS
***************************************************************************************************/

#define ADC_SMOOTH_SAMPLE_SIZE			5

#define ADC_RESET_PWDN_DELAY_MS			100
#define ADC_OFFSET_CALIBRATION_DELAY_MS	802

#define ADC_POWER_ON_SEQUENCE_DELAY_US	100

#define ADC_READ_SIZE_BITS	24


typedef enum _adcSpeed_t {
	adcSpeedSlow = 0,
	adcSpeedFast = 1,
} adcSpeed_t;

typedef enum _adcChannel_t {
	adcChannelZero	= 0,
	adcChannelOne	= 1,
	adcChannelTwo	= 2,
	adcChannelThree = 3,
} adcChannel_t;

typedef enum _adcGain_t {
	adcGain1	= 0,
	adcGain2	= 1,
	adcGain64	= 2,
	adcGain128	= 3,
} adcGain_t;


// local globals 
uint8_t mux;

int32_t dataOffset0 = 0;
int32_t dataOffset1 = 0;
int32_t dataOffset2 = 0;

double voltageToMassFactor = 1;
double mass1;
double mass2;
int32_t voltage1;
int32_t voltage2;

uint8_t offsetFlag = 0;

uint32_t prevUpdateTime;


/***************************************************************************************************
                                       PRIVATE FUNCTION STUBS
***************************************************************************************************/

static void adcApplySclk(void);
static uint8_t adcReadBit(void);
static int32_t adcReadChannel(void)
static int32_t adcReadAllChannels(void);
static int32_t adcReadSmooth(void);
static int32_t adcReadAndCalibrate(void);

static void adcSelectChannel(adcChannel_t channel);
static void adcSetSpeed(adcSpeed_t sampleSpeed);

static void adcPowerOn(void);
static void adcReset(void);


/***************************************************************************************************
                                             PUBLIC API
***************************************************************************************************/

void adcInit()
{

	// TODO: initialize digital pins used to control the ADC
		
	// initialize ADC Speed
    adcSetSpeed(adcSpeedSlow);

	// initialize ADC Gain
	adcSetGain(adcGain128);
	
	// initialize ADC MUX
	adcSelectChannel(adcChannelZero);

	// power on the ADC
    adcPowerOn();
	
    //delay(300);
    
	//tareAllLoadCells();
    
	//Serial.println("Input mass 1:");
    //while (Serial.available() == 0){}
    //getCalMass1(Serial.parseInt());
    //while (Serial.available() == 1){}

    //Serial.println("Input mass 2:");
    //while (Serial.available() == 0){}
    //getCalMass2(Serial.parseInt());

    //getVoltageToMassFactor(mass1, voltage1, mass2, voltage2);
}


/***************************************************************************************************
                                          PRIVATE FUNCTIONS
***************************************************************************************************/

static void adcApplySclk(void)
{
	// TODO: set SCLK (pin PA3) high (for the rising edge)

}


/**
 * Polls the data pin of the ADC and returns its value
 */
static uint8_t adcReadBit(void)
{
	//return digitalRead(DATA_PIN);
  
	// set SCLK high
	adcApplySclk();

	// TODO: read a single bit from the ADC
	
}


/**
 * Retrieves 24-bit ADC data. 
 * @return int32_t 24-bit ADC data
 */
static int32_t adcReadChannel(void)
{
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
static int32_t adcReadAndCalibrate(void)
{
  int32_t data = adcReadChannel();
  adcApplySclk();
  
  // TODO: delay?
  
  return data;
}


/**
 * Take measurements from all three load cells and sums them
 * @return int32_t 24-bit ADC data summed from three load cells
 */
static int32_t adcReadAllChannels(void)
{
  adcSelectChannel(0);
  int32_t data0 = adcReadChannel() - dataOffset0;
  adcSelectChannel(1);
  int32_t data1 = adcReadChannel() - dataOffset1;
  adcSelectChannel(2);
  int32_t data2 = adcReadChannel() - dataOffset2;
  int32_t data_total = data0 + data1 + data2;
  return data_total;
}


/**
 * Take mass measurements from all three load cells and averages them
 * @param N the amount of measurements to make
 * @return The average of N raw measurements
 */
static int32_t adcReadSmooth(void)
{
	uint32_t data_total = 0;
	for (uint8_t i = 0; i < ADC_SMOOTH_SAMPLE_SIZE; i++)
	{
		data_total += adcReadAllChannels();
	}

	return data_total / ADC_SMOOTH_SAMPLE_SIZE;
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
		// TODO: write to MUX pin 0		
		// TODO: write to MUX pin 1
		break;

	case (adcChannelZero):
		// TODO: write to MUX pin 0
		// TODO: write to MUX pin 1
		break;
	
	case (adcChannelZero):
		// TODO: write to MUX pin 0
		// TODO: write to MUX pin 1
		break;
	
	case (adcChannelZero):
		// TODO: write to MUX pin 0
		// TODO: write to MUX pin 1
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
  //delayMicroseconds(ADC_POWER_ON_SEQUENCE_DELAY_US);
  //digitalWrite(POWER_PIN, LOW);
  //delayMicroseconds(ADC_POWER_ON_SEQUENCE_DELAY_US);
  //digitalWrite(POWER_PIN, HIGH);
  //delayMicroseconds(ADC_POWER_ON_SEQUENCE_DELAY_US);
  //digitalWrite(POWER_PIN, LOW);
  //delayMicroseconds(ADC_POWER_ON_SEQUENCE_DELAY_US);
  //digitalWrite(POWER_PIN, HIGH);
  
	// TODO: set PWDN pin low
  
	// TODO: delay
  
	// TODO: set PWDN pin high
  
	// TODO: delay
  
	// TODO: set PWDN pin low

    // TODO: delay
    
    // TODO: set PWDN pin high

}


/**
 * Holds the PWDN pin low to power down and reset the ADC
 */
static void adcReset(void)
{

  // TODO: set the PWDN pin low
  
  // TODO: delay
  
  // TODO: release the PWDN pin
  
}


/**
 * Set the ADC samples per second
 * @param sampleSpeed 0 = 10 samples/sec, 1 = 80 samples/sec
 */
static void adcSetSpeed(adcSpeed_t sampleSpeed)
{
  //digitalWrite(SPEED_PIN, sampleSpeed);

	// TODO: write (and hold) the speed pin
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
		// TODO: write to GAIN pin 0
		// TODO: write to GAIN pin 1
		break;

	case (adcGain2):
		// TODO: write to GAIN pin 0
		// TODO: write to GAIN pin 1
		break;
		
	case (adcGain64):
		// TODO: write to GAIN pin 0
		// TODO: write to GAIN pin 1
		break;
		
	case (adcGain128):
		// TODO: write to GAIN pin 0
		// TODO: write to GAIN pin 1
		break;
		
	default:
		break;
	}
}
