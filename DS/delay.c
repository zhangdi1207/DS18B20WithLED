#include"head.h"

/**************************************
ÑÓÊ±XÎ¢Ãë(STC15F104E@12M)
**************************************/
// *
void DelayXus(uint n)                  //LCALL 6
{
	n>>=2;
    while (n--)                        //DJNZ  4
    {
        _nop_();                       //NOP  1+1=12
        _nop_();                     
    }
}
//*/
/*
void DelayXus(uint n)                  //LCALL 6
{
    while (n--);                        //DJNZ  4
}
//*/
void DelayNms(uint n)
{
	WDT_CONTR = WDT;
	while(n--)
	{
		DelayXus(1000);
	}
}
