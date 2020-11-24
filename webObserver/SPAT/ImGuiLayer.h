#pragma once
#include <functional>
#include <GLFW/glfw3.h>

#include "ImGui/imgui_impl_glfw.h"
#include "ImGui/imgui_impl_opengl3.h"
#include "RenderLayer.h"
#include "Logger.h"

class ImGuiLayer final : public RenderLayer
{
	bool show_demo_window = true;
	GLFWwindow* window;

	std::vector<std::function<void()>> guiSubs;
public:
	std::function<void()> f;
	
	ImGuiLayer(GLFWwindow* _window):window(_window)
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		ImGui::StyleColorsDark();

		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}
		
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 150");

	}
	template<typename T, decltype(&T::onImGuiDraw)* A = nullptr>
	void subscribeGui(T& sub)
	{
		guiSubs.push_back([&sub]() {sub.onImGuiDraw(); });
	}
	void onBegin() override
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

	}
	
	void onEnd() override
	{
		ImGuiIO& io = ImGui::GetIO();
		int width, height;
		glfwGetWindowSize(window, &width, &height);
		io.DisplaySize = ImVec2(width, height);

		// Rendering
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}
	float color[3] = {255, 0, 119};
	void onDraw() override
	{
		//ImGui::ShowDemoWindow(&show_demo_window);

		for (int i = 0; i < guiSubs.size(); ++i)
		{
			guiSubs[i]();
		}
	}
	~ImGuiLayer()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}
};
