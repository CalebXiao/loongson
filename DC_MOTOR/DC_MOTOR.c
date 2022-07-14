
#include "DC_MOTOR.h"


void DC_Motor(int Pin1,int State1,int Pin2,int State2,int PWM_Pin,int Duty)
{
	gpio_enable(Pin1,0);
	gpio_enable(Pin2,0);
	gpio_write(Pin1,State1);
	gpio_write(Pin2,State2);
	pwm_export(PWM_Pin);
	pwm_enable(PWM_Pin);
	pwm_config(PWM_Pin, 255, Duty);
}

void LM298N_Motor_Init(int IN1,int IN2,int ENA,int ENB,int IN3,int IN4)
{
	gpio_enable(ENA,0);
	gpio_enable(ENB,0);
	gpio_write(ENA,1);
	gpio_write(ENB,1);
	pwm_export(IN1);
	pwm_enable(IN1);
	pwm_export(IN2);
	pwm_enable(IN2);
	pwm_export(IN3);
	pwm_enable(IN3);
	pwm_export(IN4);
	pwm_enable(IN4);

}
//Motor--> 1:Left 0:Right
//State--> 1:On 0:Off
void LM298N_Motor(int Motor,int State,int Duty)
{
	if(Motor==1)
	{
		if(State==1)
		{
			pwm_config(0, 255, Duty);
			pwm_config(1, 255, 0);
		}
		else if(State==0)
		{
			pwm_config(0, 255, 0);
			pwm_config(1, 255, Duty);
		}
	}
	else if(Motor==0)
	{
		if(State==1)
		{
			pwm_config(2, 255, Duty);
			pwm_config(3, 255, 0);
		}
		else if(State==0)
		{
			pwm_config(2, 255, 0);
			pwm_config(3, 255, Duty);
		}
	}

	
}
