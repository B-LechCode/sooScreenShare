//SooScreenServer by Christian Schneider (schnaader), 2019
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */
#ifndef SCREENSHOTWIN_H
#define SCREENSHOTWIN_H
#if WITH_WINAPI

namespace shot {
#define DISPLAY_NUMBER "Display Number"
#define OVERRIDE_DISPLAY_SELECTION "Override Display Selection"
#define Grab_X "X"
#define Grab_Y "Y"
#define Grab_W "Width"
#define Grab_H "Height"
}

#include "iscreenshot.h"
#include <windows.h>

class screenShotWin : public IscreenShot
{
	virtual void parameterMapChangedEvent()
	{
		initialize();
	}

	void applyParameters()
	{
		std::string overrideStr = m_parameters[OVERRIDE_DISPLAY_SELECTION].value();
		std::transform(overrideStr.begin(), overrideStr.end(), overrideStr.begin(), [](int c) -> int { return std::tolower(c); });
		bool override = overrideStr == "true";



		if (!override)
		{
			int displayNumber = std::stoi(m_parameters[DISPLAY_NUMBER].value());
			auto displays = getScreens();

			if (displays.size() - 1 < displayNumber)
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

	void destruct()
	{
		m_init = false;

		DeleteDC(memoryDC);
		DeleteDC(screenDC);

		if (lpPixels)
			delete[] lpPixels;
	}

	virtual void parameterChangedEvent(const std::string& key)
	{
		initialize();
	}

public:
	screenShotWin() : IscreenShot(), m_init(false)
	{
		m_defaultParameters[OVERRIDE_DISPLAY_SELECTION] = parameter("Override display selection to freely define the grab ROI", "bool", "false");
		m_defaultParameters[DISPLAY_NUMBER] = parameter("The display to grab", "int16", "0");
		m_defaultParameters[Grab_X] = parameter("The X coordinate of the grab ROI", "int32", "0");
		m_defaultParameters[Grab_Y] = parameter("The Y coordinate of the grab ROI", "int32", "0");
		m_defaultParameters[Grab_W] = parameter("The width of the grab ROI", "uint32", "640");
		m_defaultParameters[Grab_H] = parameter("The width of the grab ROI", "uint32", "480");
		setParameters(m_defaultParameters);
	}

	virtual cv::Mat operator() () {
		if (!m_init)
			return cv::Mat();

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

	virtual void initialize()
	{
		applyParameters();
		if (m_init)
			destruct();

		hwnd = GetDesktopWindow();
		screenDC = GetDC(hwnd);
		memoryDC = CreateCompatibleDC(screenDC);

		hBitmap = CreateCompatibleBitmap(screenDC, m_w, m_h);

		lpPixels = new unsigned char[m_w * m_h * 4];

		m_init = true;
	}

	virtual ~screenShotWin() {
		destruct();
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

	bool m_init;
};
#endif //WITH_WINAPI
#endif //SCREENSHOTWIN_H