#include "imageprecompressordifferentialimage.h"

ImagePreCompressorDifferentialImage::ImagePreCompressorDifferentialImage():
    m_numDifferentialFrames(0),
    m_maxNumOfDifferentialFrames(10) //TODO: Parameter!
{

}

ImagePreCompressorDifferentialImage::~ImagePreCompressorDifferentialImage()
{

}

cv::Mat ImagePreCompressorDifferentialImage::compress(cv::Mat &img, imageType &type, bool &ok)
{
    ok = true;
    if(m_numDifferentialFrames<m_maxNumOfDifferentialFrames && img.size == m_diffImage.size)
    {
        int32_t* ptrDiffImg = reinterpret_cast<int32_t*>(m_diffImage.ptr(0));
        int32_t* ptrKeyImg = reinterpret_cast<int32_t*>(m_keyImage.ptr(0));
        int32_t* ptrImg = reinterpret_cast<int32_t*>(img.ptr(0));

        for (int i=0;i<img.rows*img.cols ;++i)
        {
            (*ptrDiffImg) = (*ptrImg)-(*ptrKeyImg);

            ++ptrDiffImg;
            ++ptrImg;
            ++ptrKeyImg;
        }

       type = imageType::incrementalFrame;
       m_numDifferentialFrames++;
       return m_diffImage;
    }
    else
    {   m_keyImage = img;
        m_diffImage = img;
        type = imageType::keyFrame;
        m_numDifferentialFrames = 0;
        return m_keyImage;
    }

}

void ImagePreCompressorDifferentialImage::parameterMapChangedEvent()
{

}

void ImagePreCompressorDifferentialImage::parameterChangedEvent(const std::string &key)
{

}
