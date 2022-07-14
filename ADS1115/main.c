#include <stdio.h>
#include <stdlib.h>
#include "I2C.h"
#include "ADS1115.h"
int main(void)
{
	int i2c=0;
    double val;
	init();
    i2c_export(i2c);
	ls2k_i2c_init(0, LS2K1000_I2C0_REG_BASE);
	ads1115_config_register(0x01,CONFIG_REG_H,CONFIG_REG_L);
   
    while(1)
	{
        val=ads1115_read_data();
        printf("val is %f\r\n",val);
        usleep(1000000);
	}
	return 0;
}
