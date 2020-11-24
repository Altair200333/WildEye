#pragma once
#include <vector>

#include "RenderLayer.h"

class RenderLayersSet final
{
public:
	std::vector<RenderLayer*> layers;

	void render()
	{
		for (auto* layer : layers)
		{
			layer->onBegin();
			layer->onDraw();
			layer->onEnd();
		}
	}
	void add(RenderLayer* layer)
	{
		layers.push_back(layer);
	}
};
