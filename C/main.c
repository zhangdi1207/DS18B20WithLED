#include "head.h"

uchar lowTemp=5;
uchar highTemp=10;
int historyTemp[]={6,6,6,6,6,6,6,6,6,6};

//*
void main()
{
	uchar i,j;
	LED=0;
	DQ = 1;
	OUT = 0; 
	eepromInit();
	//CLK_DIV=CLK_DIV|0x07;
	while(1)
	{
		i=i%10;
		historyTemp[i]=readOneTemp();

		ledShow(historyTemp[i]);
		for(j=0;j<10;j++)
			keyBoardTest();
		outSet();
		   
		i++;
		
		DelayNms(500); 

	}
}
//*/

/*
void main()
{
	uint i=10,j,k=10;
	uchar dat=0x44;
	while(1)
	{
		 	DS18B20_Reset(  );
	DS18B20_WriteByte(0xCC);
	DS18B20_WriteByte(0x44);   //开始转换命令
    while (!DQ);                    //等待转换完成
    DS18B20_Reset();                //设备复位
    DS18B20_WriteByte(0xCC);        //跳过ROM命令
    DS18B20_WriteByte(0xBE);
	dat=DS18B20_ReadByte();
	dat>>=4;
	ledShow(dat);
	DelayNms(500);
	//ledShow(1);
	//DelayNms(500);
	}
}
//*/
