#include"head.h"

int lowTemp=5;
int highTemp=10;

void IapIdle()
{
	IAP_CONTR = 0;         //使能IAP
    IAP_CMD = 0; 
	IAP_TRIG = 0;            //设置IAP命令
    IAP_ADDRH = 0x80;          //设置IAP高地址
	IAP_ADDRL = 0;               //设置IAP低地址
}


uchar IapReadByte(uint addr)
{
    uchar dat;                       //数据缓冲区
    IAP_CONTR = ENABLE_IAP;         //使能IAP
    IAP_CMD = CMD_READ;             //设置IAP命令
    IAP_ADDRL = addr;               //设置IAP低地址
    IAP_ADDRH = addr >> 8;          //设置IAP高地址
    IAP_TRIG = 0x5a;                //写触发命令(0x5a)
    IAP_TRIG = 0xa5;                //写触发命令(0xa5)
    DelayXus(40);                        //等待ISP/IAP/EEPROM操作完成
    dat = IAP_DATA;                 //读ISP/IAP/EEPROM数据
    IapIdle();                                            /*加上这行后EEPROM操作无效*/
    return dat;                     //返回
}
/*----------------------------
写一字节数据到ISP/IAP/EEPROM区域
addr:要写入的地址
dat:要写入的数据
----------------------------*/
void IapProgramByte(uint addr,uchar dat)
{
    IAP_CONTR = ENABLE_IAP;         //使能IAP
    IAP_CMD = CMD_PROGRAM;          //设置IAP命令
    IAP_ADDRL = addr;               //设置IAP低地址
    IAP_ADDRH = addr >> 8;          //设置IAP高地址
    IAP_DATA = dat;                 //写ISP/IAP/EEPROM数据
    IAP_TRIG = 0x5a;                //写触发命令(0x5a)
    IAP_TRIG = 0xa5;                //写触发命令(0xa5)
    DelayXus(40);                        //等待ISP/IAP/EEPROM操作完成

    IapIdle();                                            /*加上这行后EEPROM操作无效*/
}
/*----------------------------
扇区擦除 某一地址addr的数据
----------------------------*/
void IapEraseSector(uint addr)
{
    IAP_CONTR = ENABLE_IAP;         //使能IAP
    IAP_CMD = CMD_ERASE;            //设置IAP命令
    IAP_ADDRL = addr;               //设置IAP低地址
    IAP_ADDRH = addr >> 8;          //设置IAP高地址
    IAP_TRIG = 0x5a;                //写触发命令(0x5a)
    IAP_TRIG = 0xa5;                //写触发命令(0xa5)
    DelayXus(40);                        //等待ISP/IAP/EEPROM操作完成
                                                                
    IapIdle();                                        /*加上这行后EEPROM操作无效*/
}

void eepromInit()
{
	uchar i;
	lowTemp=IapReadByte(LOW_ADDRESS);
	highTemp=IapReadByte(HIGH_ADDRESS);
	if(highTemp<=lowTemp)
	{
		highTemp=10;
		lowTemp=5;
		IapEraseSector(LOW_ADDRESS);
		//IapEraseSector(HIGH_ADDRESS);
		IapProgramByte(LOW_ADDRESS,lowTemp);
		IapProgramByte(HIGH_ADDRESS,highTemp);
	}
	for(i=0;i<10;i++)
	{
		historyTemp[i]=(lowTemp+highTemp)/2;
	}
}

void keyBoardTest()
{
	int oldLow,oldHigh;
	uchar changeFlag=0;
	oldLow=lowTemp;
	oldHigh=highTemp;

	HSET=0;
	DelayNms(20);
	if(HSET==1)
	{
		HSET=0;
		DelayNms(20);
		if(HSET==1)
		{
			HSET=0;
			ledShow(lowTemp);
			DelayNms(1500);
		}
		while(HSET==1)
		{
			HSET=0;
			lowTemp--;
			ledShow(lowTemp);
			if(lowTemp<0)
				lowTemp=29;
			DelayNms(800);
			changeFlag=1;
		}
	}
	HSET=1;
	DelayNms(20);
	if(HSET==0)
	{
		HSET=1;
		DelayNms(20);
		if(HSET==0)
		{	HSET=1;
			ledShow(highTemp);
			DelayNms(1500);
		}
		while(HSET==0)
		{
			HSET=1;
			highTemp++;
			ledShow(highTemp);
			if(highTemp>29)
				highTemp=0;
			DelayNms(800);
			changeFlag=1;
		}
	}
	if(changeFlag)
	{
		changeFlag=0;
		if(lowTemp != oldLow)
		{
			IapEraseSector(LOW_ADDRESS);
			IapProgramByte(LOW_ADDRESS,lowTemp);
			IapProgramByte(HIGH_ADDRESS,highTemp);
		}
		DelayNms(1);
		if(highTemp!=oldHigh)
		{
			IapEraseSector(HIGH_ADDRESS);
			IapProgramByte(LOW_ADDRESS,lowTemp);
			IapProgramByte(HIGH_ADDRESS,highTemp);
		}
		DelayNms(1);
	}
}