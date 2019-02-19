#include<reg51.h>
#include<intrins.h>	

sbit K1=P0^0;
void Delay10ms( );   //??10ms

void main(void)
{
	unsigned int i,j;
	j=0x01;
	
	while(1)
	{	
		P2=j;	 
		if(K1==0)		 
		{
			Delay10ms();	 
			if(K1==0)
			{
				if (j==0x80)
					j=0x01;
				else
					j=j<<1; 

				while((i<50)&&(K1==0))	  
				{
					Delay10ms();
					i++;
				}
				i=0;
			}
		}		
	}				
}
 
void Delay10ms(void)    
{
    unsigned char a,b,c;
    for(c=1;c>0;c--)
        for(b=38;b>0;b--)
            for(a=130;a>0;a--);
}