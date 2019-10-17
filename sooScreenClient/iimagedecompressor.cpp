//SooScreenServer by Simon Wezstein (B-LechCode), 2019
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */
#include "iimagedecompressor.h"



IImageDecompressor::IImageDecompressor():
    m_defaultParameters(),
    m_parameters(m_defaultParameters)
{

}

IImageDecompressor::~IImageDecompressor()
{

}

parameterMap IImageDecompressor::getDefaultParameters()
{
    return m_defaultParameters;
}

parameterMap IImageDecompressor::getParameters()
{
    return m_parameters;
}

void IImageDecompressor::setParameters(parameterMap &para)
{
    parameterMap::insertParameters(m_parameters,para);
}
