//sooScreenShare by Simon Wezstein (B-LechCode), 2019
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
    #define MAX_BUFFER_UTILIZATION "maxBuffUtil"
}

/**
 * @brief The transport server tcp class
 * This class implements tcp communication based on the Qt library
 */
class transportServerTCP :private QObject, public ItransportServer
{
    Q_OBJECT
public:
    /**
     * @brief The standard constructor
     *
     */
    transportServerTCP();
    /**
     * @brief The destructor
     *
     */
    virtual ~transportServerTCP();
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
     * @brief The on new connection slot of the tcp server
     * This is called if a client connects
     */
    void on_newConnection();
    /**
     * @brief The socked disconnected slot
     * Initiates a restart of the tcp server (listening for connections)
     */
    void on_socketDisconnected();
private:
    QTcpServer m_srvr; /**< The tcp sercer */
    QTcpSocket* m_ptrSock = nullptr; /**< Pointer to the connected socket */
    uint16_t   m_port; /**< The port to listen on */
    int32_t   m_maxBuffUtil;
    QHostAddress m_interface; /**< The network interface address to listen on */
    /**
     * @brief Ends all actions: Closes the socket, Stops the tcp server
     * Call this prior tcp server restart
     */
    void end();
    /**
     * @brief Translates the parameters to the member variables
     *
     */
    void initParameters();
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


};

#endif // TRANSPORTSERVERTCP_H
