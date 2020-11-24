#pragma once
#include <functional>
#include <iostream>
#include <vector>

#include <Windows.h>
#include <winhttp.h>
class HTTPRequest final
{
public:
	static void requestBase(std::string address, std::string method, std::function<BOOL(HINTERNET)> sendRequest, std::function<void(std::string)> functionOnResponce)
	{
        DWORD dwSize = 0;
        DWORD dwDownloaded = 0;
        LPSTR pszOutBuffer;
        BOOL  bResults = FALSE;
        HINTERNET  hSession = NULL,
            hConnect = NULL,
            hRequest = NULL;

        // Use WinHttpOpen to obtain a session handle.
        hSession = WinHttpOpen(L"WinHTTP Example/1.0",
            WINHTTP_ACCESS_TYPE_NO_PROXY,
            WINHTTP_NO_PROXY_NAME,
            WINHTTP_NO_PROXY_BYPASS, 0);

        //DWORD opt = SECURITY_FLAG_IGNORE_CERT_CN_INVALID | SECURITY_FLAG_IGNORE_UNKNOWN_CA;
        //WinHttpSetOption(hSession, WINHTTP_FLAG_SECURE, &opt, sizeof(opt));
        // Specify an HTTP server.
        if (hSession)
            hConnect = WinHttpConnect(hSession, std::wstring(address.begin(), address.end()).c_str(),
                81, 0);

        // Create an HTTP request handle.
        if (hConnect)
            hRequest = WinHttpOpenRequest(hConnect, std::wstring(method.begin(), method.end()).c_str(), NULL,
                NULL, WINHTTP_NO_REFERER,
                WINHTTP_DEFAULT_ACCEPT_TYPES,
                0);

        // Send a request.
        if (hRequest)
        {
            bResults = sendRequest(hRequest);
            /*bResults = WinHttpSendRequest(hRequest,
                 WINHTTP_NO_ADDITIONAL_HEADERS,
                 0, WINHTTP_NO_REQUEST_DATA, 0,
                 0, 0);*/
        }
            


        // End the request.
        if (bResults)
            bResults = WinHttpReceiveResponse(hRequest, NULL);

        // Keep checking for data until there is nothing left.
        if (bResults)
        {
            do
            {
                // Check for available data.
                dwSize = 0;
                if (!WinHttpQueryDataAvailable(hRequest, &dwSize))
                    printf("Error %u in WinHttpQueryDataAvailable.\n", GetLastError());

                // Allocate space for the buffer.
                pszOutBuffer = new char[dwSize + 1];
                if (!pszOutBuffer)
                {
                    printf("Out of memory\n");
                    dwSize = 0;
                }
                else
                {
                    // Read the Data.
                    ZeroMemory(pszOutBuffer, dwSize + 1);

                    if (!WinHttpReadData(hRequest, (LPVOID)pszOutBuffer,
                        dwSize, &dwDownloaded))
                        printf("Error %u in WinHttpReadData.\n", GetLastError());
                    else
                    {
                        functionOnResponce(pszOutBuffer);
                        //printf("%s\n", pszOutBuffer);
                    }
                    // Free the memory allocated to the buffer.
                    delete[] pszOutBuffer;
                }

            } while (dwSize > 0);
        }

        // Report any errors.
        if (!bResults)
            printf("Error %d has occurred.\n", GetLastError());

        // Close any open handles.
        if (hRequest) WinHttpCloseHandle(hRequest);
        if (hConnect) WinHttpCloseHandle(hConnect);
        if (hSession) WinHttpCloseHandle(hSession);
	}

	static void getRequest(std::string address, std::function<void(std::string)> onResponce, std::string what="#")
	{
        std::vector<unsigned char> data(what.begin(), what.end());
        requestBase(address, "GET", [&data](HINTERNET request)
            {
                return WinHttpSendRequest(request,
                    WINHTTP_NO_ADDITIONAL_HEADERS,
                    0, data.data(), data.size(),
                    data.size(), 0);
            },

            onResponce);
	}
    static void postRequest(std::string address, std::string header, std::vector<unsigned char>& data)
    {
        data.insert(data.begin(), header.begin(), header.end());
        requestBase(address, "POST", [&data](HINTERNET request)
            {
                return WinHttpSendRequest(request,
                    WINHTTP_NO_ADDITIONAL_HEADERS,
                    0, data.data(), data.size(),
                    data.size(), 0);
            },

            [](std::string responce) {
                //std::cout << responce << "\n";
            });
    }
	
};