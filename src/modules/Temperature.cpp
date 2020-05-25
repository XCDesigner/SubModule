#include "Temperature.h"

Temperature temperature;

typedef struct {
  uint32_t RawValue;
  float Temp;
}strTempTable;

static const strTempTable temp_table[] = {
 {1111,11.0},
 {1111,12.0}
};

/**
  * @brief  Initialize
  * @retval None
  */
void Temperature::Init(void) {
  adc = &hw_adc_port;
  pwm = &hw_pwm_port;
  adc->Init();
  adc->Enable();
  pwm->Init();
}

/**
  * @brief  Initialize
  * @retval None
  */
void Temperature::Loop(void) {
  
}

/**
  * @brief  
  * @retval None
  */
void Temperature::SetTargetTemp(float temp) {
  target = temp;
}

/**
  * @brief  
  * @retval None
  */
float Temperature::GetCurrentTemp() {
  return current;
}

/**
  * @brief  
  * @retval None
  */
void Temperature::SetPID(float P, float I, float D) {
  pid.Kp = P;
  pid.Ki = I;
  pid.Kd = D;
}

/**
  * @brief  
  * @retval None
  */
void Temperature::GetPID(float *P, float *I, float *D) {
  *P = pid.Kp;
  *I = pid.Ki;
  *D = pid.Kd;
}

/**
  * @brief  
  * @retval None
  */
uint16_t Temperature::GetPIDOutput() {
  float pid_output;

  pid.last_err = pid.err;
  pid.err = target - current;
  pid_output = pid.Kp * pid.err + pid.Ki * pid.int_err + pid.Kd * (pid.err - pid.last_err);
  pid.int_err = pid.int_err + pid.err;

  if(pid_output > 0) {
    if(pid_output > 256)
      pid_output = 256;
  }
  else {
    pid_output = 0;
  }
  return pid_output;
}

/**
  * @brief  
  * @retval None
  */
void Temperature::Update() {
  uint32_t i;
  float int_temp;
  float decimal_temp;

  uint16_t raw_value;

  raw_value = adc->GetAdcValue(1);

  int_temp = -20;
  decimal_temp = 0;

  for(i=1;i<sizeof(temp_table) / sizeof(temp_table[0]);i++) {
    if(temp_table[i].RawValue < raw_value) {
      int_temp = temp_table[i - 1].Temp;
      decimal_temp = (temp_table[i].Temp - temp_table[i-1].Temp) / (temp_table[i].RawValue - temp_table[i-1].RawValue);
      decimal_temp = decimal_temp * (raw_value - temp_table[i-1].RawValue);
      break;
    }
  }
  current = int_temp + decimal_temp;
}

/**
  * @brief  This function is call by the ADC interrupt for updating the temperature and control the heater  
  * @retval None
  */
void Temperature::IsrProcess() {
  uint16_t PID_output;
  Update();
  PID_output = GetPIDOutput();
  pwm->SetPwmValue(PID_output);

}