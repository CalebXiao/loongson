#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "gpio.h"
#include "28BYJ_48.h"

unsigned char BYJ_Code[8]={0xE,0xC,0xD,0x9,0xB,0x3,0x7,0x6};

void main(void){
	
	int time=2500;
	// 初始化 gpio
	gpio_init();
	// 使能为输出模式
    BYJ_gpio_enable(10,11,12,13);
	// 循环
	while(1)
	{
        for(int index=0;index<8;index++)
        {
			BYJ_gpio_write(10,11,12,13,BYJ_Code[index]);
			usleep(time);
		}
		
	}
	
}
