#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include "PWM.h"
unsigned char *map_base=NULL;
int dev_fd;

int pwm_init(void)
{
	dev_fd = open("/dev/mem", O_RDWR | O_SYNC);
	if (dev_fd < 0)
	{
		printf("\nopen(/dev/mem) failed.\n");
		return -1;
	}
	map_base=(unsigned char *)mmap(0,MAP_SIZE,PROT_READ|PROT_WRITE,
	MAP_SHARED,dev_fd,REG_BASE);
	return 0;
}
//打开 PWM 接口
int pwm_export(unsigned int pwm)
{ 
   int gpio_move;
   int pwm_move;
	if(pwm == 0)
	{
		gpio_move = 20;
        pwm_move=12;
		printf("\n pwm0_export_success \n");
	}
	else if(pwm==1)
	{
		
		gpio_move = 21;
        pwm_move=13;
		printf("\n pwm1_export_success \n");
	}
    else if(pwm==2)
	{
		gpio_move = 22;
        pwm_move=14;
		printf("\n pwm2_export_success \n");
	}
    else if(pwm==3)
	{
		gpio_move = 23;
        pwm_move=15;
		printf("\n pwm3_export_success \n");
	}

	*(volatile unsigned int *)(map_base + GPIO_EN ) &= ~(1<<gpio_move);
    *(volatile unsigned int *)(map_base + REG0 ) |= (1<<pwm_move);

	return 0;
}

//关闭 PWM 接口
int pwm_unexport(unsigned int pwm)
{ 
   
}

//使能 pwm
int pwm_enable(unsigned int pwm)
{ 
    if(pwm == 0)
	{
		*(volatile unsigned int *)(map_base + PWM0_BASE+0xC ) = 0x01;
		printf("\n pwm0_enable_success \n");
	}
	else if(pwm==1)
	{
		*(volatile unsigned int *)(map_base + PWM1_BASE+0xC ) = 0x01;
		printf("\n pwm1_enable_success \n");
	}
    else if(pwm==2)
	{
		*(volatile unsigned int *)(map_base + PWM2_BASE+0xC ) = 0x01;
		printf("\n pwm2_enable_success \n");
	}
    else if(pwm==3)
	{
		*(volatile unsigned int *)(map_base + PWM3_BASE+0xC ) = 0x01;
		printf("\n pwm3_enable_success \n");
	} 
}

//禁止使能 pwm
int pwm_disable(unsigned int pwm)
{ 
    
}

//设置占空比
int pwm_config(unsigned int pwm, unsigned int period, unsigned int duty_cycle)
{ 
   if(pwm == 0)
	{
		*(volatile unsigned int *)(map_base + PWM0_BASE+0x4 ) = duty_cycle;
        *(volatile unsigned int *)(map_base + PWM0_BASE+0x8 ) = period;
	}
	else if(pwm==1)
	{
		
		*(volatile unsigned int *)(map_base + PWM1_BASE+0x4 ) = duty_cycle;
        *(volatile unsigned int *)(map_base + PWM1_BASE+0x8 ) = period;
		
	}
    else if(pwm==2)
	{
		*(volatile unsigned int *)(map_base + PWM2_BASE+0x4 ) = duty_cycle;
        *(volatile unsigned int *)(map_base + PWM2_BASE+0x8 ) = period;
	}
    else if(pwm==3)
	{
		*(volatile unsigned int *)(map_base + PWM3_BASE+0x4 ) = duty_cycle;
        *(volatile unsigned int *)(map_base + PWM3_BASE+0x8 ) = period;
	} 
    return 0; 
}
//设置极性
int pwm_polarity(int pwm, int polarity)
{ 
   
}
