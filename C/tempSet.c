#include"head.h"



uchar IapReadByte()
{
	uchar addr=IAP_ADDRESS;
    uchar dat;                       //数据缓冲区
    IAP_CONTR = ENABLE_IAP;         //使能IAP
    IAP_CMD = CMD_READ;             //设置IAP命令
    IAP_ADDRL = addr;               //设置IAP低地址
    IAP_ADDRH = addr >> 8;          //设置IAP高地址
    IAP_TRIG = 0x5a;                //写触发命令(0x5a)
    IAP_TRIG = 0xa5;                //写触发命令(0xa5)
    _nop_();                        //等待ISP/IAP/EEPROM操作完成
    dat = IAP_DATA;                 //读ISP/IAP/EEPROM数据
/*   IapIdle();                                            /*加上这行后EEPROM操作无效*/
    return dat;                     //返回
}
/*----------------------------
写一字节数据到ISP/IAP/EEPROM区域
addr:要写入的地址
dat:要写入的数据
----------------------------*/
void IapProgramByte(uchar dat)
{
	uchar addr=IAP_ADDRESS;
    IAP_CONTR = ENABLE_IAP;         //使能IAP
    IAP_CMD = CMD_PROGRAM;          //设置IAP命令
    IAP_ADDRL = addr;               //设置IAP低地址
    IAP_ADDRH = addr >> 8;          //设置IAP高地址
    IAP_DATA = dat;                 //写ISP/IAP/EEPROM数据
    IAP_TRIG = 0x5a;                //写触发命令(0x5a)
    IAP_TRIG = 0xa5;                //写触发命令(0xa5)
    _nop_();                        //等待ISP/IAP/EEPROM操作完成

/*   IapIdle();                                            /*加上这行后EEPROM操作无效*/
}
/*----------------------------
扇区擦除 某一地址addr的数据
----------------------------*/
void IapEraseSector()
{
	uchar addr=IAP_ADDRESS;
    IAP_CONTR = ENABLE_IAP;         //使能IAP
    IAP_CMD = CMD_ERASE;            //设置IAP命令
    IAP_ADDRL = addr;               //设置IAP低地址
    IAP_ADDRH = addr >> 8;          //设置IAP高地址
    IAP_TRIG = 0x5a;                //写触发命令(0x5a)
    IAP_TRIG = 0xa5;                //写触发命令(0xa5)
    _nop_();                        //等待ISP/IAP/EEPROM操作完成
                                                                
/*    IapIdle();                                        /*加上这行后EEPROM操作无效*/
}

void eepromInit()
{
	uchar addr=IAP_ADDRESS;
	uchar i;
	lowTemp=IapReadByte();
	highTemp=lowTemp>>4;
	lowTemp = lowTemp&0x0f;
	if(highTemp<=lowTemp)
	{
		highTemp=10;
		lowTemp=5;
		IapEraseSector();
		IapProgramByte(0xa5);
	}
	for(i=0;i<10;i++)
	{
		historyTemp[i]=(lowTemp+highTemp)/2;
	}
}

void keyBoardTest()
{
	uchar twoTemp;
	DQ=1;
	HSET=1;
	DelayNms(20);
	if(DQ==0)
	{
		DelayNms(20);
		if(DQ==0)
		{
			ledShow(lowTemp);
			DelayNms(800);
		}
		while(DQ==0)
		{
			lowTemp--;
			ledShow(lowTemp);
			if(lowTemp<0)
				lowTemp=15;
			DelayNms(400);
		}
	}
	if(HSET==0)
	{
		DelayNms(20);
		if(HSET==0)
		{
			ledShow(highTemp);
			DelayNms(800);
		}
		while(HSET==0)
		{
			highTemp++;
			ledShow(highTemp);
			if(highTemp>15)
				highTemp=0;
			DelayNms(500);
		}
	}
	twoTemp=highTemp*16+lowTemp;
	IapProgramByte(twoTemp);
}