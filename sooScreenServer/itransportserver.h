//sooScreenShare by Simon Wezstein (B-LechCode), 2019
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */
#ifndef ITRANSPORTSERVER_H
#define ITRANSPORTSERVER_H

#include <map>
#include <string>
#include <list>

#include "./../parameter.h"
#include "./../iparameter.h"

/**
 * @brief The observer interface for the transport client
 * This interface is currently used to pass messages to the observer.
 */
class ItransportServerObserver
{
public:
    /**
     * @brief The standard constructor
     *
     */
    ItransportServerObserver();
    /**
     * @brief The destructor
     *
     */
    virtual ~ItransportServerObserver();
    /**
     * @brief The new message event
     * Will be called when a new message is available
     * @param str The message
     */
    virtual void transportNewMessage(std::string str)=0;
};

/**
 * @brief Generic interaface for transport server backends
 * This interface describes the interaction between the main worker and the transport server backend.
 */
class ItransportServer:public Iparameter
{
public:
    /**
     * @brief The standard constructor
     *
     */
    ItransportServer();
    /**
     * @brief The destructor
     */
    virtual ~ItransportServer();
    /**
     * @brief Setter method for the current observer
     *
     * @param observer The observer to be set
     */
    virtual void setObserver(ItransportServerObserver* observer);
    /**
     * @brief Init method
     * This method is used to start the communication
     */
    virtual void init() = 0;
    /**
     * @brief Send data over the connection
     *
     * @param dat Pointer to the data
     * @param len Length of the data
     * @return int64_t Return value of the socket, which is the number of sent bytes or < 0  when an error occured
     */
    virtual int64_t send(const char* dat, int64_t len) = 0;
protected:
    ItransportServerObserver* m_observer = nullptr; /**< The vector of registred backends */
};

#endif // ITRANSPORTSERVER_H
