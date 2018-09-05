#include "udp.h"
#include "ui_udp.h"

UDP::UDP(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UDP)
{
    ui->setupUi(this);
    sender = new QUdpSocket(this);

    getLocalIP();
    HexDisplayFlag = true;
    HexSendFlag = true;
    DisplayFlag = true;

    receiver = NULL;
    receiver = new QUdpSocket(this);
    int port = ui->UDPSenderPortlineEdit->text().toInt();

    if(receiver->bind(QHostAddress::Any,port))//监听任何IP地址的连接
    {
        qDebug()<<"Udp client bind localhost ok!";
    }
    else
    {
        qDebug()<<"Udp client bind localhost failed!";
    }
    connect(receiver,SIGNAL(readyRead()),this,SLOT(ReadAndDisplayMessage()));
}

UDP::~UDP()
{
    delete ui;
}

void UDP::getLocalIP()
{
    IPlist = QNetworkInterface::allAddresses();

    foreach(QHostAddress IP, IPlist)
    {
        ui->UDPServerIPcomboBox->addItem(IP.toString());
    }
}

void UDP::ReadAndDisplayMessage()
{
    if(receiver == NULL) return;

    while(receiver->hasPendingDatagrams())//拥有等待的数据报
    {
        QByteArray datagram;//用于存放接收的数据报
        datagram.resize(receiver->pendingDatagramSize());
         //让datagram的大小为等待处理的数据报的大小，这样才能接收到完整的数据

        QHostAddress senderAddress;
        quint16 senderPort;
        receiver->readDatagram(datagram.data(), datagram.size(),
                                &senderAddress, &senderPort);
        //接收数据报，将其存放到datagram中,senderAddress和senderPort分别存放发送者的IP地址和端口号
        QString sPort = QString::number(senderPort);
        QString sAddress = senderAddress.toString();
        main_ui->ViewtextEdit->moveCursor(QTextCursor::End,QTextCursor::MoveAnchor);
        //光标移动到末尾
        if(false==DisplayFlag) return;
        if(m_isHexDisplay)//16进制显示
        {
            //QByteArray to qstring start
            QString str = datagram.toHex ().data ();
            str = str.toUpper ();
            QString stt;
            for(int i = 0;i<str.length ();i+=2)
            {
                QString st = str.mid (i,2);
                stt += st;
                stt += " ";
            }//QByteArray to qstring end             // str == "3f"
            main_ui->ViewtextEdit->insertPlainText(tr("From:%1:%2|%3").arg(sAddress).arg(sPort).arg(stt)+"\n");
            //qDebug()<<str;
        }
        else
        {
            main_ui->ViewtextEdit->insertPlainText(tr("From:%1:%2|").arg(sAddress).arg(sPort)+datagram+"\n");
        }
    }
}

void UDP::sendmessageslot()
{
    WorkSendMessage();
}

void UDP::on_OKpushButton_clicked()
{
    WorkOpen();
    this->close();
}

bool UDP::WorkOpen()
{
    if(receiver)
    {
        receiver->close();
        receiver = NULL;
    }
    receiver = new QUdpSocket(this);

    int port = ui->UDPSenderPortlineEdit->text().toInt();
    QHostAddress address;
    address = ui->UDPServerIPcomboBox->currentText();

    if(receiver->bind(QHostAddress::Any,port))//监听任何IP地址的连接
    {
        qDebug()<<"Udp client bind localhost ok!";
        ui->UDPServerStatelabel->setText(tr("Udp client bind localhost ok!"));
    }
    else
    {
        qDebug()<<"Udp client bind localhost failed!";
        ui->UDPServerStatelabel->setText(tr("Udp client bind localhost failed!"));
    }
    connect(receiver,SIGNAL(readyRead()),this,SLOT(ReadAndDisplayMessage()));
    main_ui->SendpushButton->setEnabled(true);
    return true;
}

void UDP::WorkClose()
{
    if(receiver == NULL)return;
    disconnect(receiver,SIGNAL(readyRead()),this,SLOT(ReadAndDisplayMessage()));
    receiver->disconnectFromHost();
    receiver->close();
    receiver = NULL;
    main_ui->SendpushButton->setEnabled(false);
    main_ui->Statelabel->setText(tr("UDP停止监听!"));
    ui->UDPServerStatelabel->setText(tr("UDP停止监听!"));
}

void UDP::WorkSendMessage()
{
    if(receiver == NULL) return;
    QByteArray datagram;
    QString sdata= main_ui->SendtextEdit->toPlainText();
    if(sdata.isEmpty())return;
    datagram = sdata.toAscii().data();
    //datagram = QByteArray::fromHex (sdata.toLatin1 ().data ());UDPFarIP

    int port = ui->UDPFarPortlineEdit->text().toInt();
    QString add = ui->UDPFarIPlineEdit->text();
    QHostAddress IP;
    IP.setAddress(add);
    if(m_isHexSend)
    {
        QByteArray aa = QByteArray::fromHex (sdata.toLatin1().data ());
        sender->writeDatagram(aa.data(),aa.size(),IP,port);
    }
    else
        sender->writeDatagram(datagram.data(),datagram.size(),IP,port);
}
