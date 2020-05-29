#ifndef _FAN_H_
#define _FAN_H_

#include "GPIOS.h"
#include "stdint.h"

class FAN {
public:
  FAN();
  void Init(GPIO_PIN Pin, GPIO_LEVEL Level);
  void On();
  void Off();
private:
  bool Valid;
  GPIO_PIN ControlPin;
  GPIO_LEVEL ActiveLevel;
  GPIO_LEVEL InactiveLevel;
};

extern FAN fan_extboard;
extern FAN fan_end;

#endif