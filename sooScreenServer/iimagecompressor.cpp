//SooScreenServer by Simon Wezstein (B-LechCode), 2019
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */
#include "iimagecompressor.h"



IImageCompressor::IImageCompressor():
    m_defaultParameters(),
    m_parameters(m_defaultParameters)
{

}

IImageCompressor::~IImageCompressor()
{

}

parameterMap IImageCompressor::getDefaultParameters()
{
    return m_defaultParameters;
}

parameterMap IImageCompressor::getParameters()
{
    return m_parameters;
}

void IImageCompressor::setParameters(parameterMap &para)
{
    parameterMap::insertParameters(m_parameters,para);
}
