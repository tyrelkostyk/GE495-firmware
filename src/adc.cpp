// adc.c
// Contains functions for reading and processing signal data from the analog-digital converter.

#include "defs.h"

//#ifdef _ARDUINO
#include <Arduino.h>
//#endif  // _ARDUINO

#define ADC_BIT_MASK  ((int32_t)1)         // 1 bit per reading
#define ADC_DATA_MASK ((int32_t)0xFFFFFF)  // 24 bit readings per channel

/**
 * Polls the data pin of the ADC and returns its value
 */
uint8_t pollADCDataPin()
{
  return digitalRead(DATA_PIN) & ADC_BIT_MASK;
}

/**
 * Retrieves 24-bit ADC data.
 * @return int32_t 24-bit ADC data
 */
int32_t retrieveADCData()
{
  int32_t data = 0;
  // wait for data line to go low then high then low again
  while (pollADCDataPin() == 0){};
  while (pollADCDataPin() == 1){};

  for (int i=NUM_ADC_BITS - 1; i>=0; i--) {
    applySCLK();
    int32_t bit = pollADCDataPin();
    if (i == NUM_ADC_BITS - 1)
      data = data - ((bit << i) & ADC_DATA_MASK); // get signed output
    else
      data = data + ((bit << i) & ADC_DATA_MASK);
  }
  // one additional SCLK ensures that the DRDY/DOUT line stays high after data
  // is received
  applySCLK();

  return data;
}

/**
 * Retrieves 24-bit ADC data and calibrates the ADC.
 * @return int32_t 24-bit ADC data
 */
int32_t retrieveADCDataWithCal()
{
  int32_t data = retrieveADCData();
  applySCLK();
  return data;
}

/**
 * Take measurements from all three load cells and sums them
 * @return int32_t 24-bit ADC data summed from three load cells
 */
int32_t readThreeLoadCells()
{
  setADCMux(0);
  int32_t data0 = retrieveADCData() - dataOffset0;
  setADCMux(1);
  int32_t data1 = retrieveADCData() - dataOffset1;
  setADCMux(2);
  int32_t data2 = retrieveADCData() - dataOffset2;
  int32_t data_total = data0 + data1 + data2;
  return data_total;
}

/**
 * Take raw measurements from one load cell and averages them
 * @param mux The laod cell to get measurements from
 * @param N the amount of measurements to make
 * @return The average of N raw voltage measurements
 */
int32_t getNRawMeasurements(uint8_t mux, int32_t N)
{
  int64_t data_total = 0;
  setADCMux(mux);
  for (uint8_t i=0; i<N; i++)
  {
    data_total += retrieveADCData();
  }
  return data_total / N;
}

/**
 * Take mass measurements from all three load cells and averages them
 * @param N the amount of measurements to make
 * @return The average of N raw measurements
 */
int32_t getNMeasurements(int32_t N)
{
  int64_t data_total = 0;
  for (uint8_t i=0; i<N; i++)
  {
    data_total += readThreeLoadCells();
  }
  return data_total / N;
}

/**
 * Picks which input to retrieve data from
 * @param uint8_t muxSelect The input port to receive data from
 */
void setADCMux(uint8_t muxSelect)
{
  digitalWrite(MUX_PIN0, muxSelect & 1);
  digitalWrite(MUX_PIN1, muxSelect & 2);
}


/**
 * Runs the power up sequence that must be applied when the ADC is
 * initially powered on.
 */
void doADCPowerUpSequence()
{
  delayMicroseconds(100);
  digitalWrite(POWER_PIN, LOW);
  delayMicroseconds(100);
  digitalWrite(POWER_PIN, HIGH);
  delayMicroseconds(100);
  digitalWrite(POWER_PIN, LOW);
  delayMicroseconds(100);
  digitalWrite(POWER_PIN, HIGH);
}

/**
 * Holds the PWDN pin low to power down and reset the ADC
 */
void powerDownADC()
{
  delayMicroseconds(100);
  digitalWrite(POWER_PIN, LOW);
}

/**
 * Set the ADC samples per second
 * @param uint8_t sampleSpeed 0 = 10 samples/sec, 1 = 80 samples/sec
 */
void setADCSpeed(uint8_t sampleSpeed)
{
  digitalWrite(SPEED_PIN, sampleSpeed);
}

/**
 * Force the ADC into standby mode - reduces power consumption by shutting down
 * most of the circuitry. In standby mode, the entire analog circuitry is
 * powered down and only the clock source circuitry is awake to reduce the
 * wake-up time from the standby mode.
 * @param uint8_t calRequest True or false for an offset calibration to occur
 * within the ADC after exiting standby mode
 */
void enterADCStandbyMode(uint8_t calRequest)
{
  for (int i=0; i<=NUM_ADC_BITS; i++)
  {
    applySCLK();
  }
  if (calRequest) {
    // applying one more SCLK will re-calibrate the ADC
    applySCLK();
  }
  setSCLKHigh();
}

/**
 * Bring the ADC out of standby mode
 **/
void exitADCStandbyMode()
{
  setSCLKLow();
}
