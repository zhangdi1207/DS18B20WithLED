#include"head.h"

void outSet()
{
	uchar i;
	if(OUT==0)
	{
		for(i=0;i<10;i++)
		{
			if(historyTemp[i]>lowTemp)
				break;

		}
		if(i ==10)
			OUT=1;
	}
	if(OUT==1)
	{
		for(i=0;i<10;i++)
		{
			if(historyTemp[i]<highTemp)
				break;
		}
		if(i==10)
			OUT=0;
	}
}