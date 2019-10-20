//SooScreenServer by Simon Wezstein (B-LechCode), 2019
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */
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
    void on_socketDisconnected();
private:
    QTcpServer m_srvr;
    QTcpSocket* m_ptrSock = nullptr;
    uint16_t   m_port;
    QHostAddress m_interface;
    void end();
    void initParameters();
    void notifyMessage(const char* str);
    void notifyMessage(const std::string& str);

    virtual void parameterMapChangedEvent();
    virtual void parameterChangedEvent(const std::string& key);


};

#endif // TRANSPORTSERVERTCP_H
