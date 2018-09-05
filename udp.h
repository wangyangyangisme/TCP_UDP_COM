#ifndef UDP_H
#define UDP_H

#include <QDialog>
#include <QtNetwork>
#include "ui_mainwindow.h"
#include "tcpudpcombase.h"

namespace Ui {
    class UDP;
}

class UDP : public QDialog,public TcpUdpComBase
{
    Q_OBJECT

public:
    explicit UDP(QWidget *parent = 0);
    ~UDP();
    void setMainUiPoint(Ui::MainWindow *ui){main_ui=ui;}
    bool WorkOpen();                        //绑定连接
    void WorkClose();                       //断开连接
    void WorkSendMessage();                 //发送数据
    void WorkSetDisplayFlag(bool flag){DisplayFlag = flag;}

private:
    Ui::UDP *ui;
    Ui::MainWindow *main_ui;
    QUdpSocket *sender;
    QUdpSocket *receiver;
    QList<QHostAddress> IPlist;
    void getLocalIP();

    bool HexDisplayFlag;
    bool HexSendFlag;
    bool DisplayFlag;

private slots:
    void sendmessageslot();
    void ReadAndDisplayMessage();//数据接收及显示槽函数
    void on_OKpushButton_clicked();
};

#endif // UDP_H
