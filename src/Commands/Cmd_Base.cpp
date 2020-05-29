#include "Cmd_Base.h"

CMD_BASE cmd_base;

// uint8_t CMD_BASE::pri_code;
// uint8_t CMD_BASE::sub_code;
// uint8_t CMD_BASE::*pData;
// char CMD_BASE::result;

/**
 *  @brief  Command Process
 *  @param  pBufferForReack:The buffer to store the reack data.without protocal head
 *  @retval The len of the reack data
 */ 
int CMD_BASE::Process(uint8_t *pBufferForReack) {
  result = 0;
  return 0;
}

int CMD_BASE::Reack() {
  return 0;
}