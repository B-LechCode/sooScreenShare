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

class ItransportClientObserver
{
public:
    ItransportClientObserver();
    virtual ~ItransportClientObserver();
    virtual void transportNewMessage(std::string str)=0;    
    virtual void transportDataAvailable(const char *dat, int64_t len)=0;
};

class ItransportClient
{
public:
    ItransportClient();
    virtual ~ItransportClient();
    virtual parameterMap getDefaultParameters();
    virtual void setParameters(parameterMap& para);
    virtual parameterMap getParameters();
    virtual void addObserverSubscriber(ItransportClientObserver& o);
    virtual void removeObserverSubscriber();
    virtual void init() = 0;
    virtual int64_t send(const char* dat, int64_t len) = 0;
protected:
    ItransportClientObserver* m_observer;
    parameterMap m_defaultParameters;
    parameterMap m_parameters;

};

#endif // ITRANSPORTSERVER_H
