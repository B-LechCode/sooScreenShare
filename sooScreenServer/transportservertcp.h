#ifndef TRANSPORTSERVERTCP_H
#define TRANSPORTSERVERTCP_H

#include "itransportserver.h"

#include <QtNetwork/QTcpServer>
#include <QtNetwork/QHostAddress>

namespace trans {
    #define PORT
}

class transportServerTCP : public ItransportServer
{
public:
    transportServerTCP();
    virtual ~transportServerTCP();
    virtual void setParameters(parameterMap& para);
    virtual void init();
    virtual void send(const char* dat, int32_t len);
private:
    QTcpServer m_srvr;
};

#endif // TRANSPORTSERVERTCP_H
