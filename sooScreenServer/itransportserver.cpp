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

ItransportServer::ItransportServer()
{

}

ItransportServer::~ItransportServer()
{

}

void ItransportServer::addObserverSubscriber(ItransportServerObserver &o)
{
    m_observer = &o;
}
