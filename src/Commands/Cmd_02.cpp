#include "Cmd_02.h"

CMD_02 cmd_02;

CMD_02::CMD_02() {
  pri_code = 0x02;
}

/**
 * brief  Process
 * retval 0
 */
int CMD_02::Process(uint8_t *pBufferForReack) {
  switch(sub_code) {
    
  }

  return 0;
}
