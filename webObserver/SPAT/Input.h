#pragma once
#include <GLFW/glfw3.h>
#include <map>

namespace KeyCode
{
	using Key = int;
	enum : Key
	{
		ESC = GLFW_KEY_ESCAPE,
	
		W = GLFW_KEY_W,
		A = GLFW_KEY_A,
		S = GLFW_KEY_D,
		D = GLFW_KEY_D,
		Q = GLFW_KEY_Q,
	
		num1 = GLFW_KEY_1,
		num2 = GLFW_KEY_2,
		num3 = GLFW_KEY_3,
		num4 = GLFW_KEY_4,
		num5 = GLFW_KEY_5,
		num6 = GLFW_KEY_6,
		num7 = GLFW_KEY_7,
		num8 = GLFW_KEY_8,
		num9 = GLFW_KEY_9,
		num0 = GLFW_KEY_0,
	
		LShift = GLFW_KEY_LEFT_SHIFT,
		RShift = GLFW_KEY_RIGHT_SHIFT,
		
		TAB = GLFW_KEY_TAB,
	
		LCtrl = GLFW_KEY_LEFT_CONTROL,
		RCtrl = GLFW_KEY_RIGHT_CONTROL,
		
		Space = GLFW_KEY_SPACE
	};
};

class Input final
{
	inline static std::map<KeyCode::Key, int> keyEvents;

	static bool contains(KeyCode::Key key)
	{
		return keyEvents.find(key) != keyEvents.end();
	}
public:

	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		keyEvents[key] = action;
		//std::cout << "key " << key << "\n";
	}
	static bool getKeyDown(KeyCode::Key key)
	{
		return contains(key) && keyEvents[key] == GLFW_PRESS;
	}
	static bool getKeyUp(KeyCode::Key key)
	{
		return contains(key) && keyEvents[key] == GLFW_RELEASE;
	}

	static bool getKeyPressed(KeyCode::Key key)
	{
		return contains(key) && keyEvents[key] == GLFW_REPEAT;//glfwGetKey(window, key) == GLFW_PRESS;
	}
	static void poll()
	{
		keyEvents.clear();
	}
};