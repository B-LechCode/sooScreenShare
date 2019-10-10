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
    m_parameters = para;
}
