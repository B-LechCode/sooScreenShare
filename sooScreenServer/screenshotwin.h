//SooScreenServer by Christian Schneider (schnaader), 2019
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */
#ifndef SCREENSHOTWIN_H
#define SCREENSHOTWIN_H

#include "iscreenshot.h"
#include <windows.h>

class screenShotWin : public IscreenShot
{
public:
	screenShotWin() : IscreenShot()
	{
		init = false;
	}

	virtual cv::Mat operator() () {
		if (init)
			init = false;

		oldBmp = SelectObject(memoryDC, hBitmap);
		BitBlt(memoryDC, 0, 0, m_w, m_h, screenDC, m_x, m_y, SRCCOPY | CAPTUREBLT);

		CURSORINFO cursor = { sizeof(cursor) };
		GetCursorInfo(&cursor);
		if (cursor.flags == CURSOR_SHOWING) {
			RECT rect;
			GetWindowRect(hwnd, &rect);
			ICONINFOEXW info = { sizeof(info) };
			if (GetIconInfoExW(cursor.hCursor, &info) != 0) {
				const int x = cursor.ptScreenPos.x - rect.left - rect.left - info.xHotspot;
				const int y = cursor.ptScreenPos.y - rect.top - rect.top - info.yHotspot;
				BITMAP bmpCursor = { 0 };
				GetObject(info.hbmMask, sizeof(bmpCursor), &bmpCursor);
				DrawIconEx(memoryDC, x, y, cursor.hCursor, bmpCursor.bmWidth, bmpCursor.bmHeight, 0, NULL, DI_NORMAL);
			}
		}

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

	virtual void initialize(int32_t x, int32_t y, uint32_t w, uint32_t h)
	{
		hwnd = GetDesktopWindow();
		screenDC = GetDC(hwnd);
		memoryDC = CreateCompatibleDC(screenDC);

		hBitmap = CreateCompatibleBitmap(screenDC, w, h);

		m_x = x;
		m_y = y;
		m_w = w;
		m_h = h;

		lpPixels = new unsigned char[w * h * 4];

		init = true;
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
	HWND hwnd = nullptr;
	HDC screenDC = nullptr;
	HDC memoryDC = nullptr;
	HBITMAP hBitmap = nullptr;
	HGDIOBJ oldBmp = nullptr;
	unsigned char* lpPixels = nullptr;
	std::vector<screenDef> screens;

	bool init = false;
};

#endif //SCREENSHOTWIN_H