#include <stdio.h>
#include <stdlib.h>
#include "I2C.h"
/*********************************
 * 设置系统参数命令用于设定 CH423 的系统级参数（写入图中的 U12）：双向输入输出引脚 IO7～IO0
的输出使能 IO_OE、输出引脚 OC7～OC0 的动态扫描使能 DEC_L、输出引脚 OC15～OC8 的动态扫描使能
DEC_H、输入电平变化中断使能 X_INT、输出引脚 OC15～OC0 开漏输出使能 OD_EN、动态显示驱动亮度
控制 INTENS、低功耗睡眠控制 SLEEP。上电复位后，上述参数默认都为 0。
IO_OE 用于控制双向输入输出引脚 IO7～IO0 的三态输出，为 0 时禁止输出（用于通过图中的 U3
进行输入），为 1 时允许输出（图中的 U5 输出）。

DEC_L 用于控制输出引脚 OC7～OC0 的动态扫描使能，为 0 时 OC7～OC0 用于 I/O 扩展的通用输出
（选择图中的 U6），为 1 时 OC7～OC0 由定时扫描计数器译码后控制（选择图中的 U10），同一时刻选
择 OC7～OC0 引脚中的某一个引脚输出低电平，其余引脚不输出，用于实现分时显示驱动。
DEC_H 用于控制输出引脚 OC15～OC8 的动态扫描使能，为 0 时 OC15～OC8 用于 I/O 扩展的通用输
出（选择图中的 U9），为 1 时 OC15～OC8 由定时扫描计数器译码后控制（选择图中的 U10），同一时刻
选择 OC15～OC8 引脚中的某一个引脚输出低电平，其余引脚不输出，用于实现分时显示驱动。
X_INT 用于使能输入电平变化中断（控制图中的 U14），为 0 时禁止电平变化中断，为 1 并且 DEC_H
为 0 时允许从 OC15 引脚输出电平变化中断（由图中的 U2 比较产生）。
OD_EN 用于使能输出引脚 OC15～OC0 的开漏输出，为 0 时 OC15～OC0 是推挽输出（可以输出低电
平和高电平），为 1 时 OC15～OC0 是开漏输出（只能输出低电平和不输出）。
INTENS 用于控制动态显示驱动的亮度，包含两位数据，有 4 种组合：数据 00B、01B、10B 分别
设置显示驱动占空比为 4/4、1/4、2/4，并且启用内部的段驱动限流；数据 11B 设置显示驱动占空比
为 4/4，但是禁止内部的段驱动限流，所以外部需要在段引脚串接限流电阻 R1～R8。
SLEEP 用于使 CH423 进入低功耗睡眠状态，从而可以节约电能。处于低功耗睡眠状态中的 CH423
可以被下述两种事件中的任何一种唤醒，第一种事件是输入电平变化，即检测到 IO7～IO0 引脚输入
的当前状态与事先写入 IO7～IO0 引脚的输出寄存器（图中的 U4）中的数据不同；第二种事件是接收
到单片机发出的下一个操作命令。当 CH423 被唤醒后，SLEEP 位会自动清 0。睡眠和唤醒操作本身不
会影响 CH423 的其它工作状态。如果是被前一种事件唤醒，那么唤醒同时将产生电平变化中断。
例如，输出字节 2 为 17H 时，那么 OC15～OC0 以开漏方式动态驱动 16 个数码管；输出字节 2 为
03H 时，仅 OC7～OC0 动态驱动 8 个数码管，剩余的 OC15～OC8 仍然用于通用输出 GPO；输出字节 2 为
05H 时，仅 OC15～OC8 动态驱动 8 个数码管，剩余的 OC7～OC0 仍然用于通用输出 GPO。
 */
#define SYS_ADDR     0x24

#define SYS_SLEEP    0x40
#define SYS_INTENS   0x20
#define SYS_OD_EN    0x10
#define SYS_X_INT    0x08
#define SYS_DEC_H    0x04
#define SYS_DEC_L    0x02
#define SYS_IO_OE    0x01

/*********************************
 * 输出字节 2 为[OC_L_DAT]B，即 00H 到 0FFH 之间的 8 位数据，用于
写入输出引脚 OC7～OC0 的输出寄存器（图中的 U6），写 0 则使引脚输出低电平，写 1 则输出高电平。
   输出字节 2 为[OC_H_DAT]B，即 00H 到 0FFH 之间的 8 位数据，用于
写入输出引脚 OC15～OC8 的输出寄存器（图中的 U9），写 0 则使引脚输出低电平，写 1 则输出高电平。
 */
