#ifndef SCREENSHOTFACTORY_H
#define SCREENSHOTFACTORY_H

#include <vector>
#include <string>

#include "iscreenshot.h"

#include "screenshotx11shm.h"

#define x11 "X11_Shm_Grab"



class screenShotFactory
{
    static std::vector<std::string> m_backends;
public:
    static const std::vector<std::string>& getAvailableBackends()
    {
        if(!m_backends.size())
        {
            m_backends.push_back(x11);
        }
        return  m_backends;
    }

    static IscreenShot* getBackend(std::string backendName)
    {
        if(backendName == x11)
            return new screenShotX11Shm();

        return nullptr;
    }
};

#endif // SCREENSHOTFACTORY_H
