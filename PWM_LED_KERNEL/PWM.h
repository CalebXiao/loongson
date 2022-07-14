#ifndef _PWM_H 
#define _PWM_H //打开 PWM 接口
int pwm_export(unsigned int pwm); //关闭 PWM 接口
int pwm_unexport(unsigned int pwm); //使能 pwm
int pwm_enable(unsigned int pwm); //禁止使能 pwm
int pwm_disable(unsigned int pwm); //设置占空比
int pwm_config(unsigned int pwm, unsigned int period, unsigned int duty_cycle); //设置极性 
int pwm_polarity(int pwm, int polarity); 
#endif // _PWM_H ----------------------------


