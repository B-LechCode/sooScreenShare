#ifndef TRANSPORTSERVERTCP_H
#define TRANSPORTSERVERTCP_H

#include "itransportserver.h"

#include <QObject>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QHostAddress>

namespace trans {
    #define PORT "port"
    #define HOST_ADDRESS "hostAddr"
}

class transportServerTCP :private QObject, public ItransportServer
{
    Q_OBJECT
public:
    transportServerTCP();
    virtual ~transportServerTCP();
    virtual void setParameters(parameterMap& para);
    virtual void init();
    virtual int64_t send(const char* dat, int64_t len);
private slots:
    void on_newConnection();
    //void on_timerTimeout();
    void on_socketDisconnected();
private:
    QTcpServer m_srvr;
    QTcpSocket* m_ptrSock = nullptr;
    uint16_t   m_port;
    QHostAddress m_interface;

    void notifyMessage(const char* str);
    void notifyMessage(const std::string& str);
};

#endif // TRANSPORTSERVERTCP_H
