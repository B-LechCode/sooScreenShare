#ifndef ISCREENSHOT_H
#define ISCREENSHOT_H
#include <opencv/cv.hpp>

class IscreenShot{
public:
    IscreenShot(int x, int y, uint w, uint h);
    virtual ~IscreenShot();

    virtual cv::Mat operator()() = 0;
protected:
        int m_x;
        int m_y;
        uint m_w;
        uint m_h;
};

#endif // ISCREENSHOT_H
