#ifndef _HEAD_H_
#define _HEAD_H_
#include <reg52.h>
#include <intrins.h>
#define nop() _nop_()
#define uchar unsigned char
#define uint unsigned int
#define CMD_IDLE    0               //¿ÕÏĞÄ£Ê½
#define CMD_READ    1               //IAP×Ö½Ú¶ÁÃüÁî
#define CMD_PROGRAM 2               //IAP×Ö½Ú±à³ÌÃüÁî
#define CMD_ERASE   3  
#define ENABLE_IAP  0x82            //if SYSCLK<20MHz
#define IAP_ADDRESS 0x0000

sfr AUXR        =   0x8e;
sfr IAP_DATA    =   0xC2;           //IAPÊı¾İ¼Ä´æÆ÷
sfr IAP_ADDRH   =   0xC3;           //IAPµØÖ·¼Ä´æÆ÷¸ß×Ö½Ú
sfr IAP_ADDRL   =   0xC4;           //IAPµØÖ·¼Ä´æÆ÷µÍ×Ö½Ú
sfr IAP_CMD     =   0xC5;           //IAPÃüÁî¼Ä´æÆ÷
sfr IAP_TRIG    =   0xC6;           //IAPÃüÁî´¥·¢¼Ä´æÆ÷
sfr IAP_CONTR   =   0xC7;           //IAP¿ØÖÆ¼Ä´æÆ÷
sfr CLK_DIV = 0x97;					//Ê±ÖÓÃüÁî¼Ä´æÆ÷

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

extern uchar lowTemp;
extern uchar highTemp;
extern int historyTemp[];

#endif