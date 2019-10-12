#include "itransportclient.h"


ItransportClientObserver::ItransportClientObserver()
{

}

ItransportClientObserver::~ItransportClientObserver()
{

}

ItransportClient::ItransportClient():
    m_observers(),
    m_defaultParameters(),
    m_parameters(m_defaultParameters)
{

}

ItransportClient::~ItransportClient()
{

}

parameterMap ItransportClient::getDefaultParameters()
{
    return m_defaultParameters;
}

void ItransportClient::setParameters(parameterMap &para)
{
    parameterMap::insertParameters(m_parameters,para);
}

parameterMap ItransportClient::getParameters()
{
    return  m_parameters;
}

void ItransportClient::addObserverSubscriber(ItransportClientObserver &o)
{
    m_observers.push_back(&o);
}

void ItransportClient::removeObserverSubscriber(ItransportClientObserver &o)
{
    m_observers.remove(&o);
}
