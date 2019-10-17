//SooScreenServer by Simon Wezstein (B-LechCode), 2019
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */
#include "transportclienttcp.h"




transportClientTCP::transportClientTCP()/*:m_pollInterface(&m_sock)*/
{
    m_defaultParameters[PORT] = parameter("The TCP port to connect on","int16","12345");
    m_defaultParameters[HOST_ADDRESS] = parameter("The address interface to connect on","string","127.0.0.1");
    setParameters(m_defaultParameters);

    connect(&m_sock,SIGNAL(disconnected()),this,SLOT(on_socketDisconnected()));
    connect(&m_sock,SIGNAL(connected()),this,SLOT(on_socketConnected()));
    connect(&m_sock,SIGNAL(readyRead()),this,SLOT(on_readyRead()));
    connect(&m_tmr,SIGNAL(timeout()),this,SLOT(on_timerTimeout()));
    m_tmr.setSingleShot(true);

}

transportClientTCP::~transportClientTCP()
{
    notifyMessage("destruct tcp!");
    m_noReconnect = true;
    m_sock.close();
}

void transportClientTCP::setParameters(parameterMap &para)
{
    ItransportClient::setParameters(para);

    //port
    m_port = static_cast<uint16_t>(std::stoi(m_defaultParameters[PORT].value()));

    //network interface    
    std::string  par = m_defaultParameters[HOST_ADDRESS].value();   
    m_address.setAddress(QString(par.c_str()));

}

void transportClientTCP::init()
{
    notifyMessage("start connecting on "+m_parameters[HOST_ADDRESS].value()+":"+m_parameters[PORT].value());
    m_sock.close();
    m_sock.connectToHost(m_address,m_port);
    if(!m_sock.waitForConnected()) //Retry
    {
        notifyMessage("Error connecting! Starting retry timer");
        m_tmr.start(3000);
    }

}

int64_t transportClientTCP::send(const char *dat, int64_t len)
{
    int64_t retVal = -1;
    if(m_sock.isWritable())
    {
        retVal =  m_sock.write(dat,len);
        while(m_sock.bytesToWrite())
            m_sock.flush();
    }

    return retVal;
}

void transportClientTCP::on_message(const QString msg)
{
    notifyMessage(msg.toStdString());
}

void transportClientTCP::on_readyRead()
{
    m_recData = m_sock.readAll();

    if(m_observer)
        (m_observer)->transportDataAvailable(m_recData.data(),m_recData.length()/*std::move(m_recData)*/);
}

void transportClientTCP::on_timerTimeout()
{
     init();
}

void transportClientTCP::notifyMessage(const char* str)
{
    notifyMessage(std::string(str));
}

void transportClientTCP::notifyMessage(const std::string &str)
{
    if(m_observer)
        (m_observer)->transportNewMessage(str);
}

void transportClientTCP::on_socketDisconnected()
{
    if(!m_noReconnect)
    {
        transportClientTCP::init();
        notifyMessage("connection closed!");
    }
}

void transportClientTCP::on_socketConnected()
{
    notifyMessage("connection opened!");
}
