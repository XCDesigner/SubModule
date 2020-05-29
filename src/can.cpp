#include "Can.h"
#include "protocal.h"
#include "CANPort.h"
#include "Commands/Command.h"

CAN can;

/**
  * @brief  Initialize
  * @retval None
  */
void CAN::Init(void) {
  hw_can_port.Init();
  commands.AddCmdList((CMD_BASE*)&cmd_AE);
}

/**
  * @brief  Initialize
  * @retval None
  */
void CAN::Loop(void) {
  uint8_t tmp_buff[8];
  uint8_t tmp_len;
  uint16_t id;
  do
  {
    tmp_len = hw_can_port.ReadBytes(tmp_buff, sizeof(tmp_buff));
    if(tmp_len > 0) {
      if(PushRingBuff(tmp_buff, tmp_len) != tmp_len) {
        read_head = 0;
        read_tail = 0;
        return;
      }
    }
  } while (tmp_len > 0);

  if(ivi_protocal.Parse(&id, recv_buffer, parsed_buff, &read_tail, &read_head, sizeof(recv_buffer)) > 0) {
    int reack_len = commands.Handle(parsed_buff);
    if(reack_len != 0) {
      commands.GetReackData(parsed_buff);
      uint16_t parsed_len = ivi_protocal.Packedup(0, parsed_buff, send_buffer, reack_len);
      hw_can_port.SendBytes(send_buffer, parsed_len);
    }
  }
}

/**
 * brief  Push datas to the ring buffer
 * retval None
 */
uint8_t CAN::PushRingBuff(uint8_t *p_src, uint8_t len) {
  uint8_t tmp_head;
  uint8_t j = 0;
  for(int i=0;i<len;i++) {
    tmp_head = (read_head + 1) % sizeof(recv_buffer);
    if(tmp_head == read_tail)
      break;
    recv_buffer[read_head] = p_src[j]; 
    j++;
  }
  return j;
}
