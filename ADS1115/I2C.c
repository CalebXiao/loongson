
#include "I2C.h"

unsigned char *map_base=NULL;
int dev_fd;

int init(void)
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
//打开 i2c 接口
int i2c_export(unsigned int i2c)
{ 
   int i2c_SDA_move;
   int i2c_SCL_move;
   int i2c_move;
	if(i2c == 0)
	{
		i2c_SDA_move = 17;
		i2c_SCL_move=16;
        i2c_move=10;
		printf("\n i2c0_export_success \n");
	}
	else if(i2c==1)
	{
		
		i2c_SDA_move = 19;
		i2c_SCL_move=18;
        i2c_move=11;
		printf("\n i2c1_export_success \n");
	}
    

	*(volatile unsigned int *)(map_base + GPIO_EN ) &= ~(1<<i2c_SDA_move);
	*(volatile unsigned int *)(map_base + GPIO_EN ) &= ~(1<<i2c_SCL_move);
    *(volatile unsigned int *)(map_base + REG0 ) |= (1<<i2c_move);

	return 0;
}

void ls2k_i2c_writeb(__u8 val, __u8 addr)
{
	//*(volatile unsigned int *)(map_base + GPIO_EN )&=~(1<<17);
	*(volatile unsigned int *)(map_base + I2C0_REG_BASE+addr ) = val;
}

int ls2k_i2c_readb(__u8 addr)
{
	//*(volatile unsigned int *)(map_base + GPIO_EN )|=(1<<17);
	return (*(volatile unsigned int *)(map_base + I2C0_REG_BASE+addr) );
}

static void ls2k_i2c_stop(void)
{
again:
        ls2k_i2c_writeb(CR_STOP, LS2K_I2C_CR_REG);
        ls2k_i2c_readb(LS2K_I2C_SR_REG);
        while (ls2k_i2c_readb(LS2K_I2C_SR_REG) & SR_BUSY)
                goto again;
}

static int ls2k_i2c_start(int dev_addr, int flags)
{
	//pr_info("ls2k_i2c_start start\n");
	int retry = 5;
	unsigned char addr = (dev_addr & 0x7f) << 1;
	addr |= (flags & I2C_M_RD)? 1:0;

start:
	ls2k_i2c_writeb(addr, LS2K_I2C_TXR_REG);
	/*ls2k_i2c_debug("%s <line%d>: i2c device address: 0x%x\n",
			__func__, __LINE__, addr);*/
	ls2k_i2c_writeb((CR_START | CR_WRITE), LS2K_I2C_CR_REG);
	while (ls2k_i2c_readb(LS2K_I2C_SR_REG) & SR_TIP) ;

	if (ls2k_i2c_readb(LS2K_I2C_SR_REG) & SR_NOACK) {
		ls2k_i2c_stop();
		while (retry--)
			goto start;
		pr_info("There is no i2c device ack\n");
		return 0;
	}
	//pr_info("ls2k_i2c_start finish\n");
	return 1;
}



__u16 ls2k_i2c_read()
{
        int i;
        __u16 buf;
        __u8 buffer[2];

		for(i=0;i<2;i++)
		{
			ls2k_i2c_writeb((i == (1))?
			(CR_READ | CR_ACK) : CR_READ,
			LS2K_I2C_CR_REG);
			while (ls2k_i2c_readb(LS2K_I2C_SR_REG) & SR_TIP) ;
			buffer[i]= ls2k_i2c_readb(LS2K_I2C_RXR_REG);
		}
		buf=buffer[0]*256+buffer[1];
        
		/*ls2k_i2c_debug("%s <line%d>: read buf[%d] <= %02x\n",
				__func__, __LINE__, i, buf[i]);*/
        

        return buf;
}

static int ls2k_i2c_write(unsigned char buf, int count)
{
		//pr_info("ls2k_i2c_write start\n");
        int i;

        for (i = 0; i < count; i++) {
		ls2k_i2c_writeb(buf, LS2K_I2C_TXR_REG);
		/*ls2k_i2c_debug("%s <line%d>: write buf[%d] => %02x\n",
				__func__, __LINE__, i, buf[i]);*/
		ls2k_i2c_writeb(CR_WRITE, LS2K_I2C_CR_REG);
		while (ls2k_i2c_readb(LS2K_I2C_SR_REG) & SR_TIP) ;

		if (ls2k_i2c_readb(LS2K_I2C_SR_REG) & SR_NOACK) {
			/*ls2k_i2c_debug("%s <line%d>: device no ack\n",
					__func__, __LINE__);*/
			ls2k_i2c_stop();
			return 0;
		}
        }
		//pr_info("ls2k_i2c_write finish\n");
        return i;
}


static int i2c_transfer(struct i2c_msg *msgs, int num)
{

}

static int Myi2c_transfer(__u16 addr, __u16 flags,__u16 len,__u8 buf)
{
}

/* ------------------------------------------------------------------------ */
/* API Functions                                                            */
/* ------------------------------------------------------------------------ */

void ls2k_i2c_init(int speed,  long long slaveaddr)
{
	if(slaveaddr)
		LS2K_I2C_BASE = slaveaddr;
        ls2k_i2c_writeb(0, LS2K_I2C_CTR_REG);
        ls2k_i2c_writeb(0x64, LS2K_I2C_PRER_LO_REG);
        ls2k_i2c_writeb(0x4, LS2K_I2C_PRER_HI_REG);
        ls2k_i2c_writeb(0x80, LS2K_I2C_CTR_REG);
}



/**
 * i2c_read: - Read multiple bytes from an i2c device
 *
 * The higher level routines take into account that this function is only
 * called with len < page length of the device (see configuration file)
 *
 * @chip:	address of the chip which is to be read
 * @addr:	i2c data address within the chip
 * @alen:	length of the i2c data address (1..2 bytes)
 * @buffer:	where to write the data
 * @len:	how much byte do we want to read
 * @return:	0 in case of success
 */

int i2c_read(__u8 chip, __u32 addr, int alen, __u8 *buffer, int len)
{
	
}

__u16 Myi2c_read(__u32 addr)
{
	__u16 buffer;
	ls2k_i2c_start(addr, I2C_M_RD);
	buffer=ls2k_i2c_read();
	ls2k_i2c_stop();
	return buffer;

}



/**
 * i2c_write: -  Write multiple bytes to an i2c device
 *
 * The higher level routines take into account that this function is only
 * called with len < page length of the device (see configuration file)
 *
 * @chip:	address of the chip which is to be written
 * @addr:	i2c data address within the chip
 * @alen:	length of the i2c data address (1..2 bytes)
 * @buffer:	where to find the data to be written
 * @len:	how much byte do we want to read
 * @return:	0 in case of success
 */

int i2c_write(__u8 chip, __u32 addr, int alen, __u8 *buffer, int len)
{

}


int Myi2c_write(__u32 addr, __u8 buffer)
{
                        
	ls2k_i2c_start(addr, 0);
	ls2k_i2c_write(buffer,1);
	ls2k_i2c_stop();
	return 0;
}

int Myi2c_ads1115_write(__u32 addr, __u8 pointADD,__u8 configH,__u8 configL)
{

	                        

	ls2k_i2c_start(addr, 0);
	ls2k_i2c_write(pointADD,1);
	ls2k_i2c_write(configH,1);
	ls2k_i2c_write(configL,1);
	ls2k_i2c_stop();
	return 0;
}

//----------------------------------------


