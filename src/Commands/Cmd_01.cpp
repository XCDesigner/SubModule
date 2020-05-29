#include "Cmd_01.h"

CMD_01 cmd_01;

CMD_01::CMD_01() {
  pri_code = 0x01;
}

/**
 * brief  Process
 * retval 0
 */
int CMD_01::Process(uint8_t *pBufferForReack) {
  switch(sub_code) {
    
  }

  return 0;
}

/**
 * brief  Process
 * retval 0
 */
void CMD_01::Sub00() {
  
}