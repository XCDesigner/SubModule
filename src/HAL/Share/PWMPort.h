#ifndef _PWMPORT_H_
#define _PWMPORT_H_

#include "stdint.h"

class PWMPort {
public:
  void Init();
  void SetPwmValue(uint16_t Value);
private:
  
private:
  
};

extern PWMPort hw_pwm_port;
#endif