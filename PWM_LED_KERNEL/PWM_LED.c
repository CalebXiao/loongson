#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include "PWM.h"
int main()
{ 
    int pwm = 0;
    //pwm0
    int period = 10000;
    //周期 ns
    int duty_cycle = 0;
    //高占空比时间 ns
    int calc = 500;
    //打开 PWM 接口
    pwm_export(pwm);
    printf("pwm_export:%d\n", pwm);
    //使能 pwm pwm_enable(pwm);
    printf("pwm_enable:%d\n", pwm);
    while (1) 
    { //设置占空比
        pwm_config(pwm, period, duty_cycle);
        duty_cycle += calc;
        if (duty_cycle == period)
        { 
            calc = calc * -1;
        }
        else if (duty_cycle == 0) 
        {
             calc = calc * -1; 
        }
        usleep(50000);
    } 
}