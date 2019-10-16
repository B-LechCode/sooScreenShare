#include "itransportclient.h"


ItransportClientObserver::ItransportClientObserver()
{

}

ItransportClientObserver::~ItransportClientObserver()
{

}

ItransportClient::ItransportClient():
    m_observer(),
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
    m_observer = &o;
}

void ItransportClient::removeObserverSubscriber()
{
    m_observer = nullptr;
}
