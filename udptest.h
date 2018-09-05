#ifndef UDPTEST_H
#define UDPTEST_H

class UDPTest
{
public:
    UDPTest();
    void sendmessage();

private slots:
    void on_BroadcastpushButton_clicked();
    void on_UDPSendAutocheckBox_toggled(bool checked);
    void sendmessageslot();
    void processPendingDatagram();

    void on_UDPServerClearpushButton_clicked();

    void on_HexDisplaycheckBox_toggled(bool checked);

    void on_HexSendcheckBox_toggled(bool checked);

    void on_UDPDisplayBtn_clicked(bool checked);

    void on_UDPListenpushButton_clicked(bool checked);

    void HandleText();

private:
    //Ui::UDPServerForm *ui;
    QUdpSocket *sender;
    QUdpSocket *receiver;
    QTimer* _time;
    QList<QHostAddress> IPlist;
    void getLocalIP();
    void BindUDPSocket();
    void DisBindUDPSocket();

    bool HexDisplayFlag;
    bool HexSendFlag;
    bool DisplayFlag;
};

#endif // UDPTEST_H
