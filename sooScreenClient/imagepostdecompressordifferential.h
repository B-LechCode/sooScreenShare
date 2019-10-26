#ifndef IMAGEPOSTDECOMPRESSORDIFFERENTIAL_H
#define IMAGEPOSTDECOMPRESSORDIFFERENTIAL_H

#include "iimagepostdecompressor.h"

class ImagePostDecompressorDifferential : public IImagePostDecompressor
{
public:
    ImagePostDecompressorDifferential();
    virtual ~ImagePostDecompressorDifferential();
    /**
     * @brief The method for image decomression.
     *
     * @param data  The Data
     * @param hdr    Header of the received data
     * @param ok     Return reference for decompression status (True if ok)
     * @return cv::Mat The decompressed image
     */
    virtual cv::Mat decompress(cv::Mat& data,dataHeaderHandling::dataHeader hdr, bool& ok)
    {
        ok = false;

        if(data.channels() == 3) //We need to ensure, we have 32 bits of data per pixel
            cv::cvtColor(data,data,cv::COLOR_BGR2BGRA);

        if(hdr.imageDataType == imageType::keyFrame)
        {
            m_keyImage =  data.clone();
            m_diffImage = data.clone();
            ok = true;
        }
        else if (data.size == m_diffImage.size && data.size == m_keyImage.size)
        {
            uint32_t* ptrDiffImg = reinterpret_cast<uint32_t*>(m_diffImage.ptr(0));
            uint32_t* ptrKeyImg = reinterpret_cast<uint32_t*>(m_keyImage.ptr(0));
            uint32_t* ptrImg = reinterpret_cast<uint32_t*>(data.ptr(0));

            for (int i=0;i<data.rows*data.cols ;++i)
            {
                (*ptrDiffImg) = (*ptrImg)+(*ptrDiffImg);

                ++ptrDiffImg;
                ++ptrImg;
                ++ptrKeyImg;
            }

            ok = true;
        }

        return  m_diffImage;
    }
    /**
     * @brief The method for image decomression.
     *
     * @param data  The Data
     * @param hdr    Header of the received data
     * @return cv::Mat The decompressed image
     */
    virtual cv::Mat decompress(cv::Mat& data,dataHeaderHandling::dataHeader hdr)
    {
        bool ok;
        return decompress(data,hdr,ok);
    }
private:
    /**
     * @brief The changed event of the underlying parameter map
     *
     */
    virtual void parameterMapChangedEvent();
    /**
     * @brief The changed event of a key/value pair
     *
     * @param key The key of the changed parameter
     */
    virtual void parameterChangedEvent(const std::string& key);
    cv::Mat m_keyImage;
    cv::Mat m_diffImage;
};
#endif // IMAGEPOSTDECOMPRESSORDIFFERENTIAL_H
