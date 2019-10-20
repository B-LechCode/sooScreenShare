//SooScreenServer by Simon Wezstein (B-LechCode), 2019
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */
#include "transportservertcp.h"

transportServerTCP::transportServerTCP()
{
    m_defaultParameters[PORT] = parameter("The TCP port to listen on","int16","12345");
    m_defaultParameters[HOST_ADDRESS] = parameter("The address/network interface to listen on","string","any");
    setParameters(m_defaultParameters);
    init();
}

transportServerTCP::~transportServerTCP()
{
   end();
}

void transportServerTCP::setParameters(parameterMap &para)
{
    ItransportServer::setParameters(para);

    initParameters();
}

void transportServerTCP::init()
{
    notifyMessage("start listening on "+m_parameters[HOST_ADDRESS].value()+":"+m_parameters[PORT].value());
    connect(&m_srvr,SIGNAL(newConnection()),this,SLOT(on_newConnection()));
    m_srvr.listen(m_interface,m_port);
}

int64_t transportServerTCP::send(const char *dat, int64_t len)
{
    int64_t retVal = -1;
    if(m_ptrSock)
    {       
        retVal =  m_ptrSock->write(dat,len);

        /*while(m_ptrSock->bytesToWrite())
            m_ptrSock->flush();*/
    }

    return retVal;
}

void transportServerTCP::on_newConnection()
{
    notifyMessage("new connection!");
    QTcpSocket* ptrSock = m_srvr.nextPendingConnection();
    m_ptrSock = ptrSock;
    connect(m_ptrSock,SIGNAL(disconnected()),this,SLOT(on_socketDisconnected()));
    m_srvr.close();
}

void transportServerTCP::on_socketDisconnected()
{
    notifyMessage("connection closed!");

    disconnect(m_ptrSock,SIGNAL(disconnected()),this,SLOT(on_socketDisconnected()));

    init();
}

void transportServerTCP::end()
{
    disconnect(&m_srvr,SIGNAL(newConnection()),this,SLOT(on_newConnection()));
    if(m_ptrSock)
    {
        m_ptrSock->close();
    }
    m_srvr.close();
}

void transportServerTCP::initParameters()
{
    //port
    m_port = static_cast<uint16_t>(std::stoi(m_parameters[PORT].value()));

    //network interface
    QHostAddress addr;
    std::string  par = m_parameters[HOST_ADDRESS].value();
    if(par == "any")
        addr = QHostAddress::Any;
    else if(par == "anyV4")
        addr = QHostAddress::AnyIPv4;
    else if(par == "anyV6")
        addr = QHostAddress::AnyIPv6;
    else
    {
        addr.setAddress(QString(par.c_str()));
    }
}

void transportServerTCP::notifyMessage(const char* str)
{
    notifyMessage(std::string(str));
}

void transportServerTCP::notifyMessage(const std::string &str)
{
    if(m_observer)
        m_observer->transportNewMessage(str);
}

void transportServerTCP::parameterMapChangedEvent()
{
   end();
   initParameters();
   init();
}

void transportServerTCP::parameterChangedEvent(const std::string &key)
{
    end();
    initParameters();
    init();
}
