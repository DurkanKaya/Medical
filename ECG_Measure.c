//DATE: March-2020
//Author:Durkan KAYA
#include "nrf.h"
#define LO1 	(12UL)
#define LO2 	(11UL)									

int deger2()
{
	volatile int16_t result = 0;
  volatile float precise_result = 0;
	
	NRF_CLOCK->TASKS_HFCLKSTART = 1;
  while (NRF_CLOCK->EVENTS_HFCLKSTARTED == 0);
  NRF_CLOCK->EVENTS_HFCLKSTARTED = 0;

	//LO1 PIN GPIO CONFIGURE
	NRF_GPIO->PIN_CNF[LO1] = (GPIO_PIN_CNF_DIR_Input << GPIO_PIN_CNF_DIR_Pos) |
                                   (GPIO_PIN_CNF_DRIVE_S0S1 << GPIO_PIN_CNF_DRIVE_Pos) |
                                   (GPIO_PIN_CNF_INPUT_Connect << GPIO_PIN_CNF_INPUT_Pos) |
                                   (GPIO_PIN_CNF_PULL_Pullup << GPIO_PIN_CNF_PULL_Pos) |
                                   (GPIO_PIN_CNF_SENSE_Disabled << GPIO_PIN_CNF_SENSE_Pos);
//LO2 PIN GPIO CONFIGURE
	NRF_GPIO->PIN_CNF[LO2] = (GPIO_PIN_CNF_DIR_Input << GPIO_PIN_CNF_DIR_Pos) |
                                   (GPIO_PIN_CNF_DRIVE_S0S1 << GPIO_PIN_CNF_DRIVE_Pos) |
                                   (GPIO_PIN_CNF_INPUT_Connect << GPIO_PIN_CNF_INPUT_Pos) |
                                   (GPIO_PIN_CNF_PULL_Pullup << GPIO_PIN_CNF_PULL_Pos) |
                                   (GPIO_PIN_CNF_SENSE_Disabled << GPIO_PIN_CNF_SENSE_Pos);

  //BEAT PIN GPIO CONFIGURE
  NRF_SAADC->CH[2].CONFIG = (SAADC_CH_CONFIG_GAIN_Gain1_6   << SAADC_CH_CONFIG_GAIN_Pos) |
                            (SAADC_CH_CONFIG_MODE_SE         << SAADC_CH_CONFIG_MODE_Pos) |
                            (SAADC_CH_CONFIG_REFSEL_Internal << SAADC_CH_CONFIG_REFSEL_Pos) |
                            (SAADC_CH_CONFIG_RESN_Bypass     << SAADC_CH_CONFIG_RESN_Pos) |
                            (SAADC_CH_CONFIG_RESP_Bypass     << SAADC_CH_CONFIG_RESP_Pos) |
                            (SAADC_CH_CONFIG_TACQ_3us        << SAADC_CH_CONFIG_TACQ_Pos);
																										
	uint32_t volatile input;
	input = (NRF_GPIO->IN >> LO1) & 1UL;
	uint32_t volatile input1;
	input1 = (NRF_GPIO->IN >> LO2) & 1UL;
	
	if(input && input1)
	{
		while(0){}
	}
	else{
		
  NRF_SAADC->CH[2].PSELP = SAADC_CH_PSELP_PSELP_VDD << SAADC_CH_PSELP_PSELP_Pos;
  NRF_SAADC->CH[2].PSELN = SAADC_CH_PSELN_PSELN_NC << SAADC_CH_PSELN_PSELN_Pos;
	NRF_SAADC->RESOLUTION = SAADC_RESOLUTION_VAL_14bit << SAADC_RESOLUTION_VAL_Pos;

	NRF_SAADC->RESULT.MAXCNT = 1;
  NRF_SAADC->RESULT.PTR = (uint32_t)&result;

  NRF_SAADC->SAMPLERATE = SAADC_SAMPLERATE_MODE_Task << SAADC_SAMPLERATE_MODE_Pos;
  NRF_SAADC->ENABLE = SAADC_ENABLE_ENABLE_Enabled << SAADC_ENABLE_ENABLE_Pos;
	
	NRF_SAADC->TASKS_CALIBRATEOFFSET = 1;
  while (NRF_SAADC->EVENTS_CALIBRATEDONE == 0);
  NRF_SAADC->EVENTS_CALIBRATEDONE = 0;
  while (NRF_SAADC->STATUS == (SAADC_STATUS_STATUS_Busy <<SAADC_STATUS_STATUS_Pos));
	
	NRF_SAADC->TASKS_START = 1;
  while (NRF_SAADC->EVENTS_STARTED == 0);
  NRF_SAADC->EVENTS_STARTED = 0;
	
	NRF_SAADC->TASKS_SAMPLE = 1;
  while (NRF_SAADC->EVENTS_END == 0);
  NRF_SAADC->EVENTS_END = 0;
	
	precise_result = (float)result / 4551.1f;
  precise_result;
	
	NRF_SAADC->TASKS_STOP = 1;
  while (NRF_SAADC->EVENTS_STOPPED == 0);
  NRF_SAADC->EVENTS_STOPPED = 0;

	}
	
	while(1)
	{
	
		__WFE();
	}
}
