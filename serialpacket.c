#include "transmit.h"
#include "serialpacket.h"
#include "variable.h"
#include <STDLIB.H>

void init_serialpacket_send(SerialPacket_send_type *package_send, unsigned char number_sizeofData, unsigned char addr)
{
    package_send->Data = (unsigned char xdata *)malloc((size_t)number_sizeofData);
    package_send->sizeofData = number_sizeofData;
    package_send->data_tail_bak = 0;
    package_send->addr = addr;
}

void init_serialpacket_receive(SerialPacket_receive_type *package_receive, unsigned char number_sizeofData, unsigned char addr)
{
    unsigned char xdata i;

    package_receive->Data_get = (unsigned char xdata *)malloc((size_t)number_sizeofData);
    package_receive->Data_receive = (unsigned char xdata *)malloc(2 * (size_t)number_sizeofData);
    package_receive->sizeofData_get = number_sizeofData;
    for (i = 0; i < 3; i++)
        package_receive->data_head_bak[i] = 0;
    package_receive->data_head_0 = 0;
    package_receive->data_head_1 = 0;
    package_receive->pointer = 0xff;
    package_receive->addr = addr;
}

void free_serialpacket_send(SerialPacket_send_type *package_send)
{
    free((void _MALLOC_MEM_ *)package_send->Data);
    package_send->Data = NULL;
}

void free_serialpacket_receive(SerialPacket_receive_type *package_receive)
{
    free((void _MALLOC_MEM_ *)package_receive->Data_get);
    free((void _MALLOC_MEM_ *)package_receive->Data_receive);
    package_receive->Data_get = NULL;
    package_receive->Data_receive = NULL;
}

unsigned char set_serialpacket_send(SerialPacket_send_type *package_send, unsigned char subscript, unsigned char Data) reentrant
{
    if (subscript < package_send->sizeofData)
    {
        package_send->Data[subscript] = Data;
        return set_success;
    }
    return set_false;
}

void send_serialpacket(Uart_type *uart_use, SerialPacket_send_type *package_send) reentrant
{
    unsigned char i;
    unsigned char head = 0;

    do
    {
        head = (unsigned char)rand();
    } while (head == package_send->data_tail_bak);
    sendbyte_UART(uart_use, head);
    sendbyte_UART(uart_use, head + 2);
    sendbyte_UART(uart_use, head + 4);
    sendbyte_UART(uart_use, package_send->addr);
    sendbyte_UART(uart_use, ~package_send->addr);
    for (i = 0; i < package_send->sizeofData; i++)
    {
        sendbyte_UART(uart_use, package_send->Data[i]);
        sendbyte_UART(uart_use, ~package_send->Data[i]);
        if (i == package_send->sizeofData - 1)
            package_send->data_tail_bak = ~package_send->Data[i];
    }
}

unsigned char receive_serialpacket(SerialPacket_receive_type *package_receive, unsigned char buffer_hardware) reentrant
{
    package_receive->data_head_bak[0] = package_receive->data_head_bak[1];
    package_receive->data_head_bak[1] = package_receive->data_head_bak[2];
    package_receive->data_head_bak[2] = buffer_hardware;

    if (package_receive->pointer == 1)
    {
        package_receive->data_head_0 = buffer_hardware;
    }
    else if (package_receive->pointer == 2)
    {
        package_receive->data_head_1 = buffer_hardware;
        if (package_receive->data_head_0 != package_receive->addr ||       \ 
            package_receive->data_head_0 != ~package_receive->data_head_1)
        {
            package_receive->pointer = 0xff;
        }
    }
    if (package_receive->pointer >= 3 && package_receive->pointer <= (package_receive->sizeofData_get * 2 + 2))
    {
        package_receive->Data_receive[package_receive->pointer - 3] = buffer_hardware;
        if (package_receive->pointer % 2 == 0)
        {
            if (package_receive->Data_receive[package_receive->pointer - 3] !=
                ~package_receive->Data_receive[package_receive->pointer - 4])
                package_receive->pointer = 0xff;
        }
    }
    if (package_receive->pointer == (package_receive->sizeofData_get * 2 + 2))
    {
        package_receive->pointer = 0xff;
        return receive_finish;
    }
    //--------------------------------------------------------------------------------------------------------------------
    if (package_receive->data_head_bak[0] + 2 == package_receive->data_head_bak[1] &&
        package_receive->data_head_bak[1] + 2 == package_receive->data_head_bak[2])
    {
        package_receive->pointer = 0;
    }
    if (package_receive->pointer != 0xff)
        package_receive->pointer++;
    return receive_notfinish;
}

void get_serialpacket(SerialPacket_receive_type *package_receive) reentrant
{
    unsigned char i = 0;

    for (i = 0; i < package_receive->sizeofData_get; i++)
    {
        package_receive->Data_get[i] = package_receive->Data_receive[2 * i];
    }
}

unsigned char readfrom_serialpacket(SerialPacket_receive_type *package_receive, unsigned char subscript) reentrant
{
    return package_receive->Data_get[subscript];
}