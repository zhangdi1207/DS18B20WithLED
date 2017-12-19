#include"head.h"

int lowTemp=5;
int highTemp=10;

void IapIdle()
{
	IAP_CONTR = 0;         //ʹ��IAP
    IAP_CMD = 0; 
	IAP_TRIG = 0;            //����IAP����
    IAP_ADDRH = 0x80;          //����IAP�ߵ�ַ
	IAP_ADDRL = 0;               //����IAP�͵�ַ
}


uchar IapReadByte(uint addr)
{
    uchar dat;                       //���ݻ�����
    IAP_CONTR = ENABLE_IAP;         //ʹ��IAP
    IAP_CMD = CMD_READ;             //����IAP����
    IAP_ADDRL = addr;               //����IAP�͵�ַ
    IAP_ADDRH = addr >> 8;          //����IAP�ߵ�ַ
    IAP_TRIG = 0x5a;                //д��������(0x5a)
    IAP_TRIG = 0xa5;                //д��������(0xa5)
    DelayXus(40);                        //�ȴ�ISP/IAP/EEPROM�������
    dat = IAP_DATA;                 //��ISP/IAP/EEPROM����
    IapIdle();                                            /*�������к�EEPROM������Ч*/
    return dat;                     //����
}
/*----------------------------
дһ�ֽ����ݵ�ISP/IAP/EEPROM����
addr:Ҫд��ĵ�ַ
dat:Ҫд�������
----------------------------*/
void IapProgramByte(uint addr,uchar dat)
{
    IAP_CONTR = ENABLE_IAP;         //ʹ��IAP
    IAP_CMD = CMD_PROGRAM;          //����IAP����
    IAP_ADDRL = addr;               //����IAP�͵�ַ
    IAP_ADDRH = addr >> 8;          //����IAP�ߵ�ַ
    IAP_DATA = dat;                 //дISP/IAP/EEPROM����
    IAP_TRIG = 0x5a;                //д��������(0x5a)
    IAP_TRIG = 0xa5;                //д��������(0xa5)
    DelayXus(40);                        //�ȴ�ISP/IAP/EEPROM�������

    IapIdle();                                            /*�������к�EEPROM������Ч*/
}
/*----------------------------
�������� ĳһ��ַaddr������
----------------------------*/
void IapEraseSector(uint addr)
{
    IAP_CONTR = ENABLE_IAP;         //ʹ��IAP
    IAP_CMD = CMD_ERASE;            //����IAP����
    IAP_ADDRL = addr;               //����IAP�͵�ַ
    IAP_ADDRH = addr >> 8;          //����IAP�ߵ�ַ
    IAP_TRIG = 0x5a;                //д��������(0x5a)
    IAP_TRIG = 0xa5;                //д��������(0xa5)
    DelayXus(40);                        //�ȴ�ISP/IAP/EEPROM�������
                                                                
    IapIdle();                                        /*�������к�EEPROM������Ч*/
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