#define OCL_ADDR   0x22
#define OCH_ADDR   0x23

/*
 * 该命令的输出字节 1 为 60H、62H、64H、66H、68H、6AH、6CH、6EH、70H、72H、74H、76H、78H、
7AH、7CH、7EH，其中位 4～位 1 为地址，可以忽略；输出字节 2 为[IO_DAT]B，即 00H 到 0FFH 之间
的 8 位数据，用于写入双向输入输出引脚 IO7～IO0 的输出寄存器（图中的 U4），如果 IO_OE 为 1 允
许输出，那么写 0 则使引脚输出低电平，写 1 则使引脚输出高电平。

 */
#define IO0_ADDR   0x30
#define IO1_ADDR   0x31
#define IO2_ADDR   0x32
#define IO3_ADDR   0x33
#define IO4_ADDR   0x34
#define IO5_ADDR   0x35
#define IO6_ADDR   0x36
#define IO7_ADDR   0x37

/*
 * 该命令的输出字节 1 为 01001101B，即 4DH；输入字节 2 为双向输入输出引脚 IO7～IO0 的当前
引脚状态。
读取双向输入输出命令用于获得 IO7～IO0 引脚的当前状态，当 IO_OE 为 0 时为获取输入状态，
否则为获取输出状态。该命令属于读操作，是唯一的具有数据返回的命令，单片机必须先释放 SDA 引
脚（三态输出禁止或者上拉到高电平），然后 CH423 从 SDA 引脚输出当前引脚状态。
当 CH423 用于数码管显示驱动时，可以在临时关闭显示后，读取外部的 8 个按键输入状态。
 */

#define IO_READ_ADDR 0x26

__u8 ocl=0;
__u8 och=0;


void oc_output(__u8 b, __u8 value){
  if(b<=7 && (value==0||value==1)){
    if(value==0){
      ocl&=~(0x01<<b);
    }else{
      ocl|=(0x01<<b);
    }
    Myi2c_write(OCL_ADDR, ocl);
    //i2c_write(OCL_ADDR,ocl);
  }else if(b<=15 && (value==0||value==1)){
    if(value==0){
      och&=~(0x01<<(b-8));
    }else{
      och|=(0x01<<(b-8));
    }
    Myi2c_write(OCH_ADDR,och);
    //i2c_write(OCH_ADDR,och);
  }
}

void dot_output(__u8 x,__u8  y,__u8 value){
  
}
const __u8  BCD_decode_tab[ 29 ] = { 0X3F, 0X06, 0X5B, 0X4F, 0X66, 0X6D, 0X7D, 0X07, 0X7F, 0X6F, 0X77, 0X7C, 0X58, 0X5E, 0X79, 0X71, 0x00, 0x46, 0x40, 0x41, 0x39, 0x0F, 0x08, 0x76, 0x38, 0x73, 0x80, 0xFF, 0x00 };
const __u8  BCD_addr_tab[8]={IO4_ADDR,IO5_ADDR,IO6_ADDR,IO7_ADDR,IO0_ADDR,IO1_ADDR,IO2_ADDR,IO3_ADDR};
void number_write(__u8  b,__u8  num){
  if(num >=28 || b>=8){
    return ;
  }
  Myi2c_write(BCD_addr_tab[b], BCD_decode_tab[num]);

}


int main(int argc,char **argv)
{
    int i2c= 0;
    __u8  i=0;
	volatile int bus;
	bus=SYS_ADDR;
    
    init();
    i2c_export(i2c);
	ls2k_i2c_init(0, LS2K1000_I2C0_REG_BASE);
    Myi2c_write(SYS_ADDR,SYS_IO_OE|SYS_DEC_L);
    //i2c_write(SYS_ADDR,0);
    Myi2c_write(OCL_ADDR, ocl);
    Myi2c_write(OCH_ADDR, och);
    while(1)
    {
        number_write(0,i);
        i++;
        usleep(1300000);
        //oc_output(8,1);
        // put your main code here, to run repeatedly:
        //usleep(1300000);
        //oc_output(8,0);
        //Serial.println(i2c_read(IO_READ_ADDR));
    }
	return 0;
}

