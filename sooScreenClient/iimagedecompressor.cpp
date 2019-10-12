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
