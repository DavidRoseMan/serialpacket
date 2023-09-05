#ifndef _serialpacket_h
#define _serialpacket_h

typedef xdata struct SerialPacket_send_struct
{
    volatile unsigned char xdata *Data;
    volatile unsigned char sizeofData;
    volatile unsigned char addr;
    volatile unsigned char data_tail_bak;
} SerialPacket_send_type;

typedef xdata struct SerialPacket_receive_struct
{
    volatile unsigned char xdata *Data_receive;
    volatile unsigned char xdata *Data_get;
    volatile unsigned char sizeofData_get;
    volatile unsigned char data_head_bak[3];
    volatile unsigned char data_head_0;
    volatile unsigned char data_head_1;
    volatile unsigned char addr;
    volatile unsigned char pointer;
} SerialPacket_receive_type;

#define receive_finish 1
#define receive_notfinish 0

#define set_success 1
#define set_false 0

void init_serialpacket_send(SerialPacket_send_type *package_send, unsigned char number_sizeofData, unsigned char addr);
void init_serialpacket_receive(SerialPacket_receive_type *package_receive, unsigned char number_sizeofData, unsigned char addr);
void free_serialpacket_send(SerialPacket_send_type *package_send);
void free_serialpacket_receive(SerialPacket_receive_type *package_receive);
unsigned char set_serialpacket_send(SerialPacket_send_type *package_send, unsigned char subscript, unsigned char Data) reentrant;
void send_serialpacket(Uart_type *uart_use, SerialPacket_send_type *package_send) reentrant;
unsigned char receive_serialpacket(SerialPacket_receive_type *package_receive, unsigned char buffer_hardware) reentrant;
void get_serialpacket(SerialPacket_receive_type *package_receive) reentrant;
unsigned char readfrom_serialpacket(SerialPacket_receive_type *package_receive, unsigned char subscript) reentrant;

#endif
