#ifndef _TEMPERATURE_H_
#define _TEMPERATURE_H_

#include "stdint.h"
#include "ADCPort.h"
#include "PWMPort.h"

typedef struct {
  float Kp;
  float Ki;
  float Kd;
  float err;
  float last_err;
  float int_err;
}strPID;

class Temperature {
public:
  void Init();
  void Loop();
  float GetCurrentTemp();
  void SetTargetTemp(float temp);
  void SetPID(float P, float I, float D);
  void GetPID(float *P, float *I, float *D);
  uint16_t GetPIDOutput();
  void IsrProcess();
private:
  void Update();
  uint16_t GetPID();

private:
  ADCPort *adc;
  PWMPort *pwm;
  float current;
  float target;
  strPID pid;
};

extern Temperature temperature;

#endif