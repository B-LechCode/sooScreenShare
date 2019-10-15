#ifndef ISCREENSHOT_H
#define ISCREENSHOT_H
#include <opencv/cv.hpp>
#include <vector>

struct screenDef{
    screenDef(){}
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
    IscreenShot(int32_t x, int32_t y, uint32_t w, uint32_t h);
    virtual ~IscreenShot();
    virtual std::vector<screenDef> getScreens()=0;
    virtual cv::Mat operator()() = 0;
protected:
        int32_t m_x;
        int32_t m_y;
        uint32_t m_w;
        uint32_t m_h;
};

#endif // ISCREENSHOT_H
