//sooScreenShare by Simon Wezstein (B-LechCode), 2019
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */
#ifndef TRANSPORTSERVERFACTORY_H
#define TRANSPORTSERVERFACTORY_H

#define qtTcpServer "qtTCP"

#include "itransportserver.h"
#include "transportservertcp.h"

/**
 * @brief The factory class for the transport server backends.
 *
 */
class transportServerFactory
{
    /**
     * @brief The standard constructor
     *
     */
    transportServerFactory(){}
    static std::vector<std::string> m_backends;  /**< The vector of registred backends */
public:
    /**
   * @brief Function for listing the available backends
   * This function will return the available, registred Backends.
   * If you want to add a new backend, append a push_back call with the identification string of your new backend.
   * @return const std::vector<std::string> Available backends
   */
    static const std::vector<std::string>& getAvailableBackends()
    {
        if(!m_backends.size())
        {
            m_backends.push_back(qtTcpServer);
        }
        return  m_backends;
    }
    /**
    * @brief Creates a new backend object of the specified type
    * This function will return a new object of the specified type.
    * If a wrong backend name was specified nullptr is returned.
    * @param backendName The desired backend type specifier
    * @return IImageDecompressor The pointer to the created backend. Nullptr if invalid type specified
    */
    static ItransportServer* getBackend(std::string backendName)
    {
        if(backendName == qtTcpServer)
            return new transportServerTCP();

        return nullptr;
    }
};
#endif // TRANSPORTSERVERFACTORY_H
