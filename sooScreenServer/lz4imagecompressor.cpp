#include "lz4imagecompressor.h"


lz4ImageCompressor::lz4ImageCompressor():IImageCompressor()
{    
    setParameters(m_defaultParameters);
}

lz4ImageCompressor::~lz4ImageCompressor()
{

}

void lz4ImageCompressor::setParameters(parameterMap &para)
{
    IImageCompressor::setParameters(para);

}
