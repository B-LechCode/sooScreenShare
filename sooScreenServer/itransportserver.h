#ifndef ITRANSPORTSERVER_H
#define ITRANSPORTSERVER_H

#include <map>
#include <string>
#include <list>

#include "parameter.h"

class ItransportServerObserver
{

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
protected:
    std::list<ItransportServerObserver*> m_observers;
    parameterMap m_defaultParameters;
    parameterMap m_parameters;

};

#endif // ITRANSPORTSERVER_H
