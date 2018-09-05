#ifndef COM_H
#define COM_H

#include <QDialog>
#include "ui_mainwindow.h"
#include "tcpudpcombase.h"
#include "qextserial/qextserialport.h"

//延时，TIME_OUT是串口读写的延时
#define TIME_OUT 500

namespace Ui {
    class COM;
}

class COM : public QDialog,public TcpUdpComBase
{
    Q_OBJECT

public:
    explicit COM(QWidget *parent = 0);
    ~COM();

    void setMainUiPoint(Ui::MainWindow *ui){main_ui=ui;}
    bool WorkOpen();                                        //打开串口
    void WorkClose();                                       //关闭串口
    void WorkSendMessage();                                 //发送数据
    void WorkSetDisplayFlag(bool flag){DisplayFlag = flag;}

private slots:
    void readMyCom();                                       //读取串口，将串口的数据显示
    void on_OKpushButton_clicked();
    void on_btn_add_clicked();

private:
    Ui::COM *ui;
    Ui::MainWindow *main_ui;
    bool DisplayFlag;
    QextSerialPort *myCom; //声明对象//串口
    bool m_isConnect;

};

#endif // COM_H
