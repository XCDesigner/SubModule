#ifndef _LOCAL_EEPROM_H_
#define _LOCAL_EEPROM_H_

#include "stdint.h"

class LocalEEPROM {
public:
  static bool RestoreSetting(uint8_t *pData, uint32_t Len);
  static void StoreSetting(uint8_t *pData, uint32_t Len);
  static uint32_t GetModuleType();

private:
  static uint32_t ReadParameterValidFlag(uint8_t Option);
  static void WriteParameterValidFlag(uint8_t Option);
  static uint32_t ReadParameterRollingFlag(uint8_t Option);
  static void WriteParameterRollingFlag(uint8_t Option, uint32_t RollingValue);
  static void ReadParameter(uint8_t Option, uint8_t *pData, uint32_t Len);
  static void WriteParameter(uint8_t Option, uint8_t *pData, uint32_t Len);
  static void WriteParameterFlag(uint8_t Option, uint8_t Type, uint32_t Value);
  static uint32_t ReadParameterFlag(uint8_t Option, uint8_t Type);

  static void EraseFlash(uint32_t Address);
  static void WriteFlash(uint32_t Address, uint8_t *pData, uint16_t Len);
  static void ReadFlash(uint32_t Address, uint8_t *pData, uint16_t Len);
  static uint32_t ReadWord(uint32_t Address);
  static void WriteWord(uint32_t Address, uint32_t Data);
  static uint16_t ReadHalfword(uint32_t Address);
  static void WriteHalfword(uint32_t Address, uint16_t Data);
  static uint8_t ReadByte(uint32_t Address);
  static void WriteByte(uint32_t Address, uint8_t Data);
private:
};

extern LocalEEPROM hw_internal_eeprom;

#endif