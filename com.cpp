#include "com.h"
#include "ui_com.h"
#include <QDebug>
#include <QMessageBox>
#include <QInputDialog>

COM::COM(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::COM)
{
    ui->setupUi(this);
    DisplayFlag = true;
    m_isConnect = false;
    myCom = NULL;

#ifdef Q_OS_LINUX
   ui->portNameComboBox->addItem( "ttyUSB0");
   ui->portNameComboBox->addItem( "ttyUSB1");
   ui->portNameComboBox->addItem( "ttyUSB2");
   ui->portNameComboBox->addItem( "ttyUSB3");
   ui->portNameComboBox->addItem( "ttyS0");
   ui->portNameComboBox->addItem( "ttyS1");
   ui->portNameComboBox->addItem( "ttyS2");
   ui->portNameComboBox->addItem( "ttyS3");
   ui->portNameComboBox->addItem( "ttyS4");
   ui->portNameComboBox->addItem( "ttyS5");
   ui->portNameComboBox->addItem( "ttyS6");
   ui->portNameComboBox->addItem( "ttySNX0");
   ui->portNameComboBox->addItem( "ttySNX1");
#elif defined (Q_OS_WIN)
   ui->portNameComboBox->addItem("COM0");
   ui->portNameComboBox->addItem("COM1");
   ui->portNameComboBox->addItem("COM2");
   ui->portNameComboBox->addItem("COM3");
   ui->portNameComboBox->addItem("COM4");
   ui->portNameComboBox->addItem("COM5");
   ui->portNameComboBox->addItem("COM6");
   ui->portNameComboBox->addItem("COM7");
   ui->portNameComboBox->addItem("COM8");
   ui->portNameComboBox->addItem("COM9");
   ui->portNameComboBox->addItem("COM10");
   ui->portNameComboBox->addItem("COM11");
   ui->portNameComboBox->addItem("COM12");
   ui->portNameComboBox->addItem("COM13");
   ui->portNameComboBox->addItem("COM14");
   ui->portNameComboBox->addItem("COM15");
#endif
}

COM::~COM()
{
    delete ui;
}

//读串口函数
void COM::readMyCom()
{
    QByteArray byte_data = myCom->readAll();
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

    //读取串口缓冲区的所有数据给临时变量temp
    if(true==DisplayFlag)
    {
//        ui->textBrowser->insertPlainText(temp);
//        //将串口的数据显示在窗口的文本浏览器中
        main_ui->ViewtextEdit->insertPlainText(strDisplay);
    }
}

void COM::on_OKpushButton_clicked()
{
    if( myCom != NULL)
        delete myCom;
    WorkOpen();
    this->close();
}

bool COM::WorkOpen()
{
    //获取串口名
    QString portName = ui->portNameComboBox->currentText();
#ifdef Q_OS_LINUX
    myCom = new QextSerialPort("/dev/" + portName);
#elif defined (Q_OS_WIN)
    myCom = new QextSerialPort(portName);
#endif
    connect(myCom, SIGNAL(readyRead()), this, SLOT(readMyCom()));

    //设置波特率
    myCom->setBaudRate((BaudRateType)ui->baudRateComboBox->currentText().toInt());

    //设置数据位
    myCom->setDataBits((DataBitsType)ui->dataBitsComboBox->currentText().toInt());

    //设置校验
    switch(ui->parityComboBox->currentIndex())
    {
    case 0:
        myCom->setParity(PAR_NONE);
        break;
    case 1:
        myCom->setParity(PAR_ODD);
        break;
    case 2:
        myCom->setParity(PAR_EVEN);
        break;
    default:
        myCom->setParity(PAR_NONE);
        qDebug("set to default : PAR_NONE");
        break;
    }

    //设置停止位
    switch(ui->stopBitsComboBox->currentIndex())
    {
    case 0:
        myCom->setStopBits(STOP_1);
        break;
    case 1:
 #ifdef Q_OS_WIN
        myCom->setStopBits(STOP_1_5);
 #endif
        break;
    case 2:
        myCom->setStopBits(STOP_2);
        break;
    default:
        myCom->setStopBits(STOP_1);
        qDebug("set to default : STOP_1");
        break;
    }

    //设置数据流控制
    myCom->setFlowControl(FLOW_OFF);
    //设置延时
    myCom->setTimeout(TIME_OUT);

    bool flag = myCom->open(QIODevice::ReadWrite);
    if(false==flag)
    {
        m_isConnect = false;
        QMessageBox::warning(this,tr("Warning"),tr("串口不存在或已被占用!"),QMessageBox::Yes);
        main_ui->Statelabel->setText(tr("串口不存在或已被占用!"));
        return false;
    }

    ui->baudRateComboBox->setEnabled(false);  //
    ui->dataBitsComboBox->setEnabled(false);
    ui->parityComboBox->setEnabled(false);
    ui->stopBitsComboBox->setEnabled(false);
    ui->portNameComboBox->setEnabled(false);
    m_isConnect = true;
    return true;
}

void COM::WorkClose()
{
    if(!m_isConnect)    return;

    myCom->close();  //
    main_ui->Statelabel->setText(tr("已关闭串口测试软件！"));

    ui->baudRateComboBox->setEnabled(true);
    ui->dataBitsComboBox->setEnabled(true);
    ui->parityComboBox->setEnabled(true);
    ui->stopBitsComboBox->setEnabled(true);
    ui->portNameComboBox->setEnabled(true);
}

void COM::WorkSendMessage()
{
    if(!(myCom->isOpen()))  return;

    //QByteArray datagram;
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
    myCom->write(sendBuf);
}

//添加串口
void COM::on_btn_add_clicked()
{
    bool ok = false;
    QString portname = QInputDialog::getText(this, tr("添加串口"), tr("设备文件名"), QLineEdit::Normal, 0, &ok);
    if(ok && !portname.isEmpty())
    {
        int res = ui->portNameComboBox->findText(portname);
        if(res >= 0)
        {
            QMessageBox::warning(this,tr("Warning"),tr("该串口已存在，不允许重复添加!"),QMessageBox::Yes);
            return;
        }
        ui->portNameComboBox->addItem(portname);
//        ui->statusBar->showMessage(tr("添加串口成功"));
    }
}
