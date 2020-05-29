#include "Cmd_AE.h"

CMD_AE cmd_AE;

/**
 * brief  
 * retval 0
 */
CMD_AE::CMD_AE() {
  pri_code = 0xAE;
}

/**
 * brief  Process
 * retval 0
 */
int CMD_AE::Process(uint8_t *pBufferForReack) {
  switch(sub_code) {
    
  }

  return 0;
}

/**
 * brief  Sub Code 01 process
 * retval 0
 */
void CMD_AE::Sub01() {
  
}

/**
 * brief  Sub Code 02 process
 * retval 0
 */
void CMD_AE::Sub02() {

}