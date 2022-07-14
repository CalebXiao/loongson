
#include "28BYJ_48.h"

void BYJ_gpio_enable(int IN0,int IN1,int IN2,int IN3)
{
	gpio_enable(IN0,1);
	gpio_enable(IN1,1);
	gpio_enable(IN2,1);
	gpio_enable(IN3,1);
}

void BYJ_gpio_write(int IN0,int IN1,int IN2,int IN3,unsigned char BeatCode)
{
	gpio_write(IN0,(BeatCode&0x1));
	gpio_write(IN1,(BeatCode&0x2)>>1);
	gpio_write(IN2,(BeatCode&0x4)>>2);
	gpio_write(IN3,(BeatCode&0x8)>>3);
}


