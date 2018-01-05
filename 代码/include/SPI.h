#ifndef _SPI_H
#define _SPI_H

#ifdef _SPI_C
extern void SPI_Send_Byte(unsigned char Data,unsigned char chipnum);
extern unsigned char SPI_Recv_Byte(unsigned char chipnum);
#endif

#endif