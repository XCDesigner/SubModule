#include "Cmd_AE.h"

CMD_AE cmd_AE;

/**
 * brief  Process
 * retval 0
 */
int CMD_AE::Process() {
  switch(sub_code) {
  
  }

  return 0;
}

/**
 * brief  Reack
 * retval 0
 */
int CMD_AE::Reack() {
  switch(sub_code) {
    case 0x00:
    break;
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