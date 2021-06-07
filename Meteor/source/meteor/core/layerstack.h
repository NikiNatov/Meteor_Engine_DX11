#pragma once

#include "base.h"
#include "layer.h"

#include <vector>

namespace meteor
{
	class LayerStack
	{
	public:
		LayerStack() = default;
		~LayerStack();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		void PopLayer(Layer* layer);
		void PopOverlay(Layer* overlay);

		std::vector<Layer*>::iterator		begin() { return m_Layers.begin(); }
		std::vector<Layer*>::iterator		end() { return m_Layers.end(); }
		std::vector<Layer*>::const_iterator begin() const { return m_Layers.cbegin(); }
		std::vector<Layer*>::const_iterator end() const { return m_Layers.cend(); }
	private:
		std::vector<Layer*> m_Layers;
		mtrUInt m_LayerInsertIndex = 0;
	};
}