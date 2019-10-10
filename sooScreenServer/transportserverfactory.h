#ifndef TRANSPORTSERVERFACTORY_H
#define TRANSPORTSERVERFACTORY_H

#define qtTcpServer "qtTCP"

#include "itransportserver.h"
#include "transportservertcp.h"


class transportServerFactory
{
    static std::vector<std::string> m_backends;
public:
    static const std::vector<std::string>& getAvailableBackends()
    {
        if(!m_backends.size())
        {
            m_backends.push_back(qtTcpServer);
        }
        return  m_backends;
    }

    static ItransportServer* getBackend(std::string backendName)
    {
        if(backendName == qtTcpServer)
            return new transportServerTCP();

        return nullptr;
    }
};
#endif // TRANSPORTSERVERFACTORY_H
