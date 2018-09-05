#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "udp.h"
#include "com.h"
#include "tcpclient.h"
#include "tcpserver.h"
#include "tcpudpcombase.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_OpenpushButton_clicked();                   //打开按钮
    void on_ClosepushButton_clicked();                  //关闭按钮
    void on_PeizhipushButton_clicked();                 //配置按钮
    void on_DisplaypushButton_clicked(bool checked);    //停止显示
    void on_ClearpushButton_clicked();
    void on_SendpushButton_clicked();
    void on_AutoSendcheckBox_toggled(bool checked);     //自动发送槽函数
    void SlectAutoSend();
    void HandleText();                                  //限制输入，16进制
    void HandleStyle();                                 //创建类型
    void on_HexSendcheckBox_toggled(bool checked);      //16进制发送
    void on_HexDisplaycheckBox_toggled(bool checked);   //16进制显示

private:
    Ui::MainWindow *ui;
    QTimer* _time;
    int style;
    int laststyle;
    UDP* P_udp;
    COM* P_com;
    TcpClient* P_tcpclient;
    TCPServer* p_tcpserver;
    TcpUdpComBase* p_base;

    void OpenCreateUdp();                               //创建UDP
    void OpenCreateCOM();                               //创建串口
    void OpenCreateTcpClient();                         //创建TCP客户端
    void OpenCreateTcpServer();                         //创建TCP服务器
    void WorkCreate();                                  //创建
    void DeleteCreation(int);
    void WorkDeleteCreation();

    bool HexDisplayFlag;
    bool HexSendFlag;
    bool DisplayFlag;

};

#endif // MAINWINDOW_H
