#include "itransportserver.h"

ItransportServer::ItransportServer():
    m_observers(),
    m_defaultParameters(),
    m_parameters(m_defaultParameters)
{

}

ItransportServer::~ItransportServer()
{

}

parameterMap ItransportServer::getDefaultParameters()
{
    return m_defaultParameters;
}

void ItransportServer::setParameters(parameterMap &para)
{
    parameterMap::insertParameters(m_parameters,para);
}

parameterMap ItransportServer::getParameters()
{
    return  m_parameters;
}

void ItransportServer::addObserverSubscriber(ItransportServerObserver &o)
{
    m_observers.push_back(&o);
}

void ItransportServer::removeObserverSubscriber(ItransportServerObserver &o)
{
    m_observers.remove(&o);
}
