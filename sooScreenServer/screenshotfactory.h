//sooScreenShare by Simon Wezstein (B-LechCode), 2019
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

#ifdef WITH_X11
#define x11 "X11_Shm_Grab"
#endif
#ifdef WITH_WINAPI
#define backend_win "Windows_Grab"
#endif


/**
 * @brief  The factory class for the screenshot backends.
 *
 */
class screenShotFactory
{
    /**
    * @brief The standard constructor
    *
    */
    screenShotFactory(){}
    static std::vector<std::string> m_backends; /**< The vector of registred backends */
public:
    /**
     * @brief Function for listing the available backends
     * This function will return the available, registred Backends.
     * If you want to add a new backend, append a push_back call with the identification string of your new backend.
     * @return const std::vector<std::string> Available backends
     */
    static const std::vector<std::string>& getAvailableBackends()
    {
        if(!m_backends.size())
        {
#ifdef WITH_X11
            m_backends.push_back(x11);
#endif
#ifdef WITH_WINAPI
			m_backends.push_back(backend_win);
#endif
        }
        return  m_backends;
    }

    /**
  * @brief Creates a new backend object of the specified type
  * This function will return a new object of the specified type.
  * If a wrong backend name was specified nullptr is returned.
  * @param backendName The desired backend type specifier
  * @return IscreenShot The pointer to the created backend. Nullptr if invalid type specified
  */
    static IscreenShot* getBackend(std::string backendName)
    {
#if WITH_X11
        if(backendName == x11)
            return new screenShotX11Shm();
#endif
#ifdef WITH_WINAPI
        if (backendName == backend_win)
             return new screenShotWin();
#endif

        return nullptr;
    }
};

#endif // SCREENSHOTFACTORY_H
