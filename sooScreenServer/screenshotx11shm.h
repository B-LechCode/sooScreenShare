//SooScreenServer by Simon Wezstein (B-LechCode), 2019
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */
#ifndef SCREENSHOTX11SHM_H
#define SCREENSHOTX11SHM_H
#if WITH_X11

namespace shot {
#define DISPLAY_NUMBER "Display Number"
#define OVERRIDE_DISPLAY_SELECTION "Override Display Selection"
#define Grab_X "X"
#define Grab_Y "Y"
#define Grab_W "Width"
#define Grab_H "Height"
}

#include <opencv2/stitching.hpp>
#include "iscreenshot.h"

#include <X11/Xlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <X11/extensions/XShm.h>
#include <X11/Xutil.h>
#include <X11/extensions/Xrandr.h>
#include <X11/extensions/Xfixes.h>
#include <iostream>
#include <cctype>
#include <algorithm>

//screenShotX11Shm

//From: https://stackoverflow.com/questions/24988164/c-fast-screenshots-in-linux-for-use-with-opencv
//https://stackoverflow.com/a/39781697
class screenShotX11Shm : public IscreenShot
{
    struct pointerStr{
        int32_t xStart;
        int32_t xStartOffset;
        int32_t xEnd;
        int32_t xEndOffset;
        int32_t yStart;
        int32_t yStartOffset;
        int32_t yEnd;
        int32_t yEndOffset;
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



    bool m_imageGrabbed,m_init,grabbing;

    virtual void parameterMapChangedEvent()
    {
        initialize();
    }

    void applyParameters()
    {
        std::string overrideStr = m_parameters[OVERRIDE_DISPLAY_SELECTION].value();
        std::transform(overrideStr.begin(),overrideStr.end(),overrideStr.begin(),[](int c) -> int { return std::tolower(c); });
        bool override = overrideStr == "true";



        if(!override)
        {
            int displayNumber = std::stoi(m_parameters[DISPLAY_NUMBER].value());
            auto displays = getScreens();

            if(displays.size()-1<displayNumber)
                ;//FAIL!
            else
            {
                auto displayNumberS = static_cast<size_t>(displayNumber);
                m_x = displays[displayNumberS].x;
                m_y = displays[displayNumberS].y;
                m_h = displays[displayNumberS].h;
                m_w = displays[displayNumberS].w;
            }
        }
        else
        {
            m_x = std::stoi(m_parameters[Grab_X].value());
            m_y = std::stoi(m_parameters[Grab_Y].value());
            m_h = static_cast<uint32_t>(std::stoi(m_parameters[Grab_H].value()));
            m_w = static_cast<uint32_t>(std::stoi(m_parameters[Grab_W].value()));
        }


    }

    virtual void parameterChangedEvent(const std::string& key)
    {
        if(grabbing)
            std::cout << "ERROR!" << std::endl;
        else
            initialize();
    }

    bool checkCoordinates(int32_t xR,int32_t yR)
    {
        bool xO = xR>=m_x && xR<(m_x+m_w);
        bool yO = yR>=m_y && yR<(m_y+m_h);
        return xO&&yO;
    }

    bool checkCoordinateRange(int& c,int& difference,int lim)
    {
        difference = 0;
        if(c>lim)
        {
            difference = c-lim;
            c = lim;
            return true;
        }

        if(c<0)
        {
            difference = -c;
            c = 0;
            return true;
        }

        return false;
    }

    pointerStr calculateCoordinates(int32_t xR,int32_t yR,int32_t wImg,int32_t hImg)
    {        
        int wX = m_cursImage.cols;
        int wY = m_cursImage.rows;

        pointerStr pointer;
        pointer.xStart = xR-m_x-m_hotX;
        pointer.xEnd = pointer.xStart+wX;
        pointer.yStart = yR-m_y-m_hotY;
        pointer.yEnd = pointer.yStart+wY;

        int xLim = wImg;
        int yLim = hImg;

        checkCoordinateRange(pointer.xStart,pointer.xStartOffset,xLim);
        checkCoordinateRange(pointer.xEnd,pointer.xEndOffset,xLim);
        checkCoordinateRange(pointer.yStart,pointer.yStartOffset,yLim);
        checkCoordinateRange(pointer.yEnd,pointer.yEndOffset,yLim);

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
        if(m_cursBuff)
            delete[] m_cursBuff;
        m_cursBuff = nullptr;

        m_init = false;
        XShmDetach(m_display, &m_shminfo);
        shmdt(m_shminfo.shmaddr);
        XCloseDisplay(m_display);
    }


public:
    screenShotX11Shm():IscreenShot(),m_imageGrabbed(false),m_init(false)
    {
        m_defaultParameters[OVERRIDE_DISPLAY_SELECTION] = parameter("Override display selection to freely define the grab ROI","bool","false");
        m_defaultParameters[DISPLAY_NUMBER] = parameter("The display to grab","int16","0");        
        m_defaultParameters[Grab_X] = parameter("The X coordinate of the grab ROI","int32","0");
        m_defaultParameters[Grab_Y] = parameter("The Y coordinate of the grab ROI","int32","0");
        m_defaultParameters[Grab_W] = parameter("The width of the grab ROI","uint32","640");
        m_defaultParameters[Grab_H] = parameter("The width of the grab ROI","uint32","480");
        setParameters(m_defaultParameters);      
    }

    virtual cv::Mat operator() ()
    {
        grabbing = true;
        if(!m_init)
            return cv::Mat();

        m_imageGrabbed = true;

        XShmGetImage(m_display, m_root, m_ximg, m_x, m_y, 0x00ffffff);

        cv::Mat img = cv::Mat(m_h, m_w, CV_8UC4, m_ximg->data);

        Window win;
        int32_t xR,yR,xW,yW;
        uint32_t sad;
        XQueryPointer(m_display,m_root,&win,&win,&xR,&yR,&xW,&yW,&sad);
        grabMousePtr();
        auto coord = calculateCoordinates(xR,yR,img.cols,img.rows);

        if(coord.yEnd != coord.yStart && coord.xStart != coord.xEnd)
        {
            RGBA* ptrCurs = reinterpret_cast<RGBA*>(m_cursImage.ptr())+coord.yStartOffset*m_cursImage.cols;

            for(int y = coord.yStart ; y < coord.yEnd;++y )
            {
                RGBA* ptrRgba = reinterpret_cast<RGBA*>(img.ptr(y))+coord.xStart;
                ptrCurs += coord.xStartOffset;
                for(int x = coord.xStart ; x < coord.xEnd;++x )
                {
                    double alpha = (ptrCurs->a)/255.0;

                    ptrRgba->r = (ptrCurs->r)*alpha+ptrRgba->r*(1-alpha);
                    ptrRgba->g = (ptrCurs->g)*alpha+ptrRgba->g*(1-alpha);
                    ptrRgba->b = (ptrCurs->b)*alpha+ptrRgba->b*(1-alpha);
                    ptrRgba++;
                    ptrCurs++;
                }
                ptrCurs += coord.xEndOffset;
            }
        }
        grabbing = false;
        return img;
    }



    virtual void initialize(/*int32_t x,int32_t y, uint32_t w,uint32_t h*/)
    {
        applyParameters();
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
#endif
#endif // SCREENSHOTX11SHM_H
