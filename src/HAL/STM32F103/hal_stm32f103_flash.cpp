#include "LocalEEPROM.h"
#include "stm32f10x_conf.h"

#define FLAG_VALID_OFFSET   0
#define FLAG_ROLLING_OFFSET 4
#define CONTENT_OFFSET  32
#define FLAG_VALID      0x11223344

#define SIZE_BOOT         (16 * 1024)
#define SIZE_MODULE_DESC  (2 * 1024)
#define SIZE_PARAMETER    (2 * 1024)

#define FLASH_START       0x8014000
#define FLASH_MODULE_DESC (FLASH_START + SIZE_BOOT)
#define FLASH_PARAMETER   (FLASH_MODULE_DESC + SIZE_MODULE_DESC)
#define FLASH_PARAMETER_BAK   (FLASH_PARAMETER + SIZE_PARAMETER)

/**
  * @brief  
  * @param  
  * @retval  
  */
bool LocalEEPROM::RestoreSetting(uint8_t *pData, uint32_t Len) {
  uint32_t Flag0, Flag1;
  uint32_t rolling0, rolling1;
  uint32_t Recover_Flag;
  bool retval;

  Flag0 = ReadParameterValidFlag(0);
  Flag1 = ReadParameterValidFlag(1);
  rolling0 = ReadParameterRollingFlag(0);
  rolling1 = ReadParameterRollingFlag(1);
  Recover_Flag = 0xff;

  FLASH_Unlock();
  if((Flag0 == FLAG_VALID) && (Flag1 == FLAG_VALID)) {
    // If the 1st region is valid, it must be the right one we stored lastest.
    // But we have to check if the 2nd region had been updated.
    // If both region have the same rolling flag means these two region have been sync
    ReadParameter(0, pData, Len);
    if(rolling0 != rolling1)
      Recover_Flag = 1;
    else
      Recover_Flag = 0xfe;
  }
  else if(Flag0 == FLAG_VALID) {
    // This case was happend when we were storing the 2nd region.So we have to restore the 1st region
    // and we have to recover the 2nd region with the 1st region.
    ReadParameter(0, pData, Len);
    Recover_Flag = 1;
  }
  else if(Flag1 == FLAG_VALID) {
    // This case was happend when we were storing thd 1st region.Do as before.
    ReadParameter(1, pData, Len);
    Recover_Flag = 0;
  }
  else {
    // This means flash is empty.
  }
  
  if(Recover_Flag == 0) {
    WriteParameter(0, pData, Len);
    WriteParameterRollingFlag(0, rolling1);
    WriteParameterValidFlag(0);
    retval = true;
  }
  else if(Recover_Flag == 1) {
    WriteParameter(1, pData, Len);
    WriteParameterRollingFlag(1, rolling0);
    WriteParameterValidFlag(1);
    retval = true;
  }
  else if(Recover_Flag == 0xff) {
    retval = false;
  }
  else {
    retval = true;
  }
  FLASH_Lock();
  return retval;
}

/**
  * @brief  
  * @param  
  * @retval  
  */
void LocalEEPROM::StoreSetting(uint8_t *pData, uint32_t Len) {
  uint32_t rolling_flag;

  rolling_flag = ReadParameterRollingFlag(0);
  rolling_flag++;
  FLASH_Unlock();
  WriteParameter(0, pData, Len);
  WriteParameterRollingFlag(0, rolling_flag);
  WriteParameterValidFlag(0);
  WriteParameter(1, pData, Len);
  WriteParameterRollingFlag(1, rolling_flag);
  WriteParameterValidFlag(1);
  FLASH_Lock();
}

/**
  * @brief  
  * @param  
  * @retval  
  */
uint32_t LocalEEPROM::GetModuleType() {
  uint32_t Address;
  uint32_t u32Value;
  
  Address = FLASH_MODULE_DESC;
  u32Value = *((uint32_t*)Address);
  return u32Value;
}

/**
  * @brief  
  * @param  
  * @retval  
  */
void LocalEEPROM::WriteParameter(uint8_t Option, uint8_t *pData, uint32_t Len) {
  uint32_t Address;
  uint16_t u16Value;
  int i;
  
  if(Option == 0)
    Address = (FLASH_PARAMETER + CONTENT_OFFSET);
  else
    Address = (FLASH_PARAMETER_BAK + CONTENT_OFFSET);
  FLASH_ErasePage(Address);
  for(i=0;i<Len;i=i+2) {
    u16Value = (pData[i+1] << 8) | pData[i];
    FLASH_ProgramHalfWord(Address, u16Value);
    Address = Address + 2;
  }
}

/**
  * @brief  
  * @param  
  * @retval  
  */
