//SooScreenServer by Simon Wezstein (B-LechCode), 2019
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */
#ifndef ITRANSPORTSERVER_H
#define ITRANSPORTSERVER_H

#include <map>
#include <string>
#include <list>

#include "./../parameter.h"

class ItransportServerObserver
{
public:
    ItransportServerObserver();
    virtual ~ItransportServerObserver();
    virtual void transportNewMessage(std::string str)=0;
};

class ItransportServer
{
public:
    ItransportServer();
    virtual ~ItransportServer();
    virtual parameterMap getDefaultParameters();
    virtual void setParameters(parameterMap& para);
    virtual parameterMap getParameters();
    virtual void addObserverSubscriber(ItransportServerObserver& o);
    virtual void removeObserverSubscriber(ItransportServerObserver& o);
    virtual void init() = 0;
    virtual int64_t send(const char* dat, int64_t len) = 0;
protected:
    std::list<ItransportServerObserver*> m_observers;
    parameterMap m_defaultParameters;
    parameterMap m_parameters;

};

#endif // ITRANSPORTSERVER_H
