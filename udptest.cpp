#include "udptest.h"

UDPTest::UDPTest()
{
}

void UDPTest::sendmessage()
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

void UDPTest::getLocalIP()
{
}

void UDPTest::BindUDPSocket()
{
}

void UDPTest::DisBindUDPSocket()
{
}
