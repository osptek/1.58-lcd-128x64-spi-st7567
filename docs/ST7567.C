//*****************************************************************
//CONTROL IC:UC1701X=ST7565=SED1815=SSD1815
//MODEL TYPE:128*64
//MPU INTERFACE:4-spi , 6800, 8080

//*****************************************************************
#include <reg52.h>
#include "ST7567.H"
#include "DELAY1.H"


//----------------------------------------------------------
void Init_LCD(void)
  {
   CS1=0;
   RST=0;
   Delay(200);
   RST=1;
   Delay(300);

   WriteData(0xE2,0); /*软复位*/
   Delay(5);
   WriteData(0x2C,0); /*升压步骤 1*/
   Delay(5);
   WriteData(0x2E,0); /*升压步骤 2*/
   Delay(5);
   WriteData(0x2F,0); /*升压步骤 3*/
   Delay(5);
   WriteData(0x25,0); /*粗调对比度值 0x20~0x27*/
		
   WriteData(0x81,0); /*对比度微调指令*/
   WriteData(0x1A,0); /*0x1a,对比度微调值 0x00~0x3f*/
		
   WriteData(0xA2,0); /*1/9 bias*/
   WriteData(0xC8,0); /*行扫描*/
   WriteData(0xA0,0); /*列扫描*/
   WriteData(0x40,0); /*起始行*/
   WriteData(0xAF,0); /*开显示*/
   CS1=1;

  }

//-----------------------------液晶驱动函数-----------------------------

	/*
void WriteData(Uchar data1, bit di)	   //串口时序 ,当使用串口时把 <uc1701.h>中的并口屏蔽掉，打开串口定义
{
       Uchar i,temp;
        CS1=0;
		A0=di;
		for(i=0;i<8;i++)
		    {
                  SCK=0;
		  temp=data1;
		  SI=(bit)(temp&0x80);
          	
		  somenop();
	      SCK=1;
          temp=data1<<1;
	      data1=temp; 
 		     } 
        CS1=1; 
		A0=~di;   
}

*/

void WriteData(Uchar data1, bit di)	 //并口6800时序,模块与单片机的接口定义看<uc1701.h>中的定义，当 IM0口接高电平时为6800时序
{   
    A0  = di;
	CS1 = 0;
	RW  = 0;
	E   = 1;  		Delay(1);
	DATABUS = data1;	 Delay(2);
	E   = 0;		Delay(2);
	CS1	= 1; 
}
 /*

void WriteData(Uchar data1, bit di)	 //并口8080时序，当IM0口接低电平时为8080时序，当使用并口时把 <uc1701.h>中的串口屏蔽掉，打开并口定义
{   
    A0  = di;
	CS1 = 0;
	E   = 1;  	
	RW  = 0;		Delay(1);
	DATABUS = data1;	 Delay(2);
	RW  = 1;		Delay(2);
	CS1	= 1; 
}
*/
/*void  Waitkey(void)
{
  M_Delay(200);
  while(KEY);
  M_Delay(200);
}*/



