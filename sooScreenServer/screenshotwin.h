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

		unsigned char* lpPixels = new unsigned char[bmInfo.bmiHeader.biSizeImage];

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

private:
	HDC screenDC = nullptr;
	HDC memoryDC = nullptr;
	HBITMAP hBitmap = nullptr;
	HGDIOBJ oldBmp = nullptr;
	unsigned char* lpPixels = nullptr;

	bool init = false;
};

#endif //SCREENSHOTWIN_H