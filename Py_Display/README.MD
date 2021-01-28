# Python上位机显示 —— Upper Display
在运行这个Python程序之前，需要保证完成以下几个步骤：
+ 确认安装了本程序所需的库(**pyqtgraph,numpy,pyQt...etc**)
+ 开启笔记本电脑(Windows 10)的移动热点，确认移动热点主机ip地址与程序中的相同
+ 参照**ESP8266**的串口AT命令手册，提前配置好**ESP8266**模块的连接模式(TCP+透明传输)，确认在开启热点时可以看到设备接入
+ 运行程序后，可以看到端口监听信息与接收数据包长度(64 Bytes(ECG)+1 Byte(Bpm)+2 Bytes(Temperature)+1 Byte(Step))

**English Version**

To display ECG and other data received from the board via TCP connection, you should follow the step below before you :
+ install required Python packages(**pyqtgraph,numpy,pyQt...etc**)(requirement.txt comes soon)
+ Turn on the Hotspot of your laptop or other devices(tested in laptop with Windows 10),change the ip in code to your Hotspot's host ip.
+ Configure your **ESP8266** via serial port commands in advance(**Station mode, Single TCP Connection, CIPMODE=1**)(check ESP8266 AT instruction set for details)
+ Run the code, you should see that the connection is established and the length of data is correct(64 Bytes(ECG)+1 Byte(Bpm)+2 Bytes(Temperature)+1 Byte(Step))
