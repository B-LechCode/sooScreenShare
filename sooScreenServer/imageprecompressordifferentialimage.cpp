#include "imageprecompressordifferentialimage.h"

ImagePreCompressorDifferentialImage::ImagePreCompressorDifferentialImage():
    m_FirstFrame(true),
    m_numDifferentialFrames(0),
    m_maxNumOfDifferentialFrames(100) //TODO: Parameter!

{

}

ImagePreCompressorDifferentialImage::~ImagePreCompressorDifferentialImage()
{

}

cv::Mat ImagePreCompressorDifferentialImage::compress(cv::Mat &img, imageType &type, bool &ok)
{
    ok = true;
    if(!m_FirstFrame && m_numDifferentialFrames<m_maxNumOfDifferentialFrames && img.size == m_diffImage.size)
    {
        uint32_t* ptrDiffImg = reinterpret_cast<uint32_t*>(m_diffImage.ptr(0));
        uint32_t* ptrKeyImg = reinterpret_cast<uint32_t*>(m_keyImage.ptr(0));
        uint32_t* ptrImg = reinterpret_cast<uint32_t*>(img.ptr(0));

        for (int i=0;i<img.rows*img.cols ;++i)
        {
            (*ptrDiffImg) = (*ptrImg)-(*ptrKeyImg);

            ++ptrDiffImg;
            ++ptrImg;
            ++ptrKeyImg;
        }

       type = imageType::incrementalFrame;
       m_numDifferentialFrames++;


    }
    else
    {
        std::cout << "Sending key frame " << std::endl;
        m_keyImage = img.clone();
        m_diffImage = img;
        type = imageType::keyFrame;
        m_numDifferentialFrames = 0;
        m_FirstFrame = false;

    }
    return m_diffImage;
}

void ImagePreCompressorDifferentialImage::parameterMapChangedEvent()
{

}

void ImagePreCompressorDifferentialImage::parameterChangedEvent(const std::string &key)
{

}
