#include "stdint.h"

class SPIPort{
public:
  static void Init();
  static void SendOneByte(uint8_t Data);
  static void SendBytes(uint8_t *Datas, uint16_t Len);
  static uint8_t ReadOneByte(void);
  static void ReadBytes(uint8_t *pData, uint16_t Len);
private:
  static uint8_t ReadWriteOneByte(uint8_t DataToSend);
  
};

extern SPIPort hw_spi_port;