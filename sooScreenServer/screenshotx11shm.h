#ifndef SCREENSHOTX11SHM_H
#define SCREENSHOTX11SHM_H

#include <opencv2/stitching.hpp>
#include "iscreenshot.h"

#include <X11/Xlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <X11/extensions/XShm.h>
#include <X11/Xutil.h>
#include <X11/extensions/Xrandr.h>
//screenShotX11Shm

//From: https://stackoverflow.com/questions/24988164/c-fast-screenshots-in-linux-for-use-with-opencv
//https://stackoverflow.com/a/39781697
class screenShotX11Shm : public IscreenShot
{
    struct pointerStr{
        int32_t xStart;
        int32_t xEnd;
        int32_t yStart;
        int32_t yEnd;
    };

    struct RGBA{
        uint8_t b;
        uint8_t g;
        uint8_t r;
        uint8_t a;
    };

    bool checkCoordinates(int32_t xR,int32_t yR)
    {
        bool xO = xR>=m_x && xR<=(m_x+m_w);
        bool yO = xR>=m_x && xR<=(m_x+m_w);
        return xO&yO;
    }

    pointerStr calculateCoordinates(int32_t xR,int32_t yR,int32_t wImg,int32_t hImg)
    {
        int rect = 5;
        pointerStr pointer;
        pointer.xStart = xR-m_x-rect;
        pointer.xEnd = xR-m_x+rect;
        pointer.yStart = yR-m_y-rect;
        pointer.yEnd = yR-m_y+rect;

        pointer.xStart = pointer.xStart < 0 ? 0 : pointer.xStart;
        pointer.xEnd = pointer.xEnd > wImg ? wImg : pointer.xEnd;
        pointer.yStart = pointer.yStart < 0 ? 0 : pointer.yStart;
        pointer.yEnd = pointer.yEnd > hImg ? hImg : pointer.yEnd;

        return  pointer;
    }


public:
    screenShotX11Shm():IscreenShot()
    {
        imageGrabbed = false;
        init = false;
    }

    virtual cv::Mat operator() ()
    {

        imageGrabbed = true;

        XShmGetImage(display, root, ximg, m_x, m_y, 0x00ffffff);

        cv::Mat img = cv::Mat(m_h, m_w, CV_8UC4, ximg->data);

        Window win;
        int32_t xR,yR,xW,yW;
        uint32_t sad;
        XQueryPointer(display,root,&win,&win,&xR,&yR,&xW,&yW,&sad);

        if(checkCoordinates(xR,yR))
        {
            auto coord = calculateCoordinates(xR,yR,img.cols,img.rows);
            for(int y = coord.yStart ; y < coord.yEnd;++y )
            {
                RGBA* ptrRgba = reinterpret_cast<RGBA*>(img.ptr(y))+coord.xStart;
                for(int x = coord.xStart ; x < coord.xEnd;++x )
                {
                    ptrRgba->r = ~(ptrRgba->r);
                    ptrRgba->g = ~(ptrRgba->g);
                    ptrRgba->b = ~(ptrRgba->b);
                    ptrRgba++;
                }
            }
        }

        return img;
    }

    virtual void initialize(int32_t x,int32_t y, uint32_t w,uint32_t h)
    {
        m_x = x;
        m_y = y;
        m_w = w;
        m_h = h;
        if(init)
            destruct();

        display = XOpenDisplay(nullptr);
        root = DefaultRootWindow(display);

        XGetWindowAttributes(display, root, &window_attributes);
        screen = window_attributes.screen;
        auto visual = DefaultVisualOfScreen(screen);

        ximg = XShmCreateImage(display, visual, DefaultDepthOfScreen(screen),  ZPixmap, nullptr, &shminfo, m_w, m_h);

        shminfo.shmid = shmget(IPC_PRIVATE, ximg->bytes_per_line * ximg->height, IPC_CREAT|0777);
        shminfo.shmaddr = ximg->data = (char*)shmat(shminfo.shmid, 0, 0);
        shminfo.readOnly = False;
        if(shminfo.shmid < 0)
            puts("Fatal shminfo error!");;
        Status s1 = XShmAttach(display, &shminfo);
        printf("XShmAttach() %s\n", s1 ? "success!" : "failure!");

        imageGrabbed = false;
        init = true;
    }


    virtual ~screenShotX11Shm()
    {
       destruct();
    }

    virtual std::vector<screenDef> getScreens()
    {
        std::vector<screenDef> ret;
        auto display = XOpenDisplay(nullptr);
        int nsizes=0;

        auto monitors = XRRGetMonitors(display, XRootWindow(display, 0) , true, &nsizes);
        auto resources = XRRGetScreenResources(display,XRootWindow(display, 0));
        for (int i = 0;i<nsizes;i++)
        {
            auto opInfo = XRRGetOutputInfo (display, resources,monitors->outputs[i]);
            auto crtcInfo = XRRGetCrtcInfo(display,resources,opInfo->crtc);
            ret.push_back(screenDef(crtcInfo->x,crtcInfo->y,crtcInfo->width,crtcInfo->height));

            XRRFreeOutputInfo(opInfo);
            XRRFreeCrtcInfo(crtcInfo);
        }

        XRRFreeMonitors(monitors);
        XRRFreeScreenResources(resources);

        XCloseDisplay(display);


        return ret;
    }
private:
    void destruct()
    {
        if(imageGrabbed)
            XDestroyImage(ximg);
        init = false;
        XShmDetach(display, &shminfo);
        shmdt(shminfo.shmaddr);
        XCloseDisplay(display);
    }


    Display* display;
    Window root;
    XWindowAttributes window_attributes;
    Screen* screen;
    XImage* ximg;
    XShmSegmentInfo shminfo;



    bool imageGrabbed,init;
};

#endif // SCREENSHOTX11SHM_H
