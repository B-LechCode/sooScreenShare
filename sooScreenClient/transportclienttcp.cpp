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
    auto l = m_sock.readAll();

    //We can do this, because we are sure that we won't sacrifice existing data in the Buffer!
    m_recData = std::vector<uint8_t>(l.size());
    memcpy(m_recData.data(),l.data(),l.size());

    (*m_observers.begin())->transportDataAvailable(std::move(m_recData));
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
    (*m_observers.begin())->transportNewMessage(str);
}

void transportClientTCP::on_socketDisconnected()
{
    transportClientTCP::init();
    notifyMessage("connection closed!");
}

void transportClientTCP::on_socketConnected()
{
    notifyMessage("connection opened!");
}
