#include"./include/SPI.h"
#include"./include/config.h"


/*
*1.函数名：SPI_Send_Byte1
*2.描述：选中MCP2515_1，向SPI总线发送一字节数据
*3.输入：unsigned char data
*4.输出：无
*5.说明：利用SPI协议实现发送数据，主机产生一个上升沿让从机锁存数据
*  先暂时写成只针对MCP2515_1
*/
static void SPI_Send_Byte1(unsigned char Data)
{
	 int i=0;	 
	 for(i=0;i<8;++i)
	 {
		 SCK1 = 0;
		 if(Data & 0x80)
		   SI1 = 1;
		 else
		   SI1 = 0;		 
		 SCK1 = 1;
		 Data<<=1;
	 }
	 SCK1 = 0;
}

/*
*1.函数名：SPI_Send_Byte1
*2.描述：向SPI总线发送一字节数据
*3.输入：unsigned char data
*4.输出：无
*5.说明：利用SPI协议实现发送数据，主机产生一个上升沿让从机锁存数据
*  先暂时写成只针对MCP2515_2
*/
static void SPI_Send_Byte2(unsigned char Data)
{
	 int i=0;	 
	 for(i=0;i<8;++i)
	 {
		 SCK2 = 0;
		 if(Data & 0x80)
		   SI2 = 1;
		 else
		   SI2 = 0;		 
		 SCK2 = 1;
		 Data<<=1;
	 }
	 SCK1 = 0;
}

/*
*1.函数名：SPI_Recv_Byte
*2.描述：从SPI总线接收一字节数据
*3.输入：无
*4.输出：data
*5.说明：利用SPI协议实现数据接收，主机产生一个上升降沿让从机发出数据。
*  先暂时写成只针对MCP2515_1 
*/
static unsigned char SPI_Recv_Byte1()
{
    unsigned char Data=0;
	int i=0;
	SCK1 = 0;
	for(i=0;i<8;++i)
	{
	    SCK1 = 1;			
		Data<<=1;
		Data |= SO1;
		SCK1 = 0;		
	}
	return Data;
}

/*
*1.函数名：SPI_Recv_Byte
*2.描述：从SPI总线接收一字节数据
*3.输入：无
*4.输出：data
*5.说明：利用SPI协议实现数据接收，主机产生一个下降沿让从机发出数据。
*  先暂时写成只针对MCP2515_2 
*/
static unsigned char SPI_Recv_Byte2()
{
    unsigned char Data=0;
	int i=0;
	SCK2 = 0;
	for(i=0;i<8;++i)
	{
	    SCK2 = 1;
		Data<<=1;
		Data |= SO2;	
		SCK2 = 0;		
	}
	return Data;
}

/*
1.函数名：SPI_Send_Byte
2.描述：选中某个MCP2515芯片，向SPI总线发送一字节数据
3.输入：unsigned char data,unsigned char chipnum
4.输出：无
5.说明：利用SPI协议实现发送数据，主机产生一个上升沿让从机锁存数据
*/
void SPI_Send_Byte(unsigned char Data,unsigned char chipnum)
{
    if(chipnum==1)
	   SPI_Send_Byte1(Data);
    else if(chipnum==2)
	   SPI_Send_Byte2(Data);
}

/*
1.函数名：SPI_Recv_Byte
2.描述：选中某个MCP2515芯片，从SPI总线接收一字节数据
3.输入：选中的芯片编号num
4.输出：data
5.说明：利用SPI协议实现数据接收，主机产生一个下降沿让从机发出数据。
*/
unsigned char SPI_Recv_Byte(unsigned char chipnum)
{
    unsigned char Data=0;
    if(chipnum==1)
	  Data = SPI_Recv_Byte1();
    else if(chipnum==2)
	  Data = SPI_Recv_Byte2();
	return Data;
}



