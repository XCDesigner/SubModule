#include "protocal.h"

Protocal ivi_protocal;

/**
 * brief  Parse a pack
 * retval None
 */
int16_t Protocal::Parse(uint16_t *target_id, uint8_t *p_src, uint8_t *p_dest, uint8_t *p_tail, uint8_t *p_head, int buff_size) {
  uint8_t *psrc;
  uint8_t *pdst;
  uint16_t tail_backup;
  uint16_t tmp_tail, tmp_head;
  uint16_t total_len, pack_len;
  uint8_t high_byte, low_byte;
  uint16_t src_checksum;
  uint16_t cal_checksum;
  int buffer_size;

  psrc = p_src;
  pdst = p_dest;
  tmp_tail = *p_tail;
  tmp_head = *p_head;
  buffer_size = buff_size;

  total_len = (tmp_head + buff_size - tmp_tail) % buffer_size;

  pack_len = 0;
  while(total_len >= 10) {
    if(psrc[tmp_tail] != 0x5a) {
      tmp_tail = (tmp_tail + 1) % buffer_size;
      total_len--;
      continue;
    }

    if(psrc[(tmp_tail + 1) % buffer_size] != 0xa5) {
      tmp_tail = (tmp_tail + 2) % buffer_size;
      total_len =  total_len - 2;
      continue;
    }

    low_byte = psrc[(tmp_tail + 2) % buffer_size];
    high_byte = psrc[(tmp_tail + 3) % buffer_size];
    pack_len = (high_byte << 8) | low_byte;

    low_byte = psrc[(tmp_tail + 6) % buffer_size];
    high_byte = psrc[(tmp_tail + 7) % buffer_size];
    *target_id = (high_byte << 8) | low_byte;

    low_byte = psrc[(tmp_tail + 8) % buffer_size];
    high_byte = psrc[(tmp_tail + 9) % buffer_size];
    src_checksum = (high_byte << 8) | low_byte;

    if(total_len > (pack_len + 10)) {
      pack_len = 0;
      break;
    }

    // Copy datas to destination buffer
    tail_backup = tmp_tail;
    for(int i=0;i<(pack_len + 10);i++) {
      pdst[i] = psrc[tmp_tail];
      tmp_tail = (tmp_tail + 1) % buffer_size;
    }

    // Fill the last data with 0x00
    if((pack_len % 2) != 0) {
      pdst[pack_len++] = 0x00;
    }

    // Verify datas;
    cal_checksum = 0;
    for(int i=10;i<pack_len;i=i+2) {
      low_byte = pdst[i];
      high_byte = pdst[i + 1];
      cal_checksum = cal_checksum + ((high_byte << 8) | (low_byte));
    }

    if(cal_checksum != src_checksum) {
      tmp_tail = (tail_backup + 2) % buffer_size;
      total_len = total_len - 2;
      continue;
    }

    *p_tail = tmp_tail;

    return (pack_len + 10);
  }

  return 0;
  
}

/**
 * brief  Setup a packed 
 * retval None
 */ 
int16_t Protocal::Packedup(uint16_t target_id, uint8_t *p_src, uint8_t *p_dest, uint16_t src_len) {
  uint16_t i = 0;
  uint16_t j;
  uint16_t low_byte, high_byte;
  uint16_t cal_checksum;

  p_dest[i++] = 0x5a;
  p_dest[i++] = 0xa5;
  p_dest[i++] = src_len >> 8;
  p_dest[i++] = src_len;
  p_dest[i++] = p_dest[2] ^ p_dest[3];
  p_dest[i++] = 0;
  p_dest[i++] = target_id >> 8;
  p_dest[i++] = target_id;

  cal_checksum = 0;
  for(j=0;j<(src_len-1);j=j+2) {
    low_byte = p_src[j];
    high_byte = p_src[j+1];
    cal_checksum = cal_checksum + ((high_byte << 8) | (low_byte));
  }
  if(src_len % 2 != 0)
    cal_checksum = cal_checksum + p_src[src_len];

  p_dest[i++] = cal_checksum >> 8;
  p_dest[i++] = cal_checksum;

  for(j=0;j<src_len;j++) {
    p_dest[i++] = p_src[j];
  }

  return i;
}
