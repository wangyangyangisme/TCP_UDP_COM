#include <QMessageBox>
#include "tcpserver.h"
#include "ui_tcpserver.h"

TCPServer::TCPServer(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TCPServer)
{
    ui->setupUi(this);
    tcpserver = new QTcpServer(this);
    connect (tcpserver, SIGNAL(newConnection()), this, SLOT(updateserver()));

    linkList.clear();
    showbox = true;
    initListen();
}

TCPServer::~TCPServer()
{
    delete ui;
}

void TCPServer::on_OKpushButton_clicked()
{
    WorkClose();
    WorkOpen();
    this->close();
}

void TCPServer::updateserver()
{
    main_ui->Statelabel->setText(tr("已连接"));
    //ui->serverSendpushButton->setEnabled(true);
    QTcpSocket *tcpclient = tcpserver->nextPendingConnection ();
    if(!tcpclient) return;

    QString clientip = tcpclient->peerAddress().toString();
    // 不可接受的Ip，断开其连接
    if(clientip.isNull() || clientip.isEmpty())
    {
        tcpclient->disconnectFromHost();
        return;
    }

    linkList.append(tcpclient);

    connect (tcpclient, SIGNAL(disconnected()), this, SLOT(updateSendStatus()));
    connect (tcpclient, SIGNAL(readyRead()), this, SLOT(serverReadMessage()));

    updataWidgetList();
}

void TCPServer::updataWidgetList()
{
    foreach(QTcpSocket* st,linkList)
    {
        if(!st) continue;

        QString name = st->peerName();
        QString ip = st->peerAddress().toString();



    }
}

//无连接
void TCPServer::updateSendStatus()
{
    main_ui->Statelabel->setText(tr("无连接"));
    //ui->serverSendpushButton->setEnabled(false);

    QTcpSocket *client = qobject_cast<QTcpSocket *>(sender());
    linkList.removeOne(client);
    client->disconnect();
    client->close();
    client->deleteLater();

    updataWidgetList();
}

void TCPServer::serverReadMessage()
{
    QTcpSocket *client = qobject_cast<QTcpSocket *>(sender());
    if(!client) return;
    QByteArray byte_data=client->readAll();

    QString strDisplay;
    if(m_isHexDisplay)
    {
        QString str = byte_data.toHex().data();
        str = str.toUpper ();
        for(int i = 0;i<str.length ();i+=2)
        {
            QString st = str.mid (i,2);
            strDisplay += st;
            strDisplay += " ";
        }
    }
    else
    {
        strDisplay = QString(byte_data);
    }
//    QString str = byte_data.toHex().data();
//    str = str.toUpper ();
//    QString stt;
//    for(int i = 0;i<str.length ();i+=2)
//    {
//        QString st = str.mid (i,2);
//        stt += st;
//        stt += " ";
//    }

    if(showbox == true)
    {
        QString stIP = client->peerAddress().toString();
        quint16 intport = client->peerPort();
        QString stPort = QString::number(intport);

        main_ui->ViewtextEdit->append(tr("自【%1:%2】:%3").arg(stIP).arg(stPort).arg(strDisplay));
    }
}

void TCPServer::initListen()
{
    serverPort = ui->serverPortlineEdit->text();

    if(serverPort.isEmpty ())
    {
        QMessageBox::warning (this, tr("警告："), tr("端口为空！"));
        return;
    }
    if(!tcpserver) return;
    tcpserver->listen (QHostAddress::Any, serverPort.toInt());
}


bool TCPServer::WorkOpen()
{
    serverPort = ui->serverPortlineEdit->text();

    if(serverPort.isEmpty ())
    {
        QMessageBox::warning (this, tr("警告："), tr("端口为空！"));
        return false;
    }
    if(!tcpserver) return false;
    tcpserver->listen (QHostAddress::Any, serverPort.toInt());

    main_ui->Statelabel->setText(tr("开始监听"));
    return true;
}

void TCPServer::WorkClose()
{
    foreach(QTcpSocket* st ,linkList)
    {
        if(!st) continue;

        st->disconnect();
        st->close();
        st->deleteLater();
    }

    tcpserver->close();
    main_ui->Statelabel->setText(tr("停止监听"));

    linkList.clear();

    updataWidgetList();
}

void TCPServer::WorkSendMessage()
{
    if(linkList.count() == 0)
        return;
    QTcpSocket* client = 0;
    foreach(QTcpSocket* s,linkList)
    {
        if(!s) continue;
        client = s;
        if(!client) return;

        QString sdata= main_ui->SendtextEdit->toPlainText();
        if(sdata.isEmpty())return;
        //datagram = sdata.toAscii().data();
        QByteArray sendBuf = "";
        if(m_isHexSend)
        {
            sendBuf = QByteArray::fromHex (sdata.toLatin1().data());
        }
        else
        {
            sendBuf = sdata.toAscii();
        }
        client->write(sendBuf);
    }
}
