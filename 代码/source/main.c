#define _UART_C
#define _MCP2515_C
#include"./include/UART.h"
#include"./include/MCP2515.h"
#include"./include/config.h"

static unsigned char PC_Cmd[10];//PC机发给MCU的命令
static unsigned char CAN_Cmd[10];//MCU从CAN总线读回的命令
static code unsigned  char err1[] = "faile write to MCP2515_1\r\n";
static code unsigned  char err2[] = "faile read from MCP2515_2\r\n";
static code unsigned  char err3[] = "no recieve cmd\r\n";
static code unsigned  char msg1[] = "\r\nys# ";
bit CAN_Flag = 0;
/*
*1.函数名：EXTI1_init
*2.描述：配置外部中断1
*3.输入：无
*4.输出：无
*5.说明：
*/
void EXTI1_init()
{
	 IT1 = 1;//触发方式为下降沿触发
	 ET0 = 1;
	 EA = 1;
}

/*
*1. 函数名：main
*2. 描述：实现程序的主流程
*3. 输入：无
*4. 输出：无
*5. 返回值：无
*6. 说明：调用各个初始化函数，主循环中实现从PC机获取数据，发给MCP2515_1, 
*   再从MCP2515_2获取数据，发给PC机，PC机传给MCU的命令以'#'结束，不计入
*   命令长度，也不在CAN总线上传输。
*/

void main()
{
    unsigned char CmdLen;
	Uart_Init(); //初始化完串口，就可以和PC机通信
	EXTI1_init();
	MCP2515_Init(1);
	MCP2515_Init(2);
	while(1)
	{
	    UART_Send_Promt(msg1);//提示用户输入命令，命令以'#'结束
		CmdLen = UART_Recv_CMD(PC_Cmd);//从UART缓冲区机获取命令
		
		/*没从缓冲区读到命令,向PC机报告错误*/
	   	if(CmdLen<1)
		{
		    UART_Send_Data(err3,sizeof(err3));	
			continue;
		}
		//UART_Send_Data(PC_CMD,sizeof(PC_CMD));
		
		if(MCP2515_Sender(PC_Cmd,1)<1)//向MCP2515_1写数据
		{
		     UART_Send_Data(err1,sizeof(err1));	//向PC机报告错误
			 continue;
		}
		/*等待接收到CAN总线的数据才退出*/
		while(1)
		{
		   if(CAN_Flag)
		   {
			    CAN_Flag = 0;
				if(MCP2515_Recver(CAN_Cmd,2)<1)//读取MCP2515_2的数据
				{
				    UART_Send_Data(err2,sizeof(err2));//读取失败，发送错误回PC
					break;
				}
				else
				{
				     UART_Send_Data(CAN_Cmd,sizeof(CAN_Cmd));//把从CAN总线读取的命令送回PC
					 break;
				}

		   }
		}
		
	}
}

/*MCP2515_2发出接受中断*/
void EXIT1_Inetrrupt()	interrupt 2
{
	  CAN_Flag = 1;
}






