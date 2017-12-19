#include"head.h"



uchar IapReadByte()
{
	uchar addr=IAP_ADDRESS;
    uchar dat;                       //���ݻ�����
    IAP_CONTR = ENABLE_IAP;         //ʹ��IAP
    IAP_CMD = CMD_READ;             //����IAP����
    IAP_ADDRL = addr;               //����IAP�͵�ַ
    IAP_ADDRH = addr >> 8;          //����IAP�ߵ�ַ
    IAP_TRIG = 0x5a;                //д��������(0x5a)
    IAP_TRIG = 0xa5;                //д��������(0xa5)
    _nop_();                        //�ȴ�ISP/IAP/EEPROM�������
    dat = IAP_DATA;                 //��ISP/IAP/EEPROM����
/*   IapIdle();                                            /*�������к�EEPROM������Ч*/
    return dat;                     //����
}
/*----------------------------
дһ�ֽ����ݵ�ISP/IAP/EEPROM����
addr:Ҫд��ĵ�ַ
dat:Ҫд�������
----------------------------*/
void IapProgramByte(uchar dat)
{
	uchar addr=IAP_ADDRESS;
    IAP_CONTR = ENABLE_IAP;         //ʹ��IAP
    IAP_CMD = CMD_PROGRAM;          //����IAP����
    IAP_ADDRL = addr;               //����IAP�͵�ַ
    IAP_ADDRH = addr >> 8;          //����IAP�ߵ�ַ
    IAP_DATA = dat;                 //дISP/IAP/EEPROM����
    IAP_TRIG = 0x5a;                //д��������(0x5a)
    IAP_TRIG = 0xa5;                //д��������(0xa5)
    _nop_();                        //�ȴ�ISP/IAP/EEPROM�������

/*   IapIdle();                                            /*�������к�EEPROM������Ч*/
}
/*----------------------------
�������� ĳһ��ַaddr������
----------------------------*/
void IapEraseSector()
{
	uchar addr=IAP_ADDRESS;
    IAP_CONTR = ENABLE_IAP;         //ʹ��IAP
    IAP_CMD = CMD_ERASE;            //����IAP����
    IAP_ADDRL = addr;               //����IAP�͵�ַ
    IAP_ADDRH = addr >> 8;          //����IAP�ߵ�ַ
    IAP_TRIG = 0x5a;                //д��������(0x5a)
    IAP_TRIG = 0xa5;                //д��������(0xa5)
    _nop_();                        //�ȴ�ISP/IAP/EEPROM�������
                                                                
/*    IapIdle();                                        /*�������к�EEPROM������Ч*/
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