/*
 * delay.c
 *
 * Created: 2022-03-06 8:24:33 PM
 *  Author: Ty
 */ 

#include <tbd.h>
#include <sysclk.h>


/***************************************************************************************************
                                  PRIVATE DEFINITIONS AND GLOBALS
***************************************************************************************************/

#define US_PER_S	(1000000)

static uint32_t cpuClockFrequency = 0;


/***************************************************************************************************
                                             PUBLIC API
***************************************************************************************************/

void delayInit(void)
{
	cpuClockFrequency = sysclk_get_cpu_hz();
}


void delayFor(uint32_t us)
{	
	uint32_t clockCycles = (cpuClockFrequency / US_PER_S) * us;
	for (uint32_t i = 0; i < clockCycles; i++) {
		__asm__ __volatile__("");
	}
}