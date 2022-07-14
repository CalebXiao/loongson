#include "ADS1115.h"

void ads1115_config_register(__u8 pointADD,__u8 configH,__u8 configL)
{

    Myi2c_write(ADS1115_WRITE_ADDRESS,pointADD);
    Myi2c_write(ADS1115_WRITE_ADDRESS,configH);
    Myi2c_write(ADS1115_WRITE_ADDRESS,configL);
    printf("\n ads1115_config \n");
}
__u16 ads1115_read_data()
{
    __u16 data;

	Myi2c_write(ADS1115_WRITE_ADDRESS,ADS1015_REG_POINTER_CONVERT);
	data=Myi2c_read(ADS1115_WRITE_ADDRESS);
   
    return data;
}

