#ifndef _PROTOCLA_H_
#define _PROTOCLA_H_

#include "stdint.h"

class Protocal {
public:
  int16_t Parse(uint16_t *target_id, uint8_t *p_src, uint8_t *p_dest, uint8_t *p_tail, uint8_t *p_head, int buff_size);
  int16_t Packedup(uint16_t target_id, uint8_t *p_src, uint8_t *p_dest, uint16_t src_len);
};

extern Protocal ivi_protocal;

#endif