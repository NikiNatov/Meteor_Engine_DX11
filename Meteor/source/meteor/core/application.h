#pragma once

#include "base.h"
#include "layerstack.h"
#include "meteor\events\eventinclude.h"
#include "timer.h"
#include "window.h"

#include "meteor\imgui\imguilayer.h"

namespace meteor
{
	class Application
	{
	public:
		Application();
		Application(const Application&) = delete;
		Application& operator=(const Application&) = delete;

		virtual ~Application();

		void					  Run();
		void					  Close();
								  
		void					  PushLayer(Layer* layer);
		void					  PushOverlay(Layer* overlay);
								  
		void					  OnEvent(Event& ev);
								  
		const ScopedPtr<Window>&  GetWindow() const { return m_Window; }
		static Application&		  GetApplication() { return *ms_Instance; }
	private:					  
		mtrBool					  OnWindowClosed(WindowClosedEvent& ev);
	private:
		static Application*		  ms_Instance;
	private:
		mtrBool					  m_Running = true;
		LayerStack				  m_LayerStack;
								  
		ScopedPtr<Window>		  m_Window;
		Timer					  m_Timer;

		ImGuiLayer*				  m_ImGuiLayer;
	};

	Application* CreateApplication();
}