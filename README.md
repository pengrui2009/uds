# uds
uds protocol will stepply satisfied iso14229-1-2-3 and iso15765-2

## support can device

virtual can

## enable virtual can

```
# 设置虚拟socketCAN接口
# setup a virtual socketCAN interface
sudo ip link add name vcan0 type vcan
sudo ip link set vcan0 up
```
virtual_socketcan.c 是基于vcan进行本地模拟测试

# 用candump看看输出
```sh
> candump -tz vcan0
 (000.000000)  vcan0  111   [4]  01 02 03 04
 (000.000028)  vcan0  111   [4]  01 02 03 04
 (000.000090)  vcan0  701   [8]  02 11 01 00 00 00 00 00  # 0x11 ECU复位请求
 (000.010199)  vcan0  700   [8]  02 51 01 00 00 00 00 00  # 0x11 ECU复位肯定响应
 (000.010213)  vcan0  701   [8]  05 22 00 01 00 08 00 00  # 0x22 RDBI请求
 (000.020318)  vcan0  700   [8]  10 1B 62 00 01 00 00 08  # ISO-TP流控框
 (000.020326)  vcan0  701   [8]  30 08 00 00 00 00 00 00  # 0x22 RDBI请求
 (000.030416)  vcan0  700   [8]  21 49 27 6D 20 61 20 55  # 0x22 RDBI响应 (1)
 (000.040674)  vcan0  700   [8]  22 44 53 20 73 65 72 76  # 0x22 RDBI响应 (2)
 (000.050829)  vcan0  700   [8]  23 65 72 20 20 20 20 00  # 0x22 RDBI响应 (3)
 (000.051509)  vcan0  701   [8]  02 10 03 00 00 00 00 00  # 0x10 会话控制
 (000.072713)  vcan0  700   [8]  03 7F 10 33 00 00 00 00  # 0x10 会话控制否定响应
 (000.072979)  vcan0  701   [8]  02 11 04 00 00 00 00 00  # 0x11 ECU复位请求
 (000.124015)  vcan0  700   [8]  03 51 04 FF 00 00 00 00  # 0x11 ECU复位肯定响应

```

## the method of compile in Linux system.
mkdir build && cd build
cmake ..
make