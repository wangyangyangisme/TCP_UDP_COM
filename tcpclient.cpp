#include <QMessageBox>
#include "tcpclient.h"
#include "ui_tcpclient.h"

TcpClient::TcpClient(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TcpClient)
{
    ui->setupUi(this);
    tcplink = NULL;
    showbox = true;
    m_isConnect = false;
}

TcpClient::~TcpClient()
{
    delete ui;
}

void TcpClient::on_OKpushButton_clicked()
{
    WorkOpen();
    this->close();
}

void TcpClient::sendmessage()
{
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
    tcplink->write(sendBuf);
}

//客户端无连接
void TcpClient::displayError(QAbstractSocket::SocketError)
{
    tcplink->close();
    main_ui->OpenpushButton->setEnabled(true);
    main_ui->ClosepushButton->setEnabled(false);
    main_ui->SendpushButton->setEnabled(false);
    main_ui->Statelabel->setText(tr("未发现连接！"));
}

//客户端读取消息
void TcpClient::clientReadMessage()
{
    QByteArray byte_data=tcplink->readAll();
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

    if(showbox == true)
    {
        quint16 intPort = tcplink->peerPort();
        QString stPort = QString::number(intPort);

        QString stIP = tcplink->peerAddress().toString();

        main_ui->ViewtextEdit->append(tr("自【%1:%2】:%3").arg(stIP).arg(stPort).arg(strDisplay));
    }
}

//已连接上服务器
void TcpClient::slotConnect()
{
    m_isConnect = true;
    main_ui->Statelabel->setText(tr("已连接"));
    main_ui->OpenpushButton->setEnabled(false);
    main_ui->ClosepushButton->setEnabled(true);
    main_ui->SendpushButton->setEnabled(true);
}

void TcpClient::slotDisconnect()
{
    m_isConnect = false;
    main_ui->Statelabel->setText(tr("连接已断开"));
    main_ui->OpenpushButton->setEnabled(true);
    main_ui->ClosepushButton->setEnabled(false);
    main_ui->SendpushButton->setEnabled(false);
}

bool TcpClient::WorkOpen()
{
    serverIP = ui->clientIPlineEdit->text();
    serverPort = ui->clientPortlineEdit->text();
    if(serverIP.isEmpty()||serverPort.isEmpty())
    {
        QMessageBox::warning (this,tr("警告："),tr("IP或端口为空！"));
        return false;
    }

    tcplink = new QTcpSocket(this);
    connect(tcplink, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(displayError(QAbstractSocket::SocketError)));
    connect(tcplink, SIGNAL(connected()), this, SLOT(slotConnect()));
    connect(tcplink, SIGNAL(disconnected()), this, SLOT(slotDisconnect()));
    connect(tcplink, SIGNAL(readyRead()), this, SLOT(clientReadMessage()));

    tcplink->connectToHost(serverIP,serverPort.toInt());

    return true;
}

void TcpClient::WorkClose()
{
    if(!m_isConnect)    return;

    tcplink->disconnectFromHost();
    main_ui->Statelabel->setText(tr("断开连接！"));
}

void TcpClient::WorkSendMessage()
{
    if(!m_isConnect)    return;

    sendmessage();
    return;
}
