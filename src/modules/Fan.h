#ifndef _FAN_H_
#define _FAN_H_

#include "GPIOS.h"
#include "stdint.h"

class FAN {
public:
  void Init(GPIO_PIN Pin, GPIO_LEVEL Level);
  void On();
  void Off();

private:
  GPIO_PIN ControlPin;
  GPIO_LEVEL ActiveLevel;
  GPIO_LEVEL InactiveLevel;
};

#endif