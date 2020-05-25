#ifndef _ADCPORT_H_
#define _ADCPORT_H_

#include "stdint.h"

#define TOTAL_CHANNELS  2
#define SAMPLE_DEEP     10

class ADCPort{
public:
  static void Init();
  static void Enable();
  static void Disable();
  static uint16_t GetAdcValue(uint8_t index) { return RawData[index]; }
  static void Filter();
private:
  static void ADCInit();
  static void ISRInit();
  static void DMAInit();
  static void TimerInit();
  
private:
  static uint16_t DMA_buffer[TOTAL_CHANNELS * SAMPLE_DEEP];
  static uint16_t RawData[TOTAL_CHANNELS];
};

extern ADCPort hw_adc_port;

#endif