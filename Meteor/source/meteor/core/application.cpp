#include "pch.h"
#include "application.h"
#include "logger.h"
#include "input.h"
#include "meteor\renderer\api\gfxcontext.h"
#include "meteor\renderer\api\gfxrendererapi.h"
#include "meteor\renderer\renderer3d.h"

namespace meteor
{
	Application* Application::ms_Instance = nullptr;

	// --------------------------------------------------------------------------------------------------------------------------------------
	Application::Application()
	{
		METEOR_ENGINE_ASSERT(!ms_Instance, "Application already exists!");
		ms_Instance = this;

		WindowProperties properties;
		properties.Title = "Meteor Engine";
		properties.Width = 1280;
		properties.Height = 720;
		properties.VSync = true;
		properties.EventCallback = METEOR_BIND_EVENT_FN(Application::OnEvent);

		m_Window = Window::Create(properties);

		Input::Initialize(m_Window->GetWindowHandle());
		RendererAPI::Initialize();
		Renderer3D::Initialize();

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
	}

	// --------------------------------------------------------------------------------------------------------------------------------------
	Application::~Application()
	{
	}

	// --------------------------------------------------------------------------------------------------------------------------------------
	void Application::Run()
	{
		while (m_Running)
		{
			Timestep ts = m_Timer.GetTimestep();
			//METEOR_ENGINE_LOG_INFO("FPS: {0}", 1000.0f / ts.GetMilliseconds());

			m_Timer.Reset();

			{
				if (!m_Window->IsMinimized())
				{
					for (auto layer : m_LayerStack)
						layer->OnUpdate(ts);
				}

				m_ImGuiLayer->Begin();
				for (auto layer : m_LayerStack)
					layer->OnImGuiRender();
				m_ImGuiLayer->End();

				m_Window->OnUpdate();
			}

			m_Timer.Stop();
		}
	}

	// --------------------------------------------------------------------------------------------------------------------------------------
	void Application::Close()
	{
		m_Running = false;
	}

	// --------------------------------------------------------------------------------------------------------------------------------------
	void Application::PushLayer(Layer* layer)
	{
		layer->OnAttach();
		m_LayerStack.PushLayer(layer);
	}

	// --------------------------------------------------------------------------------------------------------------------------------------
	void Application::PushOverlay(Layer* overlay)
	{
		overlay->OnAttach();
		m_LayerStack.PushOverlay(overlay);
	}

	// --------------------------------------------------------------------------------------------------------------------------------------
	void Application::OnEvent(Event& ev)
	{
		EventDispatcher dispatcher(ev);
		dispatcher.Dispatch<WindowClosedEvent>(METEOR_BIND_EVENT_FN(Application::OnWindowClosed));

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			if (ev.Handled)
				break;

			(*--it)->OnEvent(ev);
		}
	}

	// --------------------------------------------------------------------------------------------------------------------------------------
	mtrBool Application::OnWindowClosed(WindowClosedEvent& ev)
	{
		Close();
		return true;
	}
}