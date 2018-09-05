#include "udpserverform.h"
#include <QDebug>

UDPServerForm::UDPServerForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UDPServerForm)
{
    ui->setupUi(this);
    connect(ui->UDPSendertextEdit,SIGNAL(textChanged()),this,SLOT(HandleText()));
    sender = new QUdpSocket(this);

    getLocalIP();
    HexDisplayFlag = false;
    HexSendFlag = false;
    DisplayFlag = true;
    //ui->BroadcastpushButton->setEnabled(false);

    _time = new QTimer();
    _time->setInterval(2000);
    connect(_time,SIGNAL(timeout()),this,SLOT(sendmessageslot()));

    receiver = NULL;
    receiver = new QUdpSocket(this);
    int port = ui->UDPSenderPortlineEdit->text().toInt();
//    QHostInfo info=QHostInfo::fromName(QHostInfo::localHostName());
//    QHostAddress address=info.addresses().first();

    if(receiver->bind(QHostAddress::Any,port))//监听任何IP地址的连接

    {
        qDebug()<<"Udp client bind localhost ok!";
    }
    else
    {
        qDebug()<<"Udp client bind localhost failed!";
    }
    connect(receiver,SIGNAL(readyRead()),this,SLOT(processPendingDatagram()));
//    connect(receiver,SIGNAL(disconnected()),receiver,SLOT(disconnectFromHostImplementation()));

//    connect(ui->UDPServerIPcomboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(BindUDPSocket()));
//    connect(ui->UDPSenderPortlineEdit,SIGNAL(textChanged(QString)),this,SLOT(BindUDPSocket()));

    ui->udpReceivetextEdit->setReadOnly(true);//textChanged(QString)

}

UDPServerForm::~UDPServerForm()
{
    delete _time;
    delete ui;
}

void UDPServerForm::getLocalIP ()
{
    IPlist = QNetworkInterface::allAddresses();

    foreach(QHostAddress IP, IPlist)
    {
        ui->UDPServerIPcomboBox->addItem(IP.toString());
    }
}
//发送”按钮的单击事件槽函数
void UDPServerForm::on_BroadcastpushButton_clicked()
{
    sendmessage();
}

void UDPServerForm::on_UDPSendAutocheckBox_toggled(bool checked)
{
    if (checked)
    {
       int time = ui->timelineEdit->text().toInt();
        _time->start(time);
    }else {
        _time->stop();
    }
}

void UDPServerForm::sendmessageslot()
{
    sendmessage();
}

void UDPServerForm::sendmessage()
{
    if(receiver == NULL) return;
    QByteArray datagram;
    QString sdata= ui->UDPSendertextEdit->toPlainText();
    datagram = sdata.toAscii().data();
    //datagram = QByteArray::fromHex (sdata.toLatin1 ().data ());UDPFarIP

    int port = ui->UDPFarPortlineEdit->text().toInt();
    QString add = ui->UDPFarIPlineEdit->text();
    QHostAddress IP;
    IP.setAddress(add);
    if(true==HexSendFlag)
    {
        QByteArray aa = QByteArray::fromHex (sdata.toLatin1 ().data ());
        sender->writeDatagram(aa.data(),aa.size(),IP,port);
    }
    else
    sender->writeDatagram(datagram.data(),datagram.size(),IP,port);
}

//数据接收及显示槽函数
void UDPServerForm::processPendingDatagram()
{
    if(receiver == NULL) return;
    //int port = ui->UDPFarPortlineEdit->text().toInt();
//    QString  name = receiver->peerName();
//    qDebug()<< name;

    while(receiver->hasPendingDatagrams())//拥有等待的数据报
    {
        QByteArray datagram;//用于存放接收的数据报
        datagram.resize(receiver->pendingDatagramSize());
         //让datagram的大小为等待处理的数据报的大小，这样才能接收到完整的数据
//        receiver->readDatagram(datagram.data(),datagram.size());

        QHostAddress senderAddress;
        quint16 senderPort;
//        quint16 senderPort = ui->UDPSenderPortlineEdit->text().toShort();
        receiver->readDatagram(datagram.data(), datagram.size(),
                                &senderAddress, &senderPort);
        //接收数据报，将其存放到datagram中,senderAddress和senderPort分别存放发送者的IP地址和端口号
        QString sPort = QString::number(senderPort);
        QString sAddress = senderAddress.toString();
        ui->udpReceivetextEdit->moveCursor(QTextCursor::Start,QTextCursor::MoveAnchor);
        //光标移动到开始处
        if(false==DisplayFlag) return;
        if(true==HexDisplayFlag)//16进制显示
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
            ui->udpReceivetextEdit->insertPlainText(tr("From:%1:%2|%3").arg(sAddress).arg(sPort).arg(stt)+"\n");
            //qDebug()<<str;
        }
        else
        ui->udpReceivetextEdit->insertPlainText(tr("From:%1:%2|").arg(sAddress).arg(sPort)+datagram+"\n");
    }
}

