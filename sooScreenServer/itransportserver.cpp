//SooScreenServer by Simon Wezstein (B-LechCode), 2019
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */
#include "itransportserver.h"


ItransportServerObserver::ItransportServerObserver()
{

}

ItransportServerObserver::~ItransportServerObserver()
{

}

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
