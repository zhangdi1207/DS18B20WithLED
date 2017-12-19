#ifndef _HEAD_H_
#define _HEAD_H_
#include <reg52.h>
#include <intrins.h>
#define nop() _nop_()
#define uchar unsigned char
#define uint unsigned int
#define CMD_IDLE    0               //����ģʽ
#define CMD_READ    1               //IAP�ֽڶ�����
#define CMD_PROGRAM 2               //IAP�ֽڱ������
#define CMD_ERASE   3  
#define ENABLE_IAP  0x82            //if SYSCLK<20MHz
#define LOW_ADDRESS 0x0000
#define HIGH_ADDRESS 0x0001
#define WDT			0x35			//���Ź���ʼ��@12MHz��2.9071s

sfr AUXR        =   0x8e;
sfr IAP_DATA    =   0xC2;           //IAP���ݼĴ���
sfr IAP_ADDRH   =   0xC3;           //IAP��ַ�Ĵ������ֽ�
sfr IAP_ADDRL   =   0xC4;           //IAP��ַ�Ĵ������ֽ�
sfr IAP_CMD     =   0xC5;           //IAP����Ĵ���
sfr IAP_TRIG    =   0xC6;           //IAP������Ĵ���
sfr IAP_CONTR   =   0xC7;           //IAP���ƼĴ���
sfr CLK_DIV = 0x97;					//ʱ������Ĵ���
sfr WDT_CONTR = 0xc1;

sbit LED = P3^0;
sbit CLK = P3^1;
sbit sCLK = P3^2;
sbit DQ = P3^4;
sbit OUT = P3^3;
sbit HSET = P3^5;


void ledShow(int n);

void DelayXus(uint n);
void DelayNms(uint n);

void DS18B20_Reset();
void DS18B20_WriteByte(uchar dat);
int readOneTemp(void);

void eepromInit();
void keyBoardTest();

void outSet();

extern int lowTemp;
extern int highTemp;
extern int historyTemp[];

#endif