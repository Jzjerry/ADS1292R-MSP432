import array
import threading
import numpy as np
import time
import pyqtgraph as pg
import socket
from PyQt5.QtWidgets import QApplication, QWidget, QPushButton, QVBoxLayout, QLabel 
from PyQt5.QtCore import Qt

i = 0
bpm = 0
temp_d = 0
def tcpLink(sock, addr):
    print('Accept new connection from %s:%s...' % addr)
    sock.send('Welcome!'.encode())  # 当有新的连接到来时，向其回复Welcome!
    while True:
        data_b = sock.recv(100)        # 单次最多接收 1K 
        time.sleep(0.08)
        if data_b == 'exit':
            break
        # raw_data1 = int.from_bytes(data_b[4:6],byteorder='little')
        # raw_data1 = int.from_bytes(data_b[7:9],byteorder='little')
        
        # raw_data1 = raw_data1&0xff
        # raw_data1 = raw_data1/100.0
        global i
        global bpm
        global temp_d
        for j in range(0,len(data_b[0:63])):
            raw_data1 = int.from_bytes(data_b[j:j+1],byteorder='little')
            raw_data1 = raw_data1&0xff
            if i < historyLength:
                data[i] = raw_data1
                i = i+1
            else:
                data[:-1] = data[1:]
                data[i-1] = raw_data1
        # print(data_b)
        # bpm = int.from_bytes(data[64:65],byteorder='little')
        print(len(data_b))
        bpm = int.from_bytes(data_b[64:65],byteorder='little')
        last_temp = temp_d
        temp_d = int.from_bytes(data_b[65:67],byteorder='little')
        temp_d = temp_d&0xffff
        temp_d = temp_d/100.0
        if temp_d > 50 :
            temp_d = last_temp
        # print(temp_d)
        # print(data_b[65:67])
    sock.close()
    print('Connection from %s:%s closed.' % addr)

def plotData():
    global temp_d
    global bpm
    curve.setData(data)
    curvezoom.setData(data)
    text2.setNum(bpm)
    text4.setNum(temp_d)


if __name__ == "__main__":
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    # 监听端口:
    host = socket.gethostname() # 获取本地主机名
    s.bind(("192.168.137.1", 9999))  # 这里的IP地址不是任意的，是电脑热点的IP地址
    s.listen(5)
    print('Waiting for connection...')



    pg.setConfigOption('background', 'w')

    app = pg.mkQApp()  # 建立app
    win = pg.GraphicsWindow()  # 建立窗口
    win.setWindowTitle(u'心电曲线服务器端')
    win.resize(800, 500)  # 小窗口大小


    data = array.array('i')  # 可动态改变数组的大小,double型数组
    historyLength = 4096  # 横坐标长度
    a = 0
    data=np.zeros(historyLength).__array__('d')#把数组长度定下来
    p = win.addPlot(pen=(255,0,0),row=0,col=0,)  # 把图p加入到窗口中
    p.showGrid(x=True, y=True)  # 把X和Y的表格打开
    p.setRange(xRange=[0, historyLength], yRange=[0, 256], padding=0)
    p.setLabel(axis='left', text='y / Data')  # 靠左
    p.setLabel(axis='bottom', text='x / Samples')
    p.setTitle('ECG')  # 表格的名字
    curve = p.plot(pen=(128,0,0))  # 绘制一个图形
    curve.setData(data)
    zoom = pg.LinearRegionItem([512,1024])
    zoom.setZValue(-10)
    p.addItem(zoom)


    l1 = win.addLayout(row=1,col=0)
    pzoom = l1.addPlot(title='Zooming')
    pzoom.setRange(xRange=[512,1024],yRange=[0,256])
    curvezoom = pzoom.plot(pen=(255,0,0))
    curvezoom.setData(data)

    # text = l1.addViewBox(row=0,col=1)
    proxy1 = pg.QtGui.QGraphicsProxyWidget()
    proxy2 = pg.QtGui.QGraphicsProxyWidget()
    proxy3 = pg.QtGui.QGraphicsProxyWidget()
    proxy4 = pg.QtGui.QGraphicsProxyWidget()

    text1 = QLabel('BPM:')
    text2 = QLabel('0000000')
    text3 = QLabel('Tmp:')
    text4 = QLabel('0000000')
    
    text1.setStyleSheet("QLabel{font-size:24px;font-family: \"Microsoft YaHei\";}")
    text2.setStyleSheet("QLabel{font-size:24px;font-family: \"Microsoft YaHei\";}")
    text3.setStyleSheet("QLabel{font-size:24px;font-family: \"Microsoft YaHei\";}")
    text4.setStyleSheet("QLabel{font-size:24px;font-family: \"Microsoft YaHei\";}")
    
    proxy1.setWidget(text1)
    l1.addItem(proxy1,row=0,col=1)
    
    proxy2.setWidget(text2)
    l1.addItem(proxy2,row=0,col=2)

    proxy3.setWidget(text3)
    l1.addItem(proxy3,row=1,col=1)
    
    proxy4.setWidget(text4)
    l1.addItem(proxy4,row=1,col=2)
    
    # box = l1.addViewBox()
    def updatePlot():
        pzoom.setXRange(*zoom.getRegion(), padding=0)
    def updateRegion():
        zoom.setRegion(pzoom.getViewBox().viewRange()[0])

    zoom.sigRegionChanged.connect(updatePlot)
    pzoom.sigXRangeChanged.connect(updateRegion)



    while True:
        # 接受一个新连接:
        sock, addr = s.accept()
        # 创建新线程来处理TCP连接:
        t = threading.Thread(target=tcpLink, args=(sock, addr))
        t.start()
        timer = pg.QtCore.QTimer()
        timer.timeout.connect(plotData)  # 定时刷新数据显示
        timer.start(50)  # 多少ms调用一次
        app.exec_()
        break