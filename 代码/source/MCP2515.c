#define _SPI_C
#define _UART_C
#include"./include/MCP2515.h"
#include"./include/SPI.h"
#include"./include/config.h"
#include"./include/UART.h"

static void MCP2515_Write_Byte(unsigned char Addr,unsigned char Data,unsigned char chipnum);
static unsigned char MCP2515_Read_Byte(unsigned char Addr,unsigned char chipnum);
/*
1.函数名：MCP2515_Reset
2.说明：复位MCP2515
3.输入：芯片号
4.输出：无
5.描述：调用SPI_Send_Byte(),向MCP2515发出复位命令
*/
static void  MCP2515_Reset(unsigned char chipnum)
{
      if(chipnum==1)
	  {
	    CS1 = 0; 
	    SPI_Send_Byte(CAN_RESET,chipnum);
		CS1 = 1;
	  }
	  else if(chipnum==2)
	  {
	    CS2 = 0; 
	    SPI_Send_Byte(CAN_RESET,chipnum);
		CS2 = 1;
	  }
}
/*
*1.函数名：MCP2515_Init
*2. 描述：实现MCP2515的初始化
*3. 输入：unsigned char chipnum：要初始化的芯片号
*4. 输出：无
*5. 返回值：无
*6. 说明：无
*/
void MCP2515_Init(unsigned char chipnum)
{
    unsigned char canstate;
	/*初始化过程*/
	/*1.复位*/
	MCP2515_Reset(chipnum);
	delaynms(1);
	/*2.设置波特率和重同步宽度
	* 设总线速率=500KBps,Fosc=8MHz,每位20us=20TQ，每个TQ=1us，
	* TQ=2*BRP/Fosc,则BRP=4，取同步段=1TQ，传播段=3TQ，PS1=8TQ，PS2=8TQ
	* SJW=2TQ	   
	*/
	MCP2515_Write_Byte(CNF1,BRP|SJW_2TQ,chipnum);
	/*3.设置位时间*/
	MCP2515_Write_Byte(CNF2,BTLMODE|SAM_1|PHSEG1_8TQ|PRSEG_3TQ,chipnum);
	MCP2515_Write_Byte(CNF3,SOF|WAKFIL|PHSEG2_8TQ,chipnum);
	/*5.设置发送缓冲器0*/
	MCP2515_Write_Byte(TXB0CTRL,TXP,chipnum);
	MCP2515_Write_Byte(TXB0SIDH,SIDH,chipnum);
	MCP2515_Write_Byte(TXB0SIDL,SIDL,chipnum);
	/*6.清空接收缓冲器0的标准帧标识符*/
	
	MCP2515_Write_Byte(RXB0SIDH,0x00,chipnum);
	MCP2515_Write_Byte(RXB0SIDL,0x00,chipnum);
	/*7.设置接收缓冲器0控制寄存器*/
	MCP2515_Write_Byte(RXB0CTRL,RXM|FILHIT,chipnum);
	/*8.设置接收缓冲器0的验收滤波器,11位标识符设为11111111000B*/
	MCP2515_Write_Byte(RXF0SIDH,SIDH,chipnum);
	MCP2515_Write_Byte(RXF0SIDL,SIDL,chipnum);
	/*9.设置接收缓冲器0的屏蔽寄存器,所有位都不验证，即可以接受所有帧*/
	MCP2515_Write_Byte(RXM0SIDH,0xFF,chipnum);
	MCP2515_Write_Byte(RXM0SIDL,0xFF,chipnum);
	/*10.配置中断使能寄存器*/
	MCP2515_Write_Byte(CANINTE,RX0IE,chipnum);
	/*11.配置中断标志寄存器*/
	MCP2515_Write_Byte(CANINTF,0x00,chipnum);
	/*12.进入正常模式*/
	MCP2515_Write_Byte(CANCTRL,REQOP_NORM|OSM|CLKEN_D,chipnum);
	/*13.确认进入正常模式*/
	canstate = MCP2515_Read_Byte(CANSTAT,chipnum);
	/*未进入正常模式，重新再进一次*/
	if((canstate&(0x07<<5))) //
	{
	   	MCP2515_Write_Byte(CANCTRL,REQOP_NORM|OSM|CLKEN_D,chipnum);
	}
}
/*
1.函数名：MCP2515_Sender
2.描述：MCU向MCP2515发送数据
3.输入：unsigned char Data[]:要发送的数据,以'\0'结束,unsigned char chipnum:芯片号
4.输出：unsigned char len:发送的字节数
5.说明：MCU向MCP2515发送数据
*/
unsigned char MCP2515_Sender(unsigned char Data[],unsigned char chipnum)
{
	 unsigned char i=0;
	 /*非空才发*/
	 if(Data[0]!='\0')
	 {
		 while(Data[i]!='\0')
		 {
		      MCP2515_Write_Byte(TXB0D0+i,Data[i],chipnum);
			  ++i;
		 }
		 MCP2515_Write_Byte(TXB0DLC,i,chipnum);//向MCP2515告知要发送的字节数
		 MCP2515_Write_Byte(TXB0CTRL,TXREQ|TXP,1);//请求发送，且不破坏优先级
		 return i;
	 }
	 return 0;
}

