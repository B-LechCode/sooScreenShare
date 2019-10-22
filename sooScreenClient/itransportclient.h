//SooScreenServer by Simon Wezstein (B-LechCode), 2019
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */
#ifndef ITRANSPORTSERVER_H
#define ITRANSPORTSERVER_H

#include <map>
#include <string>
#include <list>
#include <vector>
#include "./../parameter.h"
#include "./../iparameter.h"

/**
 * @brief The observer interface for the transport client
 * This interface is currently used to pass messages and data to the observer.
 */
class ItransportClientObserver
{
public:
    /**
     * @brief The standard constructor
     *
     */
    ItransportClientObserver();
    /**
     * @brief The destructor
     *
     */
    virtual ~ItransportClientObserver();
    /**
     * @brief The new message event
     * Will be called when a new message is available
     * @param str The message
     */
    virtual void transportNewMessage(std::string str)=0;
    /**
     * @brief The new data available event
     * Will be called when new data is available
     * @param dat Pointer to the new data. Don't delete this!
     * @param len The length of the data
     */
    virtual void transportDataAvailable(const char *dat, int64_t len)=0;
};

/**
 * @brief Generic interaface for transport client backends
 * This interface describes the interaction between the main worker and the transport client backend.
 */
class ItransportClient: public Iparameter
{
public:
    /**
     * @brief The standard constructor
     *
     */
    ItransportClient();
    /**
     * @brief The destructor
     */
    virtual ~ItransportClient();
    /**
     * @brief Getter method for the current observer
     *
     * @return IdrawObserver The current observer
     */
    virtual ItransportClientObserver *observer() const;
    /**
     * @brief Setter method for the current observer
     *
     * @param observer The observer to be set
     */
    virtual void setObserver(ItransportClientObserver* observer);
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
    ItransportClientObserver* m_observer; /**< The vector of registred backends */
};

#endif // ITRANSPORTSERVER_H
