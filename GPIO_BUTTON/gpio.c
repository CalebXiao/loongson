#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include "gpio.h"

unsigned char *map_base=NULL;
int dev_fd;
int gpio_init(void){
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

int gpio_enable(int gpio_num,int val){ 
	int offset,gpio_move;
	if(gpio_num > 31)
	{
		offset = 4;
		gpio_move = gpio_num- 32;
	}
	else
	{
		offset = 0;
		gpio_move = gpio_num;
	}
	if(val==0)
	{
		//GPIO 使能 in
		*(volatile unsigned int *)(map_base + GPIO_EN +offset) |= (1<<gpio_move);
		//printf("Enable GPIO%d in\n",gpio_num);
	}
	else
	{
		//GPIO 使能 out
		*(volatile unsigned int *)(map_base + GPIO_EN +offset) &= ~(1<<gpio_move);
		//printf("Enable GPIO%d out\n",gpio_num);
	}
	return 0;
}

int gpio_close(void){
	if (dev_fd < 0)
	{
		printf("\nopen(/dev/mem) failed.\n");
		return -1;
	}
	munmap(map_base,MAP_SIZE);// 解除映射关系
	if(dev_fd)
	{
		close(dev_fd);
	}
	return 0;
}

int gpio_write(int gpio_num, int val){
	int offset, gpio_move;
	if(gpio_num > 31)
	{
		offset = 4;
		gpio_move = gpio_num- 32;
	}
	else
	{
		offset = 0;
		gpio_move = gpio_num;
	}
	if(val == 1)
	{// 输出高
		*(volatile unsigned int *)(map_base + GPIO_OUT +offset) |= (1<<gpio_move);
	}
	else
	{// 输出低
		*(volatile unsigned int *)(map_base + GPIO_OUT +offset) &=~(1<<gpio_move);
	}
}

int gpio_read(int gpio_num){
	int offset, gpio_move;
	if(gpio_num > 31)
	{
		offset = 4;
		gpio_move = gpio_num - 32;
	}
	else
	{
		offset = 0;
		gpio_move = gpio_num;
	}// 读取
	return (*(volatile unsigned int *)(map_base + GPIO_IN +offset) >> gpio_move) & 0x01;

}
