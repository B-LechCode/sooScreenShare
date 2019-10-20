//SooScreenServer by Simon Wezstein (B-LechCode), 2019
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */
#ifndef TRANSPORTSERVERFACTORY_H
#define TRANSPORTSERVERFACTORY_H

#define qtTcpclient "qtTCP"

#include "itransportclient.h"
#include "transportclienttcp.h"


class transportClientFactory
{
    transportClientFactory(){}
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
