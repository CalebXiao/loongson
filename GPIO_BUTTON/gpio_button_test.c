#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "gpio.h"

void main(void){
	int i;
	int state;
	// 初始化 gpio
	gpio_init();
	// 使能为输出模式
	gpio_enable(13,0);
	// 循环
	while(1)
	{
		for(i=0;i<5000;i++)
		{
			state=gpio_read(13);
			printf("\n%d\n",state);
			/*
			// 输出高
			gpio_write(23,1);
			usleep(370);
			// 输出低
			gpio_write(23,1);
			usleep(370);
			*/
			usleep(1000000);
		}
	}
}
