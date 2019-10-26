//sooScreenShare by Simon Wezstein (B-LechCode), 2019
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */
#include "iscreenshot.h"

IscreenShot::IscreenShot():Iparameter()
{
    m_x = m_y = m_w = m_h = 0;
}

IscreenShot::~IscreenShot()
{

}
