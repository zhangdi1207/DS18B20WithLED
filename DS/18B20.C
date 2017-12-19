#include"head.h"

/**************************************
复位DS18B20,并检测设备是否存在
**************************************/
void DS18B20_Reset()
{
    CY = 1;
    while (CY)
    {
        DQ = 0;                     //送出低电平复位信号
        DelayXus(540);              //延时至少480us
        DQ = 1;                     //释放数据线
        DelayXus(60);               //等待60us
        CY = DQ;                    //检测存在脉冲
        DelayXus(240);              //等待设备释放数据线
        DelayXus(180);
    }
}

/**************************************
从DS18B20读1字节数据
**************************************/
uchar DS18B20_ReadByte()
{
    uchar i;
    uint dat = 0;

    for (i=0; i<8; i++)             //8位计数器
    {
        dat >>= 1;
        DQ = 0;                     //开始时间片
        nop();                //延时等待
        DQ = 1;                     //准备接收
        nop();                //接收延时
        if (DQ) 
			dat |= 0x80;        //读取数据
        DelayXus(60);               //等待时间片结束
    }

    return dat;
}

/**************************************
向DS18B20写1字节数据
**************************************/
void DS18B20_WriteByte(uchar dat)
{
    char i,j;

    for (i=0; i<8; i++)             //8位计数器
    {
		j=dat&0x01;
		dat>>=1;
        DQ = 0;                     //开始时间片
        nop();                //延时等待
        DQ = j;
        DelayXus(60);               //等待时间片结束
        DQ = 1;                     //恢复数据线
        nop();                //恢复延时
    }
}

int readOneTemp()
{	
	uchar a,b;
	int t;
	DS18B20_Reset(  );
	DS18B20_WriteByte(0xCC);
	DS18B20_WriteByte(0x44);   //开始转换命令
    while (!DQ);                    //等待转换完成
    DS18B20_Reset();                //设备复位
    DS18B20_WriteByte(0xCC);        //跳过ROM命令
    DS18B20_WriteByte(0xBE);

	a=DS18B20_ReadByte();
	b=DS18B20_ReadByte();
	t=b;
	t<<=4;
	a>>=4;
	t|=a;
	return t;
}

