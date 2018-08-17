 
#include<reg51.h>
#include<intrins.h>

#define uchar unsigned char  
sbit SCK=P3^6;
sbit LCK=P3^5;
sbit SDI=P3^4;

void HC595_Send(uchar dat1,uchar dat0,uchar dat3,uchar dat2);

void main()
{
	while(1)
	{	
		HC595_Send(0x7f,0xff,0x80,0x00);
	}	
}

//   3
//	 2
//1 0

void HC595_Send(uchar dat1,uchar dat0,uchar dat3,uchar dat2)
{
	uchar i;
	SCK=0;
	LCK=0;
	for(i=0;i<8;i++)
	{
		SDI=dat0&0x01;		 
		dat0>>=1;
		SCK=0;
		_nop_();
		_nop_();
		SCK=1;
		_nop_();
		_nop_();	
	}
	for(i=0;i<8;i++)
	{
		SDI=dat1&0x01;		 
		dat1>>=1;
		SCK=0;
		_nop_();
		_nop_();
		SCK=1;
		_nop_();
		_nop_();	
	}
	for(i=0;i<8;i++)
	{
		SDI=dat2&0x01;		 
		dat2>>=1;
		SCK=0;
		_nop_();
		_nop_();
		SCK=1;
		_nop_();
		_nop_();	
	}
	for(i=0;i<8;i++)
	{
		SDI=dat3&0x01;		 
		dat3>>=1;
		SCK=0;
		_nop_();
		_nop_();
		SCK=1;
		_nop_();
		_nop_();	
	}
	LCK=0;	 
	_nop_();
	_nop_();
	LCK=1;  // flush the data
	_nop_();
	_nop_();
}
