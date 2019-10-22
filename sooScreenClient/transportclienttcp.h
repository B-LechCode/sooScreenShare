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
#include <QElapsedTimer>
#include <QTimer>
#include <iostream>
namespace trans {
    #define PORT "port"
    #define HOST_ADDRESS "hostAddr"
}

/**
 * @brief The transport client tcp class
 * This class implements tcp communication based on the Qt library
 */
class transportClientTCP :private QObject, public ItransportClient
{
    Q_OBJECT
public:
    /**
     * @brief The standard constructor
     *
     */
    transportClientTCP();
    /**
     * @brief The destructor
     *
     */
    virtual ~transportClientTCP();
    /**
     * @brief Init method
     * This method is used to start the communication
     */
    virtual void init();
    /**
    * @brief Send data over the connection
    *
    * @param dat Pointer to the data
    * @param len Length of the data
    * @return int64_t Return value of the socket, which is the number of sent bytes or < 0  when an error occured
    */
    virtual int64_t send(const char* dat, int64_t len);

private slots:   
    /**
     * @brief The socked disconnected slot
     * Initiates a reconnect
     */
    void on_socketDisconnected();
    /**
     * @brief The socket connected slot
     * Only used for messaging
     */
    void on_socketConnected();
    /**
     * @brief The socket ready read slot
     * This is called when data is available
     */
    void on_readyRead();
    /**
     * @brief The timer timeout slot
     * This starts the timed reconnect of the socket
     */
    void on_timerTimeout();
private:    
    /**
     * @brief Ends all actions: Closes the socket
     * Call this prior reconnect
     */
    void end();
    /**
     * @brief Translates the parameters to the member variables
     *
     */
    void initParameters();
    /**
     * @brief The changed event of the underlying parameter map
     *
     */
    virtual void parameterMapChangedEvent();
    /**
     * @brief The changed event of a key/value pair
     *
     * @param key The key of the changed parameter
     */
    virtual void parameterChangedEvent(const std::string& key);
    /**
     * @brief The new message event
     * Will be called when a new message is available
     * @param str The message
     */
    void notifyMessage(const char* str);
    /**
     * @brief The new message event
     * Will be called when a new message is available
     * @param str The message
     */
    void notifyMessage(const std::string& str);

    QTcpSocket m_sock; /**< The socket */
    uint16_t   m_port; /**< The port to connect to */
    QHostAddress m_address; /**< The address to connect to */

    QByteArray m_recData; /**< The last received data */
    QTimer               m_reconnectTimer; /**< Timer for reconnecting */
    bool                 m_noReconnect = false; /**< Flag for supressing the timed reconnect */
};

#endif // TRANSPORTSERVERTCP_H
