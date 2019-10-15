#ifndef SCREENSHOTWIN_H
#define SCREENSHOTWIN_H

#include "iscreenshot.h"
#include <windows.h>

struct screenShotWin : public IscreenShot
{
	screenShotWin(int x, int y, uint w, uint h) : IscreenShot(x, y, w, h)
	{
		//GetWindowDC(GetDesktopWindow());
		screenDC = GetDC(NULL);
		memoryDC = CreateCompatibleDC(screenDC);

		hBitmap = CreateCompatibleBitmap(screenDC, w, h);

		m_x = x;
		m_y = y;
		m_w = w;
		m_h = h;

		lpPixels = new unsigned char[w * h * 4];
		
		init = true;
	}

	virtual cv::Mat operator() () {
		if (init)
			init = false;

		oldBmp = SelectObject(memoryDC, hBitmap);
		BitBlt(memoryDC, 0, 0, m_w, m_h, screenDC, m_x, m_y, SRCCOPY | CAPTUREBLT);
		SelectObject(memoryDC, oldBmp);

		BITMAPINFO bmInfo = { 0 };
		bmInfo.bmiHeader.biSize = sizeof(bmInfo.bmiHeader);
		GetDIBits(memoryDC, hBitmap, 0, 0, NULL, &bmInfo, DIB_RGB_COLORS);

		bmInfo.bmiHeader.biBitCount = 32;
		bmInfo.bmiHeader.biCompression = BI_RGB;
		bmInfo.bmiHeader.biHeight = -bmInfo.bmiHeader.biHeight; // negative height means "flip the image vertically"

		GetDIBits(memoryDC, hBitmap, 0, bmInfo.bmiHeader.biHeight, lpPixels, &bmInfo, DIB_RGB_COLORS);
		cv::Mat img = cv::Mat(m_h, m_w, CV_8UC4, lpPixels);

		return img;
	}

	virtual ~screenShotWin() {
		DeleteDC(memoryDC);
		DeleteDC(screenDC);

		if (lpPixels)
			delete[] lpPixels;
	}

	static BOOL CALLBACK ScreenInfoEnumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData)
	{
		std::vector<screenDef>* screens = reinterpret_cast<std::vector<screenDef>*>(dwData);

		MONITORINFOEX iMonitor;
		iMonitor.cbSize = sizeof(MONITORINFOEX);
		GetMonitorInfo(hMonitor, &iMonitor);

		if (iMonitor.dwFlags == DISPLAY_DEVICE_MIRRORING_DRIVER)
		{
			return true;
		}
		else
		{
			auto m = iMonitor.rcMonitor;
			auto x = m.left;
			auto y = m.top;
			auto w = m.right - m.left;
			auto h = m.bottom - m.top;
			screenDef screen(x, y, w, h);
			screens->push_back(screen);

			return true;
		};
	}

	virtual std::vector<screenDef> getScreens()
	{
		EnumDisplayMonitors(NULL, NULL, ScreenInfoEnumProc, (LPARAM)(&screens));

		return screens;
	}

private:
	HDC screenDC = nullptr;
	HDC memoryDC = nullptr;
	HBITMAP hBitmap = nullptr;
	HGDIOBJ oldBmp = nullptr;
	unsigned char* lpPixels = nullptr;
	std::vector<screenDef> screens;

	bool init = false;
};

#endif //SCREENSHOTWIN_H