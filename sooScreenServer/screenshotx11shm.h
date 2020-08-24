//sooScreenShare by Simon Wezstein (B-LechCode), 2019
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */
#ifndef SCREENSHOTX11SHM_H
#define SCREENSHOTX11SHM_H
#ifdef WITH_X11

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



//From: https://stackoverflow.com/questions/24988164/c-fast-screenshots-in-linux-for-use-with-opencv
//https://stackoverflow.com/a/39781697
/**
 * @brief The X11 screenshot backend
 *
 */
class screenShotX11Shm : public IscreenShot
{
    /**
     * @brief The data struct for the mouse pointer drawing coordinates
     *
     */
    struct pointerStr{
        int32_t xStart; /**< x start value for drawing in the destination image */
        int32_t xStartOffset; /**< x start offset value in the mouse pointer image  */
        int32_t xEnd; /**< x end value for drawing in the destination image */
        int32_t xEndOffset; /**< x end offset value in the mouse pointer image  */
        int32_t yStart; /**< y start value for drawing in the destination image */
        int32_t yStartOffset; /**< y start offset value in the mouse pointer image  */
        int32_t yEnd; /**< y end value for drawing in the destination image */
        int32_t yEndOffset; /**< y end offset value in the mouse pointer image  */
    };

    /**
     * @brief BGRA struct for fast pixel access
     *
     */
    struct BGRA{
        uint8_t b; /**< The blue component */
        uint8_t g; /**< The green component */
        uint8_t r; /**< The red component */
        uint8_t a; /**< The alpha channel */
    };

    Display* m_display; /**< The X11 display */
    Window m_root; /**< The X11 root window */
    XWindowAttributes m_windowAttributes; /**< The X11 window attributes */
    Screen* m_screen; /**< The X11 screen */
    XImage* m_ximg; /**< The X11 image */
    XShmSegmentInfo m_shminfo; /**< The X11 shared memory info */



    bool m_imageGrabbed; /**< Image grabbed flag, is true if an image was grabbed after init */
    bool m_init; /**< The initialized flat, is true if the init was successful */


    /**
     * @brief The changed event of the underlying parameter map
     *
     */
    virtual void parameterMapChangedEvent()
    {
        initialize();
    }
    /**
     * @brief The changed event of a key/value pair
     *
     * @param key The key of the changed parameter
     */
    virtual void parameterChangedEvent(const std::string& key)
    {
            initialize();
    }

    /**
     * @brief Converts the parameters in the parameter map to the corresponding member variables
     *
     */
    void applyParameters()
    {

        bool override = m_parameters[OVERRIDE_DISPLAY_SELECTION].valueBool();
        bool ok;


        int parI;

        if(!override)
        {
            parI = m_parameters[DISPLAY_NUMBER].valueInt(ok);if(!ok) return;
            int displayNumber = parI;
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
            parI = m_parameters[Grab_X].valueInt(ok);if(!ok) return;
            m_x = parI;
            parI = m_parameters[Grab_Y].valueInt(ok);if(!ok) return;
            m_y = parI;
            parI = m_parameters[Grab_H].valueInt(ok);if(!ok) return;
            m_h = static_cast<uint32_t>(parI);
            parI = m_parameters[Grab_W].valueInt(ok);if(!ok) return;
            m_w = static_cast<uint32_t>(parI);
        }


    }


    /**
     * @brief Helper function to check for illegal coordinates
     * This function checks against lower than zero and higher than the limit values
     * @param c Reference of the value to check
     * @param difference Reference of the difference to calculate
     * @param lim The positive limit
     */
    void checkCoordinateRange(int& c,int& difference,int lim)
    {
        difference = 0;
        if(c>lim)
        {
            difference = c-lim;
            c = lim;
            return;
        }

        if(c<0)
        {
            difference = -c;
            c = 0;
            return;
        }        
    }

