#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QDialog>
#include <QNetworkInterface>
#include <QtNetwork>
#include "ui_mainwindow.h"
#include "tcpudpcombase.h"

namespace Ui {
    class TcpClient;
}

class TcpClient : public QDialog,public TcpUdpComBase
{
    Q_OBJECT

public:
    explicit TcpClient(QWidget *parent = 0);
    ~TcpClient();
    void sendmessage();                                     //发送数据
    void setMainUiPoint(Ui::MainWindow *ui){main_ui=ui;}
    void setDisplayFlag(bool flag){showbox = flag;}
    bool WorkOpen();                                        //连接
    void WorkClose();                                       //断开连接
    void WorkSendMessage();                                 //发送数据
    void WorkSetDisplayFlag(bool flag){showbox = flag;}

private slots:
    void displayError(QAbstractSocket::SocketError);
    void slotConnect();
    void slotDisconnect();
    void clientReadMessage();                               //读取信息

    void on_OKpushButton_clicked();

private:
    Ui::TcpClient *ui;
    Ui::MainWindow *main_ui;
    QTcpSocket *tcplink;
    QString serverIP;
    QString serverPort;
    QTimer* _time;
    bool showbox;
    bool m_isConnect;

};

#endif // TCPCLIENT_H
