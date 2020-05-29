#ifndef _FUNCTION_DEFINER_H_
#define _FUNCTION_DEFINER_H_

#include "stdint.h"
#include "ADCPort.h"

typedef enum {
  M_UNDEFINE = 0,
  M_FDM,
  M_LASER,
  M_CNC,
  M_MOTOR
}ModuleType;

class FunctionDefiner {
public:
  static void Init();
  static void StartJob();
  static void EndJob();
  static ModuleType GetType();
  static uint32_t GetServiceLift();
  static void UpdateServiceLift();
  static void Loop();
private:
  static void FDM_loop();
  static void Laser_loop();
  static void CNC_loop();
  static void Motor_loop();

  static void FDM_Init();
  static void Laser_Init();
  static void CNC_Init();
  static void Motor_Init();
private:
  static ADCPort *adc;
  static ModuleType type;
  static uint32_t service_life;
  static uint32_t time_counter;
};

extern FunctionDefiner function_definer;

#endif