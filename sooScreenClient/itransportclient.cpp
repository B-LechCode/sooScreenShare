//SooScreenServer by Simon Wezstein (B-LechCode), 2019
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */
#include "itransportclient.h"


ItransportClientObserver::ItransportClientObserver()
{

}

ItransportClientObserver::~ItransportClientObserver()
{

}

ItransportClient::ItransportClient():
    m_observer()
{

}

ItransportClient::~ItransportClient()
{

}

ItransportClientObserver *ItransportClient::observer() const
{
    return  m_observer;
}

void ItransportClient::setObserver(ItransportClientObserver* observer)
{
    m_observer = observer;
}
