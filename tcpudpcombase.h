#ifndef TCPUDPCOMBASE_H
#define TCPUDPCOMBASE_H

//#include "ui_mainwindow.h"

class TcpUdpComBase
{
public:
    TcpUdpComBase();
    //virtual void WorkPeiZhi()=0;
    virtual bool WorkOpen()=0;
    virtual void WorkClose()=0;
    virtual void WorkSendMessage()=0;
    virtual void WorkSetDisplayFlag(bool)=0;
    void setIsHexSend(bool isHexSend){m_isHexSend = isHexSend;}
    void setIsHexDisplay(bool isHexDisplay){m_isHexDisplay = isHexDisplay;}

public:
    bool m_isHexSend;   //是否16进制发送
    bool m_isHexDisplay;//是否16进制显示
};

#endif // TCPUDPCOMBASE_H
