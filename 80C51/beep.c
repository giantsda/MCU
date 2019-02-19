#include <reg51.h>
sbit Beep =  P1^5 ;
void delay(unsigned int i) ;
void main()
{
	int i,j;
	for (i=5;i<=500;i++)
		for (j=0;j<=50;j++)
		{ 
			Beep= 1;
			delay(i);
			Beep= 0;
			delay(i);
		}
} 

 void delay(unsigned int i)
{
    char j;
    for(i; i > 0; i--)
        for(j = 200; j > 0; j--);
}

