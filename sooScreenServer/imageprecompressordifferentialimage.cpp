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
        uint32_t* ptrImg = reinterpret_cast<uint32_t*>(img.ptr(0));
        uint32_t* ptrOldImg = reinterpret_cast<uint32_t*>(m_oldImage.ptr(0));

        uint32_t* ptrImgEnd = ptrImg+img.rows*img.cols;


        while(ptrImg != ptrImgEnd)
        {
            (*ptrDiffImg) = (*ptrImg)-(*ptrOldImg);
            (*ptrOldImg) = (*ptrImg);

            ++ptrDiffImg;
            ++ptrImg;
            ++ptrOldImg;
        }

       type = imageType::incrementalFrame;
       m_numDifferentialFrames++;

       return m_diffImage;
    }
    else
    {      
        m_oldImage = img.clone();
        m_diffImage = cv::Mat(img.rows,img.cols,CV_8UC4,cv::Scalar(0));
        type = imageType::keyFrame;
        m_numDifferentialFrames = 0;
        m_FirstFrame = false;

    }
    return m_oldImage;

}

void ImagePreCompressorDifferentialImage::parameterMapChangedEvent()
{

}

void ImagePreCompressorDifferentialImage::parameterChangedEvent(const std::string &)
{

}
