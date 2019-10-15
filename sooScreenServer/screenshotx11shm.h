#ifndef SCREENSHOTX11SHM_H
#define SCREENSHOTX11SHM_H

#include <opencv2/stitching.hpp>
#include "iscreenshot.h"

#include <X11/Xlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <X11/extensions/XShm.h>
#include <X11/Xutil.h>
//screenShotX11Shm

//From: https://stackoverflow.com/questions/24988164/c-fast-screenshots-in-linux-for-use-with-opencv
//https://stackoverflow.com/a/39781697
struct screenShotX11Shm : public IscreenShot
{
    screenShotX11Shm(int x, int y, uint w, uint h):IscreenShot(x,y,w,h)
    {
        display = XOpenDisplay(nullptr);
        root = DefaultRootWindow(display);

        auto lol =  getScreens();

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

        init = true;
    }

    virtual cv::Mat operator() (){
        if(init)
            init = false;

        XShmGetImage(display, root, ximg, m_x, m_y, 0x00ffffff);

        cv::Mat img = cv::Mat(m_h, m_w, CV_8UC4, ximg->data);

        return img;
    }

    virtual ~screenShotX11Shm(){
        if(!init)
            XDestroyImage(ximg);

        XShmDetach(display, &shminfo);
        shmdt(shminfo.shmaddr);
        XCloseDisplay(display);
    }
    virtual std::vector<screenDef> getScreens()
    {
        std::vector<screenDef> ret;
        auto display = XOpenDisplay(nullptr);

        //From:https://stackoverflow.com/questions/11367354/obtaining-list-of-all-xorg-displays
        //https://stackoverflow.com/a/11368019
        int32_t count = XScreenCount(display);
                                printf("Display %s has %d screens\n",
                                    "display_name", count);

        int32_t i;
        for (i=0; i<count; i++)
        {
            printf(" %d: %dx%d\n",
                i, XDisplayWidth(display, i), XDisplayHeight(display, i));


            auto window = XRootWindow(display, i);

            int32_t x,y;
            uint32_t w,h,bw,dpt;
            XGetGeometry(display,(Window)window,(Window*)&window,&x,&y,&w,&h,&bw,&dpt);
            screenDef scr(x,y,w,h);
            ret.push_back(scr);
        }
        XCloseDisplay(display);
        //
        return ret;
    }
private:
    Display* display;
    Window root;
    XWindowAttributes window_attributes;
    Screen* screen;
    XImage* ximg;
    XShmSegmentInfo shminfo;



    bool init;
};

#endif // SCREENSHOTX11SHM_H
