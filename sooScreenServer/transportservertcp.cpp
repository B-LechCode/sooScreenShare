#include "transportservertcp.h"

transportServerTCP::transportServerTCP()
{
    m_defaultParameters[PORT] = parameter("The TCP port to listen on","int16","12345");
    m_defaultParameters[HOST_ADDRESS] = parameter("The address/network interface to listen on","string","any");
    setParameters(m_defaultParameters);
    connect(&m_srvr,SIGNAL(newConnection()),this,SLOT(on_newConnection()));
}

transportServerTCP::~transportServerTCP()
{

}

void transportServerTCP::setParameters(parameterMap &para)
{
    ItransportServer::setParameters(para);

    //port
    m_port = static_cast<uint16_t>(std::stoi(m_defaultParameters[PORT].value()));

    //network interface
    QHostAddress addr;
    std::string  par = m_defaultParameters[HOST_ADDRESS].value();
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

void transportServerTCP::init()
{
    notifyMessage("start listening on "+m_parameters[HOST_ADDRESS].value()+":"+m_parameters[PORT].value());
    m_srvr.listen(m_interface,m_port);
}

int64_t transportServerTCP::send(const char *dat, int64_t len)
{
    int64_t retVal = -1;
    if(m_ptrSock)
    {
        retVal =  m_ptrSock->write(dat,len);
        while(m_ptrSock->bytesToWrite())
            ;//m_ptrSock->flush();
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

void transportServerTCP::notifyMessage(const char* str)
{
    notifyMessage(std::string(str));
}

void transportServerTCP::notifyMessage(const std::string &str)
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
