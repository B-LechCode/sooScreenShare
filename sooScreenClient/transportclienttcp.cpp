#include "transportclienttcp.h"

transportClientTCP::transportClientTCP()
{
    m_defaultParameters[PORT] = parameter("The TCP port to connect on","int16","12345");
    m_defaultParameters[HOST_ADDRESS] = parameter("The address interface to connect on","string","127.0.0.1");
    setParameters(m_defaultParameters);
    connect(&m_sock,SIGNAL(disconnected()),this,SLOT(on_socketDisconnected()));
    connect(&m_sock,SIGNAL(connected()),this,SLOT(on_socketConnected()));

}

transportClientTCP::~transportClientTCP()
{

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
    m_sock.connectToHost(m_address,m_port);
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


void transportClientTCP::on_socketDisconnected()
{
    notifyMessage("connection closed!");

    init();
}

void transportClientTCP::on_socketConnected()
{
    notifyMessage("connection opened!");
}

void transportClientTCP::notifyMessage(const char* str)
{
    notifyMessage(std::string(str));
}

void transportClientTCP::notifyMessage(const std::string &str)
{
    auto itObs = m_observers.begin();

    while(itObs != m_observers.end())
    {
        if(*itObs)
        {
            (*itObs)->transportNewMessage(str);
        }
        itObs++;
    }
}
