#ifndef ISCREENSHOT_H
#define ISCREENSHOT_H
#include <opencv2/opencv.hpp>
#include <vector>

struct screenDef{

    screenDef(int32_t x,int32_t y, uint32_t w,uint32_t h)
    {
        this->x = x;
        this->y = y;
        this->w = w;
        this->h = h;
    }
    int32_t x,y;
    uint32_t w,h;
};

class IscreenShot{
public:
    IscreenShot();
    virtual ~IscreenShot();
    virtual void initialize(int32_t x,int32_t y, uint32_t w,uint32_t h)=0;
    virtual std::vector<screenDef> getScreens()=0;
    virtual cv::Mat operator()() = 0;
protected:
        int32_t m_x;
        int32_t m_y;
        uint32_t m_w;
        uint32_t m_h;
};

#endif // ISCREENSHOT_H
