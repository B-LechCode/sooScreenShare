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

/**
 * @brief
 *
 */
class transportServerTCP :private QObject, public ItransportServer
{
    Q_OBJECT
public:
    /**
     * @brief
     *
     */
    transportServerTCP();
    /**
     * @brief
     *
     */
    virtual ~transportServerTCP();
    /**
     * @brief
     *
     */
    virtual void init();
    /**
     * @brief
     *
     * @param dat
     * @param len
     * @return int64_t
     */
    virtual int64_t send(const char* dat, int64_t len);
private slots:
    /**
     * @brief
     *
     */
    void on_newConnection();
    /**
     * @brief
     *
     */
    void on_socketDisconnected();
private:
    QTcpServer m_srvr; /**< TODO: describe */
    QTcpSocket* m_ptrSock = nullptr; /**< TODO: describe */
    uint16_t   m_port; /**< TODO: describe */
    QHostAddress m_interface; /**< TODO: describe */
    /**
     * @brief
     *
     */
    void end();
    /**
     * @brief
     *
     */
    void initParameters();
    /**
     * @brief
     *
     * @param str
     */
    void notifyMessage(const char* str);
    /**
     * @brief
     *
     * @param str
     */
    void notifyMessage(const std::string& str);

    /**
     * @brief
     *
     */
    virtual void parameterMapChangedEvent();
    /**
     * @brief
     *
     * @param key
     */
    virtual void parameterChangedEvent(const std::string& key);


};

#endif // TRANSPORTSERVERTCP_H