/*void sleepmode(unsigned char *p)
{
  unsigned char i;
  for(i=0;i<2;i++)
   {
    WriteData(p[i],0);
   }
   Delay(400);
}*/
/*
//----------------------------------------------------------

//----------------------------------------------------------
void Displine(Uchar array[])
  {
   Uint i,j,k;
   for(i=4;i<8;i++)
   {
    WriteData(0x40,0);
	WriteData(0x01,0);	 //SEG起始地址
	WriteData(0xb0|i,0);//set page address
    WriteData(0x10,0); //column  msb
    WriteData(0x01,0); //column  lsb	
    for(j=0;j<32;j++)
    for(k=0;k<4;k++)
    {
     WriteData(array[k],1);
    }
   }
      for(i=0;i<4;i++)
   {
    WriteData(0x40,0);
	WriteData(0x01,0);	 
	WriteData(0xb0|i,0);//set page address
    WriteData(0x10,0); //column  msb  //SEG起始地址H
    WriteData(0x01,0); //column  lsb //SEG起始地址L	
    for(j=0;j<32;j++)
    for(k=0;k<4;k++)
    {
     WriteData(array[k],1);
    }
   }
  }

//----------------------------------------------------------
void display8(Uchar a[])
   {unsigned int i,j,k;
    for(i=4;i<8;i++)
	{ 
	  WriteData(0x40,0);
	  WriteData(0x01,0);  
	  WriteData(i|0xb0,0);
	  WriteData(0x10,0);   //SEG起始地址H
	  WriteData(0x01,0);  //SEG起始地址L
	 for(j=0;j<16;j++)
	  {
	   for(k=0;k<8;k++)
	   { WriteData(a[k],1);} 
	   
	  }	    
    }
	for(i=0;i<4;i++)
	{ 
	  WriteData(0x40,0);
	  WriteData(0x01,0);  
	  WriteData(i|0xb0,0);
	  WriteData(0x10,0);
	  WriteData(0x01,0);	//SEG起始地址
	 for(j=0;j<16;j++)
	  {
	   for(k=0;k<8;k++)
	   { WriteData(a[k],1);} 
	   
	  }
	    
    }	
   }
//----------------------------------------------------------
void clearlcd(void)          //清屏
   {unsigned int i,j,k;
    for(i=0;i<8;i++)
	{ 
	  WriteData(0x40,0);
	  WriteData(0x01,0);   
	  WriteData(i|0xb0,0);
	  WriteData(0x10,0);	//SEG起始地址H
	  WriteData(0x01,0);	//SEG起始地址L
	 for(j=0;j<16;j++)
	  {
	   for(k=0;k<8;k++)
	   { WriteData(0x00,1);} 
	   
	  }
	    
    }	
   }
//----------------------------------------------------------

void Dispgraphic(Uchar *p)        //显示图片
    {
	   Uchar i,j;
	   for(i=4;i<8;i++)
		   {
			    WriteData(0x40,0);
				WriteData(0x01,0);

			    WriteData(0xb0|i,0);//页地址

			    WriteData(0x10,0);	//SEG起始地址H
			    WriteData(0x01,0);	//SEG起始地址L
			    for(j=0;j<128;j++)
				    {
				     WriteData(p[(i-4)*128+j],1);
			         }
           }
		   for(i=0;i<4;i++)
		   {
			    WriteData(0x40,0);
				WriteData(0x01,0);	

			    WriteData(0xb0|i,0);//页地址

			    WriteData(0x10,0);	 //SEG起始地址H
			    WriteData(0x01,0);	//SEG起始地址L
			    for(j=0;j<128;j++)
				    {
				     WriteData(p[(i+4)*128+j],1);
			         }
           }

  }
*/
//----------------------------------------------------------
void Displine(Uchar array[])
  {
   Uint i,j,k;
   for(i=0;i<8;i++)
   {
    WriteData(0x40,0);	 //Set Scroll line
	WriteData(0xb0|i,0);//set page address
    WriteData(0x10,0); //column  msb
    WriteData(0x00,0); //column  lsb	
    for(j=0;j<33;j++)
    for(k=0;k<4;k++)
    {
     WriteData(array[k],1);
    }
   }
  }

//----------------------------------------------------------
void display8(Uchar a[])
   {unsigned int i,j,k;
    for(i=0;i<8;i++)
	{ 
	  WriteData(0x40,0);
	  WriteData(i|0xb0,0);
	  WriteData(0x10,0);
	  WriteData(0x00,0);
	 for(j=0;j<17;j++)
	  {
	   for(k=0;k<8;k++)
	   { WriteData(a[k],1);} 
	   
	  }
	    
    }	
   }
//----------------------------------------------------------
void clearlcd(void)
   {unsigned int i,j,k;
    for(i=0;i<9;i++)
	{ 
	  WriteData(0x40,0);
	  WriteData(0x01,0);
	  WriteData(i|0xb0,0);
	  WriteData(0x10,0);
	  WriteData(0x00,0);
	 for(j=0;j<16;j++)
	  {
	   for(k=0;k<8;k++)
	   { WriteData(0x00,1);} 
	   
	  }
	    
    }	
   }

//----------------------------------------------------------

void Dispgraphic(Uchar *p)
     {
	   Uchar i,j;
		   for(i=0;i<8;i++)
		   {
			    WriteData(0x40,0);
			    WriteData(0xb0|i,0);
			    WriteData(0x10,0);
			    WriteData(0x00,0);
			    for(j=0;j<128;j++)
				    {
				     WriteData(p[i*128+j],1);
			         }
           }

  }
	