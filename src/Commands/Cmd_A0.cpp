#include "Cmd_A0.h"

CMD_A0 cmd_A0;

/**
 * brief  Process
 * retval 0
 */
int CMD_A0::Process() {
  switch(sub_code) {
  
  }

  return 0;
}

/**
 * brief  Reack
 * retval 0
 */
int CMD_A0::Reack() {
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
void CMD_A0::Sub01() {

}

/**
 * brief  Sub Code 02 process
 * retval 0
 */
void CMD_A0::Sub02() {

}