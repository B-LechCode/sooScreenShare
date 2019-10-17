//SooScreenServer by Simon Wezstein (B-LechCode), 2019
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */
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
#include <X11/extensions/Xfixes.h>

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

    Display* m_display;
    Window m_root;
    XWindowAttributes m_windowAttributes;
    Screen* m_screen;
    XImage* m_ximg;
    XShmSegmentInfo m_shminfo;



    bool m_imageGrabbed,m_init;

    bool checkCoordinates(int32_t xR,int32_t yR)
    {
        bool xO = xR>=m_x && xR<=(m_x+m_w);
        bool yO = xR>=m_x && xR<=(m_x+m_w);
        return xO&yO;
    }

    pointerStr calculateCoordinates(int32_t xR,int32_t yR,int32_t wImg,int32_t hImg)
    {        
        int wX = m_cursImage.cols;
        int wY = m_cursImage.rows;
        std::cout << m_hotX << " " << m_hotY << std::endl;
        pointerStr pointer;
        pointer.xStart = xR-m_x-m_hotX;
        pointer.xEnd = pointer.xStart+wX;
        pointer.yStart = yR-m_y-m_hotY;
        pointer.yEnd = pointer.yStart+wY;

        pointer.xStart = pointer.xStart < 0 ? 0 : pointer.xStart;
        pointer.xEnd = pointer.xEnd > wImg ? wImg : pointer.xEnd;
        pointer.yStart = pointer.yStart < 0 ? 0 : pointer.yStart;
        pointer.yEnd = pointer.yEnd > hImg ? hImg : pointer.yEnd;

        return  pointer;
    }
    uint8_t* m_cursBuff = nullptr;
    size_t   m_cursBuffSize = 0;
    cv::Mat  m_cursImage;
    int m_hotX;
    int m_hotY;
    std::string m_cursorName;

    void grabMousePtr()
    {
        XFixesCursorImage * asd = XFixesGetCursorImage (m_display);

        if(!asd->pixels)
            return;


        auto a = sizeof(*asd->pixels);
        size_t mySize = asd->width*asd->height*a;
        std::string myName(asd->name);
        bool compare = false;



        //compare buffer
        if(mySize==m_cursBuffSize && m_cursBuff && !myName.compare(m_cursorName))
        {
            compare = !memcmp(asd->pixels,m_cursBuff,mySize);
        }

        if(!compare)
        {
            if(m_cursBuff)
                delete[] m_cursBuff;

            m_cursBuff = new uint8_t[mySize];

            if(!m_cursBuff)
                return;

            m_hotX = asd->xhot;
            m_hotY = asd->yhot;

            memcpy(m_cursBuff,asd->pixels,mySize);
            m_cursBuffSize = mySize;
            m_cursorName = myName;

            m_cursImage = cv::Mat(asd->width,asd->height,CV_8UC4);

            RGBA* ptrRgba = reinterpret_cast<RGBA*>(m_cursImage.ptr());
            auto ptrSrc = reinterpret_cast<uint8_t*>(asd->pixels);

            for(int y = 0 ; y < asd->height;++y )
            {
                for(int x = 0 ; x < asd->width;++x )
                {
                    (*ptrRgba) = (*reinterpret_cast<RGBA*>(ptrSrc));
                    ptrSrc+=a;
                    ptrRgba++;

                }
            }
        }
    }

    void destruct()
    {
        if(m_imageGrabbed)
            XDestroyImage(m_ximg);
        m_init = false;
        XShmDetach(m_display, &m_shminfo);
        shmdt(m_shminfo.shmaddr);
        XCloseDisplay(m_display);
    }


public:
    screenShotX11Shm():IscreenShot()
    {
        m_imageGrabbed = false;
        m_init = false;
    }

    virtual cv::Mat operator() ()
    {

        m_imageGrabbed = true;

        XShmGetImage(m_display, m_root, m_ximg, m_x, m_y, 0x00ffffff);

        cv::Mat img = cv::Mat(m_h, m_w, CV_8UC4, m_ximg->data);

        Window win;
        int32_t xR,yR,xW,yW;
        uint32_t sad;
        XQueryPointer(m_display,m_root,&win,&win,&xR,&yR,&xW,&yW,&sad);

        if(checkCoordinates(xR,yR))
        {
            grabMousePtr();
            auto coord = calculateCoordinates(xR,yR,img.cols,img.rows);


            RGBA* ptrCurs = reinterpret_cast<RGBA*>(m_cursImage.ptr());
            for(int y = coord.yStart ; y < coord.yEnd;++y )
            {
                RGBA* ptrRgba = reinterpret_cast<RGBA*>(img.ptr(y))+coord.xStart;

                for(int x = coord.xStart ; x < coord.xEnd;++x )
                {
                    double alpha = (ptrCurs->a)/255.0;

                    ptrRgba->r =  ((ptrCurs->r)*alpha+ptrRgba->r)*(1-alpha);
                    ptrRgba->g = ((ptrCurs->g)*alpha+ptrRgba->g)*(1-alpha);
                    ptrRgba->b = ((ptrCurs->b)*alpha+ptrRgba->b)*(1-alpha);
                    ptrRgba++;
                    ptrCurs++;
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
        if(m_init)
            destruct();

        m_display = XOpenDisplay(nullptr);
        m_root = DefaultRootWindow(m_display);

        XGetWindowAttributes(m_display, m_root, &m_windowAttributes);
        m_screen = m_windowAttributes.screen;
        auto visual = DefaultVisualOfScreen(m_screen);

        m_ximg = XShmCreateImage(m_display, visual, DefaultDepthOfScreen(m_screen),  ZPixmap, nullptr, &m_shminfo, m_w, m_h);

        m_shminfo.shmid = shmget(IPC_PRIVATE, m_ximg->bytes_per_line * m_ximg->height, IPC_CREAT|0777);
        m_shminfo.shmaddr = m_ximg->data = (char*)shmat(m_shminfo.shmid, 0, 0);
        m_shminfo.readOnly = False;
        if(m_shminfo.shmid < 0)
            puts("Fatal shminfo error!");;
        Status s1 = XShmAttach(m_display, &m_shminfo);
        printf("XShmAttach() %s\n", s1 ? "success!" : "failure!");

        m_imageGrabbed = false;
        m_init = true;
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
};

#endif // SCREENSHOTX11SHM_H
