#include "DS2431.h"
#include "stm32f10x_conf.h"

#define TW1L  (30)
#define TW0L  (10)
#define TRL   (5)
#define TRS   (13)
#define TRSTL   (100)
#define TPDH  (13)
#define TPDL  (15)
#define TPROGMAX  (200000)
#define DAT_PIN GPIO_Pin_11

#define DAT_H() GPIOA->BSRR = DAT_PIN
#define DAT_L() GPIOA->BRR = DAT_PIN
#define READ_DAT()  (GPIOA->IDR & DAT_PIN)

#define DS_ADDRESS_TIME   (16)

bool OneWireEEPROM::chip_exist = true;

/**
 * brief  Initialize
 * retval None
 */
void OneWireEEPROM::Init(void) {
  GPIO_InitTypeDef GPIO_InitStruct;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_OD;
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_11;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStruct);
}

/**
 * brief  
 * retval None
 */
uint32_t OneWireEEPROM::ReadTime() {
  uint32_t counter, counter_invert;

  SkipRomMach();
	WriteOneByte(0xF0);
	WriteOneByte(DS_ADDRESS_TIME);
	WriteOneByte(DS_ADDRESS_TIME>>8);
  for(int i=0;i<4;i++) {
    counter |= ReadOneByte() << 24;
    counter = (counter >> 8) & 0xffffff;
  }
  for(int i=0;i<4;i++) {
    counter_invert |= ReadOneByte() << 24;
    counter_invert = (counter_invert >> 8) & 0xffffff;
  }
  if((counter ^ counter_invert) == 0xffffffff)
    return counter;
  else
    return 0xffffffff;
}

/**
 * brief  
 * retval None
 */
bool OneWireEEPROM::WriteTime(uint32_t Counter) {
  uint32_t counter_invert;
  uint32_t tmp_shifter;
  uint16_t readbak_addr;
  uint16_t crc_read;
  uint8_t flag_es;

  counter_invert = ~Counter;

  SkipRomMach();
  WriteOneByte(0x0f);
  WriteOneByte(DS_ADDRESS_TIME);
  WriteOneByte(DS_ADDRESS_TIME >> 8);
  tmp_shifter = Counter;
  for(int i=0;i<4;i++) {
    WriteOneByte(tmp_shifter);
    tmp_shifter = tmp_shifter >> 8;
  }
  tmp_shifter = counter_invert;
  for(int i=0;i<4;i++) {
    WriteOneByte(tmp_shifter);
    tmp_shifter = tmp_shifter >> 8;
  }
  // CRC
  crc_read = ReadOneByte();
  crc_read = (ReadOneByte() << 8) | crc_read;

  for(int i=0;i<1000;i++);

  SkipRomMach();
  WriteOneByte(0xaa);
  // Address verify
  readbak_addr = ReadOneByte();
  readbak_addr = (ReadOneByte() << 8) | readbak_addr;
  flag_es = ReadOneByte();
  // if(readbak_addr == DS_ADDRESS_TIME) {

  // }
  if(flag_es != 0x07)
    return false;

  // Data verify
  tmp_shifter = 0;
  for(int i=0;i<4;i++) {
    tmp_shifter |= ReadOneByte() << 24;
    tmp_shifter = (tmp_shifter >> 8) & 0xffffff;
  }
  // if(tmp_shifter != Counter) {

  // }
  tmp_shifter = 0;
  for(int i=0;i<4;i++) {
    tmp_shifter |= ReadOneByte() << 24;
    tmp_shifter = (tmp_shifter >> 8) & 0xffffff;
  }
  // if(tmp_shifter != counter_invert) {

  // }
  // CRC
  crc_read = ReadOneByte();
  crc_read = (ReadOneByte() << 8) | crc_read;
  

  SkipRomMach();
  WriteOneByte(0x55);
  WriteOneByte(DS_ADDRESS_TIME);
  WriteOneByte(DS_ADDRESS_TIME >> 8);
  WriteOneByte(0x07);

  for(int i=0;i<TPROGMAX;i++);
  if(ReadOneByte() == 0xaa) 
    return true;
  else
    return false;
}

/**
 * brief  
 * retval None
 */
void OneWireEEPROM::WriteOneByte(uint8_t data) {
  int i;
  for(i=0;i<8;i++) {
    if(data & 0x01)
      WriteBitHigh();
    else
      WriteBitLow();
    data = data >> 1;
  }
  for(i=0;i<100;i++);
}

/**
 * brief  
 * retval None
 */
uint8_t OneWireEEPROM::ReadOneByte(void) {
  uint8_t retval;
  retval = 0xff;
  for(int j=0;j<8;j++) {
    DAT_L();
    for(int i=0;i<TRL;i++);
    DAT_H();
    for(int i=0;i<TRS;i++);
    if(READ_DAT() != 0)
      retval = (retval << 1) | 1;
    else
      retval = (retval << 1) & 0xfe;
  }
  return retval;
}

/**
 * brief  
 * retval None
 */
void OneWireEEPROM::SkipRomMach(void) {
  if(ResetBus() == true) {
    WriteOneByte(0xcc);
  }
}

/**
 * brief  
 * retval None
 */
bool OneWireEEPROM::ResetBus(void) {
  uint32_t bus_level;
  DAT_L();
  for(int i=0;i<TRSTL;i++);
  DAT_H();
  for(int i=0;i<TPDH;i++);
  bus_level = READ_DAT();
  while(READ_DAT() == 0);
  chip_exist = (bus_level == 0);
  return (bus_level == 0);
}

/**
 * brief  
 * retval None
 */
__INLINE void OneWireEEPROM::WriteBitHigh() {
  DAT_L();
  for(int i=0;i<TW1L;i++);
  DAT_H();
  // Delay for 60us
  for(int i=0;i<TW0L;i++);
}

/**
 * brief  
 * retval None
 */
__INLINE void OneWireEEPROM::WriteBitLow() {
  DAT_L();
  // Delay for TW0L
  for(int i=0;i<TW0L;i++);
  DAT_H();
  // Delay for 60us
  for(int i=0;i<TW0L;i++);
}

/**
 * brief  
 * retval None
 */
__INLINE bool OneWireEEPROM::ReadBit() {
  bool retval;
  DAT_L();
  // Delay for TRL
  for(int i=0;i<TRL;i++);
  DAT_H();
  for(int i=0;i<TRS;i++);
  // Delay for 60us
  for(int i=0;i<TW0L;i++);
  retval = (READ_DAT() != 0);

  return retval;
}