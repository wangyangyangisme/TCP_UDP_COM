#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QDialog>
#include <QNetworkInterface>
#include <QtNetwork>
#include <QTimer>
#include <QList>
#include "ui_mainwindow.h"
#include "tcpudpcombase.h"

namespace Ui {
    class TCPServer;
}

class TCPServer : public QDialog,public TcpUdpComBase
{
    Q_OBJECT

public:
    explicit TCPServer(QWidget *parent = 0);
    ~TCPServer();

    void setDisplayFlag(bool flag){showbox = flag;}
    void setMainUiPoint(Ui::MainWindow *ui){main_ui=ui;}
    bool WorkOpen();                                        //侦听
    void WorkClose();                                       //停止侦听
    void WorkSendMessage();                                 //发送数据
    void WorkSetDisplayFlag(bool flag){showbox = flag;}

signals:
    void formSmit();

private slots:
    void updateserver();
    void updateSendStatus();//无连接
    void serverReadMessage();
    void on_OKpushButton_clicked();

private:
    Ui::TCPServer *ui;
    Ui::MainWindow *main_ui;
    void updataWidgetList();
    void cleaerWidgetList();
    void initListen();


    QTcpServer *tcpserver;
    QString serverPort;
    QTimer* _time;
    QList<QTcpSocket* > linkList;
    bool showbox;

};

#endif // TCPSERVER_H
