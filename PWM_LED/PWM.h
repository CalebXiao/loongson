#ifndef _PWM_H 
#define _PWM_H 
#define MAP_SIZE 0x20000
#define REG_BASE 0x1fe00000

#define GPIO_EN 0x10500
#define GPIO_OUT 0x10510
#define GPIO_IN 0x10520

#define REG0 0x10420//通用配置寄存器0
#define PWM0_BASE 0x02000//
#define PWM1_BASE 0x02010//
#define PWM2_BASE 0x02020//
#define PWM3_BASE 0x02030//
//打开 PWM 接口
int pwm_export(unsigned int pwm); 
//关闭 PWM 接口
int pwm_unexport(unsigned int pwm); 
//使能 pwm
int pwm_enable(unsigned int pwm); 
//禁止使能 pwm
int pwm_disable(unsigned int pwm); 
//设置占空比
int pwm_config(unsigned int pwm, unsigned int period, unsigned int duty_cycle);
 //设置极性 
int pwm_polarity(int pwm, int polarity); 
#endif // _PWM_H ----------------------------


