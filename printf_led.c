 

#include <reg51.h>	   

void delay(unsigned int i);  
void printf(int number,int time);
sbit b1=P2^5;
sbit s1=P1^4;

unsigned char ledchar[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,
0xf8,0x80,0x90,0x88,0x83,0xc6,0xa1,0x86,0x8e};
void main()
{ 
	int i,j,k;
	i=0;
	while (1)
	{
		printf(i,2);
		i++;
	}
  
}

/*******????*************/
void delay(unsigned int i)
{
    unsigned char j;
    for(i; i > 0; i--)   //?? 600*255 ? 
        for(j = 100; j > 0; j--);
}

void printf(int number,int time)
{
	int i,j;
	unsigned char c=0x01;
	int ledbuff[7]={0,0,0,0,0,0,0};
	ledbuff[1]=number%10;
	ledbuff[2]=(number/10)%10;
	ledbuff[3]=(number/100)%10;
	ledbuff[4]=(number/1000)%10;
	ledbuff[5]=(number/10000)%10;
	ledbuff[6]=(number/100000)%10;
	for(j=1;j<time;j++)
	{
		for (i=1;i<7;i++)
				{
					P1=~(c<<i);	
					P2=~ledchar[ledbuff[i]];
					delay(2);
				}
	}
}	

 
