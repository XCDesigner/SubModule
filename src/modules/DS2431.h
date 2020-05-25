#ifndef _ONEWIREEEPROM_H_
#define _ONEWIREEEPROM_H_

#include "stdint.h"

class OneWireEEPROM {
public:
  static void Init(void);
  static uint32_t ReadTime();
  static bool WriteTime(uint32_t Counter);
private:
  static void SkipRomMach(void);
  static bool ResetBus(void);
  static void WriteOneByte(uint8_t data);
  static uint8_t ReadOneByte(void);
  static void WriteBitHigh();
  static void WriteBitLow();
  static bool ReadBit();

private:
  static bool chip_exist;
};

OneWireEEPROM ds2431;

#endif