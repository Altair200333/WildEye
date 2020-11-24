#pragma once
#include <string>

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_glfw.h"
#include "ImGui/imgui_impl_opengl3.h"
#include "ImGuiLayer.h"
#include "RenderLayersSet.h"
#include "Window.h"
#include "WebObserver.h"
class SPATEngine
{
	Window window;
	RenderLayersSet layers;
public:
	Window& getWindow();
	inline static SPATEngine* runningInstance;
	SPATEngine(const std::string& windowName = "SPAT renderer");

	void run();
};

inline Window& SPATEngine::getWindow()
{
	return window;
}

inline SPATEngine::SPATEngine(const std::string& windowName): window(windowName)
{
	runningInstance = this;
}

inline void SPATEngine::run()
{
	auto imgui = new ImGuiLayer(getWindow().getGLFWwindow());
	auto webObserver = new WebObserver();
	
	imgui->subscribeGui(*webObserver);

	layers.add(webObserver);
	layers.add(imgui);
	while (window.running)
	{
		window.clear();
		
		layers.render();
		
		//Update window
		window.onUpdate();
		
		//process input
		
		if (Input::getKeyDown(KeyCode::ESC))
		{
			window.close();
		}
	}
}

