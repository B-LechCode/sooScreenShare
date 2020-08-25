//sooScreenShare by Simon Wezstein (B-LechCode), 2019
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */
#include "nullimagecompressor.h"


nullImageCompressor::nullImageCompressor():IImageCompressor()
{   
    setParameters(m_defaultParameters);
}

nullImageCompressor::~nullImageCompressor()
{

}

void nullImageCompressor::initParameters()
{

}

void nullImageCompressor::parameterMapChangedEvent()
{
    initParameters();
}

void nullImageCompressor::parameterChangedEvent(const std::string &key)
{
    initParameters();
}