/*
1.函数名：MCP2515_Recver
2.描述：MCU接收MCP2515发送数据
3.输入：unsigned char Data[]:数据缓冲区,unsigned char chipnum:芯片号
4.输出：unsigned char len
5.说明：MCU接收MCP2515发送数据
*/
unsigned char MCP2515_Recver(unsigned char Data[],unsigned char chipnum)
{
	 unsigned char len,i=0;
	 unsigned char canintf;	 
	 canintf = MCP2515_Read_Byte(CANINTF,chipnum);//查询该芯片有无接收中断
	 if(canintf&0x01) //有接收中断请求才读取数据
	 {
	     len = 	MCP2515_Read_Byte(RXB0DLC,chipnum);
	     len &= 0x0f;//保留数据长度位
		 while(i<len)
		 {
		     Data[i] = MCP2515_Read_Byte(RXB0D0+i,chipnum);
			 ++i;
		 }
		MCP2515_Write_Byte(CANINTF,canintf&0xFE,chipnum);//清中断标志
		return len;
	 }
	 return 0;
}

/*
*1.函数名：MCP2515_Write_Byte
*2. 描述：向MCP2515写一字节数据
*3. 输入：unsigned char Addr：要写的地址,unsigned char Data：要写的数据,
          unsigned char chipnum：选中的芯片
*4. 输出：无
*5. 返回值：无
*6. 说明：选中某个MCP2515芯片，然后向其写入数据
*/
static void MCP2515_Write_Byte(unsigned char Addr,unsigned char Data,unsigned char chipnum)
{
	  if(chipnum==1)
	  {
		  CS1 = 0;
		  SPI_Send_Byte(CAN_WRITE,chipnum);//先发出写命令
		  SPI_Send_Byte(Addr,chipnum);//再发出写地址
		  SPI_Send_Byte(Data,chipnum);//最后是要写的数据
		  CS1 = 1;
	  }
	  else if(chipnum==2)
	  {
		  CS2 = 0;
		  SPI_Send_Byte(CAN_WRITE,chipnum);//先发出写命令
		  SPI_Send_Byte(Addr,chipnum);//再发出写地址
		  SPI_Send_Byte(Data,chipnum);//最后是要写的数据
		  CS2 = 1;
	  }
}
/*
*1.函数名：MCP2515_Read_Byte
*2. 描述：从MCP2515读一字节数据
*3. 输入：unsigned char Addr：要读的地址,unsigned char chipnum：选中的芯片
*4. 输出：Data：输出数据
*5. 返回值：无
*6. 说明：选中某个MCP2515芯片，然后向其写入数据
*/
static unsigned char MCP2515_Read_Byte(unsigned char Addr,unsigned char chipnum)
{
      unsigned char Data;
      if(chipnum==1)
	  {
		  CS1 = 0;
		  SPI_Send_Byte(CAN_READ,chipnum);//先发出读命令
		  SPI_Send_Byte(Addr,chipnum);//再发出读地址
		  Data = SPI_Recv_Byte(chipnum);//读数据
		  CS1 = 1;
	  }
	  else if(chipnum==2)
	  {
		  CS2 = 0;
		  SPI_Send_Byte(CAN_READ,chipnum);//先发出读命令
		  SPI_Send_Byte(Addr,chipnum);//再发出读地址
		  Data = SPI_Recv_Byte(chipnum);//读数据
		  CS2 = 1;
	  }
	  return Data;
}





