#ifndef TRANSPORTSERVERTCP_H
#define TRANSPORTSERVERTCP_H

#include "itransportclient.h"

#include <QObject>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QHostAddress>

namespace trans {
    #define PORT "port"
    #define HOST_ADDRESS "hostAddr"
}

class transportClientTCP :private QObject, public ItransportClient
{
    Q_OBJECT
public:
    transportClientTCP();
    virtual ~transportClientTCP();
    virtual void setParameters(parameterMap& para);
    virtual void init();
    virtual int64_t send(const char* dat, int64_t len);
private slots:

    //void on_timerTimeout();
    void on_socketDisconnected();
    void on_socketConnected();
private:    
    QTcpSocket m_sock;
    uint16_t   m_port;
    QHostAddress m_address;

    void notifyMessage(const char* str);
    void notifyMessage(const std::string& str);
};

#endif // TRANSPORTSERVERTCP_H
