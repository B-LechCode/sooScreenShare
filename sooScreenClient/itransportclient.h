#ifndef ITRANSPORTSERVER_H
#define ITRANSPORTSERVER_H

#include <map>
#include <string>
#include <list>
#include <vector>
#include "parameter.h"

class ItransportClientObserver
{
public:
    ItransportClientObserver();
    virtual ~ItransportClientObserver();
    virtual void transportNewMessage(std::string str)=0;
    virtual void transportDataAvailable(std::vector<uint8_t> data)=0;
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
    virtual void removeObserverSubscriber(ItransportClientObserver& o);
    virtual void init() = 0;
    virtual int64_t send(const char* dat, int64_t len) = 0;
protected:
    std::list<ItransportClientObserver*> m_observers;
    parameterMap m_defaultParameters;
    parameterMap m_parameters;

};

#endif // ITRANSPORTSERVER_H
