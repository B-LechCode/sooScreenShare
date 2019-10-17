//SooScreenServer by Simon Wezstein (B-LechCode), 2019
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */
#ifndef TRANSPORTSERVERTCP_H
#define TRANSPORTSERVERTCP_H

#include "itransportclient.h"

#include <QObject>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QHostAddress>

#include <thread>
#include <chrono>
#include <QString>
#include <QThread>
#include <QMutex>
#include <QElapsedTimer>
#include <QTimer>
#include <iostream>
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

    void on_message(const QString msg);
    void on_socketDisconnected();
    void on_socketConnected();
    void on_readyRead();
    void on_timerTimeout();
private:    
    QTcpSocket m_sock;
    uint16_t   m_port;
    QHostAddress m_address;

    QByteArray m_recData;
    QTimer               m_tmr;
    bool                 m_noReconnect = false;

    void notifyMessage(const char* str);
    void notifyMessage(const std::string& str);
};

#endif // TRANSPORTSERVERTCP_H
