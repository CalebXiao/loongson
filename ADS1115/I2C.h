#ifndef _I2C_H 
#define _I2C_H 
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <linux/types.h>
#include "ADS1115.h"

#define MAP_SIZE 0x20000
#define REG_BASE 0x1fe00000

#define GPIO_EN 0x10500
#define GPIO_OUT 0x10510
#define GPIO_IN 0x10520

#define REG0 0x10420//通用配置寄存器0

#define I2C0_REG_BASE 0x01000
#define I2C1_REG_BASE 0x01800

#define UNCACHED_MEMORY_ADDR	0x8000000000000000
#define	PHYS_TO_UNCACHED(x) 	((unsigned long long)(x) | UNCACHED_MEMORY_ADDR)
#define LS2K1000_I2C1_REG_BASE				PHYS_TO_UNCACHED(0x1fe21800)
#define LS2K1000_I2C0_REG_BASE				PHYS_TO_UNCACHED(0x1fe21000)

#define inb(p) ({__u8 __v=__raw_readb(__io(p));__v;})
//#define writeb(reg, val)	outb(reg, val)
//#define readb(reg)		inb(reg)
//#define ls2k_i2c_writeb(val, addr)	outb(LS2K_I2C_BASE + addr, val)
//#define ls2k_i2c_readb(addr)		inb(LS2K_I2C_BASE +  addr)


/* All transfers are described by this data structure */
struct i2c_msg {
	__u16 addr;	/* slave address			*/
	__u16 flags;
#define I2C_M_TEN		0x0010	/* this is a ten bit chip address */
#define I2C_M_RD		0x0001	/* read data, from slave to master */
#define I2C_M_NOSTART		0x4000	/* if I2C_FUNC_PROTOCOL_MANGLING */
#define I2C_M_REV_DIR_ADDR	0x2000	/* if I2C_FUNC_PROTOCOL_MANGLING */
#define I2C_M_IGNORE_NAK	0x1000	/* if I2C_FUNC_PROTOCOL_MANGLING */
#define I2C_M_NO_RD_ACK		0x0800	/* if I2C_FUNC_PROTOCOL_MANGLING */
#define I2C_M_RECV_LEN		0x0400	/* length will be first received byte */
	__u16 len;		/* msg length				*/
	__u8 *buf;		/* pointer to msg data			*/
};


#define	CR_START			0x80
#define	CR_STOP				0x40
#define	CR_READ				0x20
#define	CR_WRITE			0x10
#define	CR_ACK				0x8
#define	CR_IACK				0x1

#define	SR_NOACK			0x80
#define	SR_BUSY				0x40
#define	SR_AL				0x20
#define	SR_TIP				0x2
#define	SR_IF				0x1

#define LS2K_I2C_PRER_LO_REG	0x0
#define LS2K_I2C_PRER_HI_REG	0x1
#define LS2K_I2C_CTR_REG    	0x2
#define LS2K_I2C_TXR_REG    	0x3
#define LS2K_I2C_RXR_REG    	0x3
#define LS2K_I2C_CR_REG     	0x4
#define LS2K_I2C_SR_REG     	0x4

//#define ls2k_i2c_debug(fmt, args...)	printf(fmt, ##args)
#define pr_info printf

static long long LS2K_I2C_BASE=LS2K1000_I2C1_REG_BASE;//I2C1


union commondata{
		unsigned char data1;
		unsigned short data2;
		unsigned int data4;
		unsigned int data8[2];
		unsigned char c[8];
};

extern unsigned int syscall_addrtype;
extern int (*syscall1)(int type,long long addr,union commondata *mydata);
extern int (*syscall2)(int type,long long addr,union commondata *mydata);


static int syscall_i2c_addrlen;
static char syscall_i2c_chip;
static int syscall_i2c_addrlen;

int init(void);

//打开 i2c 接口
int i2c_export(unsigned int i2c);

void ls2k_i2c_writeb(__u8 val, __u8 addr);

int ls2k_i2c_readb(__u8 addr);

static void ls2k_i2c_stop(void);

static int ls2k_i2c_start(int dev_addr, int flags);

__u16 ls2k_i2c_read();

static int ls2k_i2c_write(unsigned char buf, int count);

static int i2c_transfer(struct i2c_msg *msgs, int num);

static int Myi2c_transfer(__u16 addr, __u16 flags,__u16 len,__u8 buf);

void ls2k_i2c_init(int speed,  long long slaveaddr);

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

int i2c_read(__u8 chip, __u32 addr, int alen, __u8 *buffer, int len);

__u16 Myi2c_read(__u32 addr);

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

int i2c_write(__u8 chip, __u32 addr, int alen, __u8 *buffer, int len);


int Myi2c_write(__u32 addr, __u8 buffer);

int Myi2c_ads1115_write(__u32 addr, __u8 pointADD,__u8 configH,__u8 configL);


#endif // _PWM_H ----------------------------
