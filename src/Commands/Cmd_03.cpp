#include "Cmd_03.h"
#include "Fan.h"

CMD_03 cmd_03;

CMD_03::CMD_03() {
  pri_code = 0x03;
}

/**
 * brief  Process
 * retval 0
 */
int CMD_03::Process(uint8_t *pBufferForReack) {
  switch(sub_code) {
    case 0x00:
      Sub00();
    break;

    case 0x01:
      Sub01();
    break;
    
  }

  return 0;
}

/**
 * brief  Fan on extern board
 * retval 0
 */
void CMD_03::Sub00() {
  // pData[0]:status
  if(pData[0] == 0)
    fan_extboard.Off();
  else
    fan_extboard.On();
}

/**
 * brief  Fan on terminal
 * retval 0
 */
void CMD_03::Sub01() {
  // pData[0]:status
  if(pData[0] == 0)
    fan_end.Off();
  else
    fan_end.On();
}

/**
 * brief  Process
 * retval 0
 */
void CMD_03::Sub02() {
  
}
