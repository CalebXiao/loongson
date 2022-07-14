#ifndef _DC_MOTOR_H 
#define _DC_MOTOR_H 
#include <stdio.h>
#include <stdlib.h>
#include "PWM.h"
#include "gpio.h"


void DC_Motor(int Pin1,int State1,int Pin2,int State2,int PWM_Pin,int Duty);
void LM298N_Motor_Init(int IN1,int IN2,int ENA,int ENB,int IN3,int IN4);
void LM298N_Motor(int Motor,int State,int Duty);
#endif // _DC_MOTOR_H ----------------------------
