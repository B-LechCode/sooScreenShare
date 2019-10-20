//SooScreenServer by Simon Wezstein (B-LechCode), 2019
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */
#include "lz4imagedecompressor.h"


lz4ImageDecompressor::lz4ImageDecompressor():IImageDecompressor()
{

}

lz4ImageDecompressor::~lz4ImageDecompressor()
{

}

void lz4ImageDecompressor::setParameters(parameterMap &para)
{
    IImageDecompressor::setParameters(para);
}

void lz4ImageDecompressor::parameterMapChangedEvent()
{

}

void lz4ImageDecompressor::parameterChangedEvent(const std::string &key)
{

}
