#ifndef TRANSPORTSERVERFACTORY_H
#define TRANSPORTSERVERFACTORY_H

#define qtTcpclient "qtTCP"

#include "itransportclient.h"
#include "transportclienttcp.h"


class transportClientFactory
{
    static std::vector<std::string> m_backends;
public:
    static const std::vector<std::string>& getAvailableBackends()
    {
        if(!m_backends.size())
        {
            m_backends.push_back(qtTcpclient);
        }
        return  m_backends;
    }

    static ItransportClient* getBackend(std::string backendName)
    {
        if(backendName == qtTcpclient)
            return new transportClientTCP();

        return nullptr;
    }
};
#endif // TRANSPORTSERVERFACTORY_H
