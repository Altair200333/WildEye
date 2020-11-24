#pragma once
#define TJE_IMPLEMENTATION
#include "tiny_jpeg.h"

#include <iostream>
#include <string>
#include <windows.h>


class ScreenshotMaker final
{
public:
	struct ScreenInfo final
	{
		int width;
		int height;
		HBITMAP bitmap;
		HDC memory;
	};
	static ScreenInfo makeScreenshot()
	{
		// get the device context of the screen
		HDC hScreenDC = CreateDC(L"DISPLAY", NULL, NULL, NULL);
		// and a device context to put it in
		HDC hMemoryDC = CreateCompatibleDC(hScreenDC);

		int width = GetDeviceCaps(hScreenDC, HORZRES);
		int height = GetDeviceCaps(hScreenDC, VERTRES);

		// maybe worth checking these are positive values
		HBITMAP hBitmap = CreateCompatibleBitmap(hScreenDC, width, height);

		// get a new bitmap
		HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemoryDC, hBitmap);

		BitBlt(hMemoryDC, 0, 0, width, height, hScreenDC, 0, 0, SRCCOPY);
		hBitmap = (HBITMAP)SelectObject(hMemoryDC, hOldBitmap);

		return {width, height, hBitmap, hMemoryDC};
	}
	static bool saveScreenshot(ScreenInfo& info, std::string path)
	{
		BITMAPINFO MyBMInfo = { 0 };
		MyBMInfo.bmiHeader.biSize = sizeof(MyBMInfo.bmiHeader);

		// Get the BITMAPINFO structure from the bitmap
		if (0 == GetDIBits(info.memory, info.bitmap, 0, 0, NULL, &MyBMInfo, DIB_RGB_COLORS)) {
			//std::cout << "error" << std::endl;
			return true;
		}

		// create the bitmap buffer
		BYTE* lpPixels = new BYTE[MyBMInfo.bmiHeader.biSizeImage];

		// Better do this here - the original bitmap might have BI_BITFILEDS, which makes it
		// necessary to read the color table - you might not want this.
		MyBMInfo.bmiHeader.biCompression = BI_RGB;

		// get the actual bitmap buffer
		if (0 == GetDIBits(info.memory, info.bitmap, 0, MyBMInfo.bmiHeader.biHeight, (LPVOID)lpPixels, &MyBMInfo, DIB_RGB_COLORS)) {
			//std::cout << "error2" << std::endl;
			return true;
		}

		for (int i = 0; i < info.height / 2; ++i)
		{
			for (int j = 0; j < info.width * 4; ++j)
			{
				std::swap(lpPixels[i * info.width * 4 + j], lpPixels[(info.height - i - 1) * info.width * 4 + j]);
			}
		}
		for (int i = 0; i < info.width * info.height; ++i)
		{
			std::swap(lpPixels[i * 4], lpPixels[i * 4 + 2]);
		}
		
		tje_encode_to_file(path.c_str(), info.width, info.height, 4, reinterpret_cast<const unsigned char*>(lpPixels));
		delete[] lpPixels;

		return false;
	}
};
