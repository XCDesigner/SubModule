#include "stdint.h"

class CANPort{
public:
  static void Init();
  static int SendBytes(uint8_t *Datas, uint16_t Len);
  static bool ReadOneByte(uint8_t *Data);
  static int ReadBytes(uint8_t *pData, uint16_t Len);
  static void ISRInit();
  static void PushData(uint8_t *pData, uint8_t Len);
private:
  static void FilterInit();

private:
  static uint8_t ring_recv_buffer[1024];
  static uint16_t read_tail;
  static uint16_t read_head;
};

extern CANPort hw_can_port;