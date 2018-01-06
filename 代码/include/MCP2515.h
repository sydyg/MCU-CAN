#ifndef _MCP2515_H
#define _MCP2515_H

/*MCP2515 SPI命令*/
#define CAN_RESET       0xC0
#define CAN_READ        0x03
#define CAN_RDRX_BUFF   0x90
#define CAN_WRITE       0x02
#define CAN_TRS_RB0     0x81
#define CAN_TRS_RB1     0x82
#define CAN_CAN_TRS_RB2 0x84
#define CAN_RD_STATE    0xA0
#define CAN_RD_RX_STATE	0xB0
#define CAN_BIT_MODIFY	0x05

/*MCP2515 报文发送缓冲区*/
/*TX buffer0*/
#define TXB0CTRL   0x30
#define TXB0SIDH   0x31
#define TXB0SIDL   0x32
#define TXB0EID8   0x33
#define TXB0EID0   0x34
#define TXB0DLC	   0x35
#define TXB0D0     0x36
#define TXB0D1     0x37
#define TXB0D2     0x38
#define TXB0D3     0x39
#define TXB0D4     0x3a
#define TXB0D5     0x3b
#define TXB0D6     0x3c
#define TXB0D7     0x3d
/*TX buffer1*/
#define TXB1CTRL   0x40
#define TXB1SIDH   0x41
#define TXB1SIDL   0x42
#define TXB1EID8   0x43
#define TXB1EID0   0x44
#define TXB1DLC	   0x45
#define TXB1D0     0x46
#define TXB1D1     0x47
#define TXB1D2     0x48
#define TXB1D3     0x49
#define TXB1D4     0x4a
#define TXB1D5     0x4b
#define TXB1D6     0x4c
#define TXB1D7     0x4d
/*TX buffer2*/
#define TXB2CTRL   0x50
#define TXB2SIDH   0x51
#define TXB2SIDL   0x52
#define TXB2EID8   0x53
#define TXB2EID0   0x54
#define TXB2DLC	   0x55
#define TXB2D0     0x56
#define TXB2D1     0x57
#define TXB2D2     0x58
#define TXB2D3     0x59
#define TXB2D4     0x5a
#define TXB2D5     0x5b
#define TXB2D6     0x5c
#define TXB2D7     0x5d

/*MCP2515 报文接收寄存器*/
/*RX buffer0*/
#define RXB0CTRL 0x60
#define RXB0SIDH 0x61
#define RXB0SIDL 0x62
#define RXB0EID8 0x63
#define RXB0EID0 0x64
#define RXB0DLC	 0x65
#define RXB0D0   0x66
#define RXB0D1   0x67
#define RXB0D2   0x68
#define RXB0D3   0x69
#define RXB0D4   0x6a
#define RXB0D5   0x6b
#define RXB0D6   0x6c
#define RXB0D7   0x6d
/*RX buffer1*/
#define RXB1CTRL 0x70
#define RXB1SIDH 0x71
#define RXB1SIDL 0x72
#define RXB1EID8 0x73
#define RXB1EID0 0x74
#define RXB1DLC	 0x75
#define RXB1D0   0x76
#define RXB1D1   0x77
#define RXB1D2   0x78
#define RXB1D3   0x79
#define RXB1D4   0x7a
#define RXB1D5   0x7b
#define RXB1D6   0x7c
#define RXB1D7   0x7d

/*接收缓冲器滤波寄存器*/
#define RXF0SIDH   0x00
#define RXF0SIDL   0x01
#define RXF0EID8   0x02
#define RXF0EID0   0x03
#define RXF1SIDH   0x04
#define RXF1SIDL   0x05
#define RXF1EID8   0x06
#define RXF1EID0   0x07
#define RXF2SIDH   0x08
#define RXF2SIDL   0x09
#define RXF2EID8   0x0a
#define RXF2EID0   0x0b
#define RXF3SIDH   0x10
#define RXF3SIDL   0x11
#define RXF3EID8   0x12
#define RXF3EID0   0x13
#define RXF4SIDH   0x14
#define RXF4SIDL   0x15
#define RXF4EID8   0x16
#define RXF4EID0   0x17
#define RXF5SIDH   0x18
#define RXF5SIDL   0x19
#define RXF5EID8   0x1a
#define RXF5EID0   0x1b

/*接收缓冲器屏蔽寄存器*/
#define RXM0SIDH   0x20
#define RXM0SIDL   0x21
#define RXM0EID8   0x22
#define RXM0EID0   0x23
#define RXM1SIDH   0x24
#define RXM1SIDL   0x25
#define RXM1EID8   0x26
#define RXM1EID0   0x27
/*位定时寄存器*/
#define CNF1	   0x2A
#define CNF2	   0x29
#define CNF3	   0x28
/*错误检测*/
#define TEC		   0x1C
#define REC		   0x1D
#define EFLG	   0x2D
/*中断寄存器*/
#define CANINTE	   0x2B
#define CANINTF	   0x2C
/*复位*/
#define CANCTRL	   0x0F
#define CANSTAT	   0x0E
/*杂项*/
#define TXRTSCTRL 0x0D //引脚控制
#define BFPCTRL	  0x0c //引脚控制

/*数据预定义*/
/*CNF1*/
#define BRP           0x04
#define SJW_2TQ       0x40
/*CNF2*/
#define BTLMODE		  0x80
#define SAM_1		  0x00
#define PHSEG1_8TQ	  (0x07<<3)
#define PRSEG_3TQ	  0x02
/*CNF3*/
#define SOF			  0x00
#define WAKFIL		  (0x01<<6)
#define PHSEG2_8TQ	  0x07
/*报文发送缓冲器*/
#define TXP			  0x11
#define TXREQ		  (0x01<<3)
#define SIDH		  0xFF
#define SIDL		  0x00

/*报文接收缓冲器*/
#define RXM	          (0x03<<5)
#define FILHIT		  0x00

/*中断*/
#define RX0IE        0x01
#define RX1IE        (0x01<<1)
/*CAN控制寄存器*/
#define REQOP_NORM    (0x00<<5)
#define REQOP_CONF    (0x4<<5)
#define REQOP_LOOP    (0x2<<5)
#define OSM			  (0x00<<3)
#define CLKEN_D		  (0x00<<2)

 
#ifdef 	_MCP2515_C
extern unsigned char MCP2515_Sender(unsigned char Data[],unsigned char chpnum);
extern unsigned char MCP2515_Recver(unsigned char Data[],unsigned char chpnum);
extern void MCP2515_Init(unsigned char chipnum);
#endif

#endif
