#pragma once
#include <map>
#include <string>
#include "Reflections.h"
#include "Component.h"

class ComponentManager final
{
	std::map<std::string, Component*> components;
public:
	
	template<typename T, typename std::enable_if<std::is_base_of<Component, T>::value>::type* = nullptr>
	void addComponent(T* component)
	{
		components.insert(Reflections::type_name<T>(), component);
	}
	
	template<typename T>
	Component* getComponent()
	{
		auto it = components.find(Reflections::type_name<T>());
		return it == components.end() ? nullptr : it->second;
	}

	~ComponentManager()
	{
		for (auto& component : components)
		{
			delete component.second;
		}
	}
};
