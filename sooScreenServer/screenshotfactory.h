//SooScreenServer by Simon Wezstein (B-LechCode), 2019
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */
#ifndef SCREENSHOTFACTORY_H
#define SCREENSHOTFACTORY_H

#include <vector>
#include <string>

#include "iscreenshot.h"

#include "screenshotx11shm.h"
#include "screenshotwin.h"

#if WITH_X11
#define x11 "X11_Shm_Grab"
#endif
#if WITH_WINAPI
#define backend_win "Windows_Grab"
#endif


class screenShotFactory
{
    screenShotFactory(){}
    static std::vector<std::string> m_backends;
public:
    static const std::vector<std::string>& getAvailableBackends()
    {
        if(!m_backends.size())
        {
#if WITH_X11
            m_backends.push_back(x11);
#endif
#if WITH_WINAPI
			m_backends.push_back(backend_win);
#endif
        }
        return  m_backends;
    }

    static IscreenShot* getBackend(std::string backendName)
    {
#if WITH_X11
        if(backendName == x11)
            return new screenShotX11Shm();
#endif
#if WITH_WINAPI
        if (backendName == backend_win)
             return new screenShotWin();
#endif

        return nullptr;
    }
};

#endif // SCREENSHOTFACTORY_H