// Copyright ⓒ 2020 Valentyn Bondarenko. All rights reserved.

#include <StdAfx.hpp>

#include <Resource.h>

#include <SplashScreen.hpp>

namespace be::utils
{
	SplashScreen::SplashScreen()
	{
		name = "BE Splash Screen"s;
		class_name = name + " Class"s;
	}

	void SplashScreen::create()
	{
		loadBitMap();
		
		DWORD exStyle = 0ul;
		auto xPos = 0u, yPos = 0u;
		auto width = bitmapInfo.bmiHeader.biWidth;
		auto height = bitmapInfo.bmiHeader.biHeight;

		RECT parentRect = { 0 };
		GetWindowRect(GetDesktopWindow(), &parentRect);

		xPos = parentRect.left + (parentRect.right - parentRect.left) / 2 - (width / 2);
		yPos = parentRect.top + (parentRect.bottom - parentRect.top) / 2 - (height / 2);

		HWND hwnd = GetDesktopWindow();
		HMENU menu = nullptr;

		winhandle = CreateWindowExA(exStyle,
									class_name.c_str(),
									name.c_str(),
									WS_POPUP | WS_VISIBLE,
								    xPos, yPos,
									width, height,
									hwnd,
									menu,
									m_instance,
									this);

		SetWindowPos(winhandle, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	}

	void SplashScreen::registerClass()
	{
		winclass.cbSize = sizeof(WNDCLASSEX);
		winclass.style = CS_HREDRAW | CS_VREDRAW;
		winclass.lpfnWndProc = WNDPROC(WndProc);
		winclass.cbClsExtra = 0;
		winclass.cbWndExtra = 0;
		winclass.hInstance = GetModuleHandleA(nullptr);
		winclass.hIcon = LoadIconA(nullptr, MAKEINTRESOURCE(IDI_BONDENGINE));
		winclass.hCursor = LoadCursorA(nullptr, IDC_ARROW);
		winclass.hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
		winclass.lpszMenuName = nullptr;
		winclass.lpszClassName = class_name.c_str();
		winclass.hIconSm = LoadIconA(nullptr, MAKEINTRESOURCE(IDI_SMALL));

		if (!RegisterClassExA(&winclass))
			MessageBoxA(nullptr, "Can't register window class", "Error", MB_OK | MB_ICONERROR);
	}

	void SplashScreen::loadBitMap()
	{
		m_instance = GetModuleHandleA(nullptr);
		bitmap = LoadBitmapA(m_instance, MAKEINTRESOURCE(IDB_SPLASH));

		HWND hwndDesktop = GetDesktopWindow();
		HDC hdcDesktop = GetDC(hwndDesktop);

		memset(&bitmapInfo, 0, sizeof(BITMAPINFOHEADER));
		bitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);

		GetDIBits(hdcDesktop,		// handle to DC
				  bitmap,			// handle to bitmap
				  0,				// first scan line to set
				  0,				// number of scan lines to copy
				  NULL,				// array for bitmap bits
				  &bitmapInfo,		// bitmap data buffer
				  DIB_RGB_COLORS);	// RGB or palette index
	}

	void SplashScreen::isShown(bool is_shown) const
	{
		ShowWindow(winhandle, is_shown ? SW_SHOW : SW_HIDE);
		UpdateWindow(winhandle);
		InvalidateRect(winhandle, nullptr, FALSE);
	}

	void SplashScreen::show()
	{
		PAINTSTRUCT ps{ 0 };
		HDC paintDC = BeginPaint(winhandle, &ps);
		HDC imageDC = CreateCompatibleDC(paintDC);

		BITMAPINFO bitmapInfo{ 0 };
		memset(&bitmapInfo, 0, sizeof(BITMAPINFOHEADER));
		bitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		int scanLines = GetDIBits(imageDC,		// handle to DC
								  bitmap,			// handle to bitmap
								  0,				// first scan line to set
								  0,				// number of scan lines to copy
								  NULL,				// array for bitmap bits
								  &bitmapInfo,		// bitmap data buffer
								  DIB_RGB_COLORS);	// RGB or palette index

							  // Paint the bitmap image.
		HBITMAP pOldBitmap = static_cast<decltype(pOldBitmap)>(SelectObject(imageDC, bitmap));
		auto width = bitmapInfo.bmiHeader.biWidth;
		auto height = bitmapInfo.bmiHeader.biHeight;
		BitBlt(paintDC, 0, 0, width, height, imageDC, 0, 0, SRCCOPY);
		SelectObject(imageDC, pOldBitmap);

		EndPaint(winhandle, &ps);
	}

	void SplashScreen::hide() const
	{
		if (winhandle)
		{
			ShowWindow(winhandle, FALSE);
			DestroyWindow(winhandle);
		}
	};

	bool SplashScreen::update()
	{
		static MSG msg{ 0 };

		while (PeekMessageA(&msg, winhandle, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessageA(&msg);

			return true;
		}

		return false;
	}

	LRESULT CALLBACK SplashScreen::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{
			case WM_PAINT:
				break;

			case WM_TIMER:
			case WM_QUIT:
				break;

			default:
				return DefWindowProc(hwnd, message, wParam, lParam);
		}

		return 0;
	}
}