void UDPServerForm::BindUDPSocket()
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
    qDebug()<<address;

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
    connect(receiver,SIGNAL(readyRead()),this,SLOT(processPendingDatagram()));
    ui->BroadcastpushButton->setEnabled(true);

    QString add,sip,ssip;
//    QHostAddress IP = receiver->peerAddress();
//    receiver->peek()
    add = receiver->peerAddress().toString();
    sip = sender->peerAddress().toString();
    ssip = receiver->localAddress().toString();
    port = receiver->peerPort();
    qDebug()<<tr("add:")<<add;
    qDebug()<<tr("sip:")<<sip;
    qDebug()<<tr("port:")<<port;
    qDebug()<<tr("ssip:")<<ssip;
}

void UDPServerForm::DisBindUDPSocket()
{
    disconnect(receiver,SIGNAL(readyRead()),this,SLOT(processPendingDatagram()));
    receiver->disconnectFromHost();
    receiver->close();
    receiver = NULL;
    ui->BroadcastpushButton->setEnabled(false);
    ui->UDPServerStatelabel->setText(tr("停止监听!"));
}

void UDPServerForm::on_UDPServerClearpushButton_clicked()
{
    ui->udpReceivetextEdit->setText("");
}


void UDPServerForm::closeEvent(QCloseEvent *e)
{
    emit(closef());
}

//16进制显示
void UDPServerForm::on_HexDisplaycheckBox_toggled(bool checked)
{
    if (checked)
    {
       HexDisplayFlag = true;
    }else
    {
        HexDisplayFlag = false;
    }
}

//16进制发送
void UDPServerForm::on_HexSendcheckBox_toggled(bool checked)
{
    if (checked)
    {
       HexSendFlag = true;
    }else
    {
        HexSendFlag = false;
    }
}

void UDPServerForm::on_UDPDisplayBtn_clicked(bool checked)
{
    if(checked)
    {
        ui->UDPDisplayBtn->setText(tr("继续显示"));
        DisplayFlag = false;
    }
    else
    {
        ui->UDPDisplayBtn->setText(tr("停止显示"));
        DisplayFlag = true;
    }
}


//监听按钮
void UDPServerForm::on_UDPListenpushButton_clicked(bool checked)
{
    if(checked)
    {
        ui->UDPListenpushButton->setText(tr("停止监听"));
        BindUDPSocket();
    }
    else
    {
        ui->UDPListenpushButton->setText(tr("开始监听"));
        DisBindUDPSocket();
    }
}

void UDPServerForm::HandleText()
{
    disconnect (ui->UDPSendertextEdit,SIGNAL(textChanged()),this,SLOT(HandleText()));
    QString AllStr = ui->UDPSendertextEdit->toPlainText ();
    AllStr = AllStr.replace (" ","");

    qDebug () <<"【A】"<<AllStr;
    QString stt;
    for(int i=0;i <AllStr.length ();i+=2)
    {
        QString ss = AllStr.mid(i,2);
        stt +=ss;

        if(i+2 >= AllStr.length ())
        {
            stt += AllStr.mid(i+2);
            break;
        }
        if(ss.length () == 2)
        {
            stt += " ";
        }
    }
    qDebug () <<"【S】" <<stt;

    QString lastStr = stt.mid(stt.length ()-1,1);

    QRegExp regExp("[A-Fa-f0-9]{1}");//创建了一个正则表达式模式
    bool match = regExp.exactMatch(lastStr);
    if(true != match)
    {
       stt = stt.mid(0,stt.length ()-1);
    }

    stt = stt.toUpper ();
    ui->UDPSendertextEdit->clear ();
    ui->UDPSendertextEdit->insertPlainText (stt);
    connect (ui->UDPSendertextEdit,SIGNAL(textChanged()),this,SLOT(HandleText()));
}


