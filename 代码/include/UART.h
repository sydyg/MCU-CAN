#ifndef _UART_H
#define _UART_H
#include<intrins.h>
/*导出外部接口*/
#ifdef _UART_C
extern void Uart_Init(void);
extern unsigned char UART_Send_Data(unsigned char Data[],unsigned char len);
extern void UART_Send_Promt(unsigned char* prompt);
extern unsigned char UART_Recv_CMD(unsigned char Data[]);
extern void delaynms(unsigned int i);
#endif

#endif