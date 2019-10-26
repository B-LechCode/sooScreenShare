//sooScreenShare by Simon Wezstein (B-LechCode), 2019
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */
#include "lz4imagecompressor.h"


lz4ImageCompressor::lz4ImageCompressor():IImageCompressor()
{    
    setParameters(m_defaultParameters);
}

lz4ImageCompressor::~lz4ImageCompressor()
{

}

void lz4ImageCompressor::parameterMapChangedEvent()
{

}

void lz4ImageCompressor::parameterChangedEvent(const std::string &key)
{

}
