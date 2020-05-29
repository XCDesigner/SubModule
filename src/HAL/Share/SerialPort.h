#include "stdint.h"

class SerialPort {
public:
  static void Init(int Baudary);
  int SendBytes(uint8_t *Datas, uint16_t Len);
  static bool ReadOneByte(uint8_t *Data);
  static int ReadBytes(uint8_t *pData, uint16_t Len);
  static bool ISR_SendOneByte(void);
  static void ISR_StoreOneByte(uint8_t Data);

private:
  static uint8_t ring_recv_buffer[1024];
  static uint16_t read_tail;
  static uint16_t read_head;
  static uint8_t ring_send_buffer[1024];
  static uint16_t send_tail;
  static uint16_t send_head;
};

extern SerialPort hw_uart_port;