void LocalEEPROM::ReadParameter(uint8_t Option, uint8_t *pData, uint32_t Len) {
  uint32_t Address;
  int i;
  
  if(Option == 0)
    Address = (FLASH_PARAMETER + CONTENT_OFFSET);
  else
    Address = (FLASH_PARAMETER_BAK + CONTENT_OFFSET);
  for(i=0;i<Len;i++) {
    *pData++ = *((uint8_t*)Address++);
  }
}

/**
  * @brief  
  * @param  
  * @retval  
  */
uint32_t LocalEEPROM::ReadParameterFlag(uint8_t Option, uint8_t Type) {
  uint32_t Address;
  uint32_t u32Value;
  
  if(Type == 0)
    Address = FLAG_VALID_OFFSET;
  else
    Address = FLAG_ROLLING_OFFSET;

  if(Option == 0)
    Address += FLASH_PARAMETER;
  else
    Address += FLASH_PARAMETER_BAK;
  u32Value = *((uint32_t*)Address);
  return u32Value;
}

/**
  * @brief  
  * @param  
  * @retval  
  */
void LocalEEPROM::WriteParameterFlag(uint8_t Option, uint8_t Type, uint32_t Value) {
  uint32_t Address;
  
  if(Type == 0)
    Address = FLAG_VALID_OFFSET;
  else
    Address = FLAG_ROLLING_OFFSET;

  if(Option == 0)
    Address += FLASH_PARAMETER;
  else
    Address += FLASH_PARAMETER_BAK;
  FLASH_ProgramWord(Address, Value);
}

/**
  * @brief  
  * @param  
  * @retval  
  */
uint32_t LocalEEPROM::ReadParameterValidFlag(uint8_t Option) {
  uint32_t u32Value;

  u32Value = ReadParameterFlag(Option, 0);
  return u32Value;
}

/**
  * @brief  
  * @param  
  * @retval  
  */
void LocalEEPROM::WriteParameterValidFlag(uint8_t Option) {  
  WriteParameterFlag(Option, 0, FLAG_VALID);
}

/**
  * @brief  
  * @param  
  * @retval  
  */
uint32_t LocalEEPROM::ReadParameterRollingFlag(uint8_t Option) {
  uint32_t u32Value;

  u32Value = ReadParameterFlag(Option, 1);
  return u32Value;
}

/**
  * @brief  
  * @param  
  * @retval  
  */
void LocalEEPROM::WriteParameterRollingFlag(uint8_t Option, uint32_t RollingValue) {
  WriteParameterFlag(Option, 1, RollingValue);
}

/**
  * @brief  
  * @param  
  * @retval  
  */
void LocalEEPROM::WriteFlash(uint32_t Address, uint8_t *pData, uint16_t Len) {
  uint16_t u16Data;
  FLASH_Unlock();
  for(int i=0;i<Len;i=i+1) {
    u16Data = pData[i] | (pData[i+1] << 8);
    FLASH_ProgramHalfWord(Address, u16Data);
  }
  FLASH_Lock();
}

/**
  * @brief  
  * @param  
  * @retval 
  */
void LocalEEPROM::ReadFlash(uint32_t Address, uint8_t *pData, uint16_t Len) {
  for(int i=0;i<Len;i++)
    *pData++ = *((uint8_t*)Address++);
}

/**
  * @brief  
  * @param  
  * @retval  
  */
uint32_t LocalEEPROM::ReadWord(uint32_t Address) {
  uint32_t u32Data;
  u32Data = *(uint32_t*)Address;
  return u32Data;
}

/**
  * @brief  
  * @param  
  * @retval  
  */
void LocalEEPROM::WriteWord(uint32_t Address, uint32_t Data) {
  FLASH_Unlock();
  FLASH_ProgramWord(Address, Data);
  FLASH_Lock();
}

/**
  * @brief  
  * @param  
  * @retval  
  */
uint16_t LocalEEPROM::ReadHalfword(uint32_t Address) {
  uint16_t u16Data;
  u16Data = *(uint16_t*)Address;
  return u16Data;
}

/**
  * @brief  
  * @param  
  * @retval  
  */
void LocalEEPROM::WriteHalfword(uint32_t Address, uint16_t Data) {
  FLASH_Unlock();
  FLASH_ProgramHalfWord(Address, Data);
  FLASH_Lock();
}

/**
  * @brief  
  * @param  
  * @retval  
  */
uint8_t LocalEEPROM::ReadByte(uint32_t Address) {
  uint8_t u8Data;
  u8Data = *(uint8_t*)Address;
  return u8Data;
}

/**
  * @brief  
  * @param  
  * @retval  
  */
void LocalEEPROM::WriteByte(uint32_t Address, uint8_t Data) {
  FLASH_Unlock();
  FLASH_ProgramHalfWord(Address, Data);
  FLASH_Lock();
}