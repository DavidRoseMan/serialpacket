# 通过串口发送或接收一个或多个可寻址的数据包

## 先决条件

```c
init_heap();//初始化堆空间。
```

## 发送数据包

```c
SerialPacket_send_type packet_send;//创建一个发送端的数据包对象。
init_serialpacket_send(&packet_send, 10, 1);//初始化发送端的数据包对象。此数据包发送的内容大小为10个unsigned char，此数据包的地址为1。
free_serialpacket_send(&packet_send);//释放发送端的数据包对象。(不建议启用)
set_serialpacket_send(&packet_send, 5, 1);//设置发送端的数据包对象。设置数组下标为5的位置的值为1。
send_serialpacket(&uart_use, &packet_send);//将此数据包的内容通过串口uart_use发送出去。
```

## 接收数据包

```c
SerialPacket_receive_type packet_receive;//创建一个接收端的数据包对象。
init_serialpacket_receive(&packet_receive, 10, 1);//初始化接收端的数据包对象。此数据包接收的内容大小为10个unsigned char，此数据包的地址为1。
free_serialpacket_receive(&packet_receive);//释放接收端的数据包对象。(不建议启用)
receive_serialpacket(&packet_receive,SBUF);//在串口中断程序中调用，用于发送端数据包的接收。数据包校验完成时，此函数返回1。
get_serialpacket(&packet_receive);//当数据包校验完成时，调用此函数对数据进行保存。
readfrom_serialpacket(&packet_receive,5);//此函数用于读取保存区的数据，5为数组下标。返回值即是下标为5的位置的值。
```