    /**
     * @brief Calculates the draw coordinates for the mouse pointer
     *
     * @param xR Mouse pointer position x
     * @param yR Mouse pointer position y
     * @param wImg Width of the destination image
     * @param hImg Height of the destination image
     * @return pointerStr The coordinate structure
     */
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
    uint8_t* m_cursBuff = nullptr; /**< TODO: describe */
    size_t   m_cursBuffSize = 0; /**< TODO: describe */
    cv::Mat  m_cursImage; /**< TODO: describe */
    int m_hotX; /**< TODO: describe */
    int m_hotY; /**< TODO: describe */
    std::string m_cursorName; /**< TODO: describe */

    /**
     * @brief Grabs the mouse pointer image
     *
     */
    void grabMousePtr()
    {
        XFixesCursorImage * ptrCursImg = XFixesGetCursorImage (m_display);

        if(!ptrCursImg || !ptrCursImg->pixels)
            return;


        auto a = sizeof(*ptrCursImg->pixels);
        size_t mySize = ptrCursImg->width*ptrCursImg->height*a;
        std::string myName(ptrCursImg->name);
        bool compare = false;        

        //compare buffer
        if(mySize==m_cursBuffSize && m_cursBuff && !myName.compare(m_cursorName))
        {
            compare = !memcmp(ptrCursImg->pixels,m_cursBuff,mySize);
        }

        if(!compare)
        {
            if(m_cursBuff)
                delete[] m_cursBuff;

            m_cursBuff = new uint8_t[mySize];

            if(!m_cursBuff)
                return;

            m_hotX = ptrCursImg->xhot;
            m_hotY = ptrCursImg->yhot;

            memcpy(m_cursBuff,ptrCursImg->pixels,mySize);
            m_cursBuffSize = mySize;
            m_cursorName = myName;

            m_cursImage = cv::Mat(ptrCursImg->height,ptrCursImg->width,CV_8UC4);

            BGRA* ptrRgba = reinterpret_cast<BGRA*>(m_cursImage.ptr());
            auto ptrSrc = reinterpret_cast<uint8_t*>(ptrCursImg->pixels);

            for(int y = 0 ; y < ptrCursImg->height;++y )
            {
                for(int x = 0 ; x < ptrCursImg->width;++x )
                {
                    (*ptrRgba) = (*reinterpret_cast<BGRA*>(ptrSrc));
                    ptrSrc+=a;
                    ptrRgba++;

                }
            }            
        }        
    }

    /**
     * @brief Cleans up
     *
     */
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
    /**
     * @brief The standard constructor
     *
     */
    screenShotX11Shm():IscreenShot(),m_imageGrabbed(false),m_init(false)
    {
        m_defaultParameters[OVERRIDE_DISPLAY_SELECTION] = parameter("Override display selection to freely define the grab ROI","bool","false");
        m_defaultParameters[DISPLAY_NUMBER] = parameter("The display to grab","int16","0");        
        m_defaultParameters[Grab_X] = parameter("The X coordinate of the grab ROI","int32","0");
        m_defaultParameters[Grab_Y] = parameter("The Y coordinate of the grab ROI","int32","0");
        m_defaultParameters[Grab_W] = parameter("The width of the grab ROI","uint32","640");
        m_defaultParameters[Grab_H] = parameter("The height of the grab ROI","uint32","480");
        setParameters(m_defaultParameters);      
    }

    /**
     * @brief The grab operator
     * This operator grabs the screen with mouse pointer
     * @return cv::Mat The grabbed image
     */
    virtual cv::Mat operator() ()
    {       
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
            BGRA* ptrCurs = reinterpret_cast<BGRA*>(m_cursImage.ptr())+coord.yStartOffset*m_cursImage.cols;

            for(int y = coord.yStart ; y < coord.yEnd;++y )
            {
                BGRA* ptrRgba = reinterpret_cast<BGRA*>(img.ptr(y))+coord.xStart;
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

        return img;
    }


    /**
     * @brief The initializer method
     * Allocates memory and opens the display for image grabbing
     */
    virtual void initialize()
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


    /**
     * @brief The destructor
     *
     */
    virtual ~screenShotX11Shm()
    {
       destruct();
    }

    /**
    * @brief The currently active screens
    *
    * @return std::vector<screenDef> The screens
    */
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
