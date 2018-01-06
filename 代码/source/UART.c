#define _SPI_C
#include"./include/UART.h"
#include"./include/SPI.h"
#include"./include/config.h"

static unsigned char UART_Recv_Data;//MCU接收到的来自PC机的一个字节
static unsigned char UART_Recv_Buff[10]={'\0'};//MCU接收到的来自PC机数据的缓冲区
static unsigned char cmd_finish=0;//接收PC机命令完成标记
/*
*1. 函数名：Uart_Init
*2. 描述：实现MCU串口的初始化
*3. 输入：无
*4. 输出：无
*5. 返回值：无
*6. 说明：1.配置串口模式(SCON) 2.配置位定时(TMOD)，定时器1做位定时器
     3.使能串口中断ES 4.使能总中断
*    每个位时间采样32次，配置完成，打开T1，就可以和PC机正常通信
*/
void Uart_Init(void)
{
   	  SCON = 0x50;//模式1，10位数据，无奇偶校验
	  TMOD = 0x20; //配置定时器为8位重装模式
	  TH1 = TL1 = 0xfd;//配置采样点，每(256-0xfd)个机器周期溢出一次，T1溢出就对串口线采样
      TR1 = 1;//开启定时器1,串口就可以收发数据了
	  ES = 1;
	  EA = 1; 
}

/*
*1.函数名：delaynms
*2.描述：延时nMs
*3.输入：n 
*4.输出：无
*5.说明：
*/
void delaynms(unsigned int i)
{
    unsigned int j=100;
    while(i--)
	{
	    while(j--)//内部延时1ms
		    _nop_();
	}
}

/*
*1. 函数名：Uart_Send_Byte
*2. 描述：MCU给PC机发送一字节数据
*3. 输入：unsigned char data
*4. 输出：无
*5. 返回值：无
*6. 说明：往MCU串口模块的发送缓冲器中写入一字节数据
*/
void Uart_Send_Byte(unsigned char Data)
{
	  SBUF = Data;
	  delaynms(1); //延时2ms，数据写入PC机后再做其他操作
}

/*
*1. 函数名：UART_Send_Data
*2. 描述：MCU把从MCP2515_2接收到的数据发送到PC机
*3. 输入：unsigned char Data[],unsigned char len
*4. 输出：实际发送的字节数
*5. 说明：调用Uart_Send_Byte()实现多字节发送
*/
unsigned char UART_Send_Data(unsigned char Data[],unsigned char len)
{
      int i=0;
	  if(len>0)
	  {
	   while(Data[i]!='\0')
	       Uart_Send_Byte(Data[i++]);
	   return i;
	  }
	  return 0;
}
/*
*1.函数名：UART_Send_Promt
*2.描述：向PC机的串口工具打印提示符，进入命令行界面
*3.输入：unsigned char* 命令提示符
*4.输出：无
*5.说明：MCU向PC机打印提示符，然后进入死循环，接收到PC机发来的命令才退出死循环
   退出时，PC机发来的命令已经拷贝到UART缓冲区。
*/
void UART_Send_Promt(unsigned char* prompt)
{
     while(*prompt!='\0')
	 {
	     SBUF = *prompt;
		 delaynms(1); //延时2ms，数据写入PC机后再做其他操作
		 ++prompt;
	 }
	 /*等待，接收到PC机发来的命令才退出死循环*/
	 while(1)
	 {
	      if(cmd_finish == 1)
		  {
		    cmd_finish = 0;
		    break;
		  }
	 }
}

/*
*1. 函数名：UART_Recv_CMD
*2. 描述：把UART命令缓冲区的命令拷贝出去
*3. 输入：unsigned char data[]
*4. 输出：拷贝的字节数,0表示缓冲区空
*5. 说明：规定PC机的命令以'#'结束，从串口输入缓冲器循环接收数据，并检测'#'
    结束循环。
*/
unsigned char UART_Recv_CMD(unsigned char Data[])
{
    int i=0;
	/*缓冲区非空才拷贝*/
	if(UART_Recv_Buff[0]!='\0')
	{
		while(UART_Recv_Buff[i]!='\0')
		{
		    Data[i] = UART_Recv_Buff[i];
			++i;
		}
		Data[i] = '\0';
		UART_Recv_Buff[0] = '\0';//拷贝完成，清空缓冲区
    	return i;
	}
	return 0;	
}
/*
*1.函数名：UART_Interrupt
*2.描述：UART串口中断函数
*3.输入：无
*4.输出：无
*5.说明：接收到PC机发来的一个字节后中断，把接收到的数据移入接收缓冲区,'#'表示结束
*/
void UART_Interrupt() interrupt 4
{
    static int i=0;
    if(RI)
	{
		RI = 0;
		UART_Recv_Data = SBUF;	
		if(UART_Recv_Data != '#')
		{
			UART_Recv_Buff[i++] = UART_Recv_Data;			
		}
		else
		{
		  cmd_finish = 1;
		  UART_Recv_Buff[i] = '\0';//添加字符串结束标记，以便操作
		  i = 0;
		}
	}
	else if(TI)
	{
	    TI = 0;
	}
}

