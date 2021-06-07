#pragma once

#include "base.h"
#include "meteor\events\eventinclude.h"

namespace meteor
{
	struct WindowProperties
	{
		using EventCallbackFn = std::function<void(Event&)>;

		mtrString		 Title = "Meteor Engine";
		mtrUInt			 Width = 1280;
		mtrUInt			 Height = 720;
		mtrBool			 VSync = true;
		EventCallbackFn  EventCallback;
	};

	class Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() = default;

		virtual void				Initialize(const WindowProperties& properties) = 0;
		virtual void				OnUpdate() = 0;

		virtual const mtrString&	GetTitle() const = 0;
		virtual mtrUInt				GetHeight() const = 0;
		virtual mtrUInt				GetWidth() const = 0;
		virtual mtrBool				IsVSync() const = 0;
		virtual mtrBool				IsFullscreen() const = 0;
		virtual mtrBool				IsMinimized() const = 0;
		virtual void*				GetWindowHandle() const = 0;

		virtual void				ToogleVSync() = 0;
		virtual void				ToogleFullscreen() = 0;
		virtual void				SetMinimized(mtrBool state) = 0;
		virtual void				SetEventCallback(const EventCallbackFn& callback) = 0;
		
		static ScopedPtr<Window>	Create(WindowProperties& properties);
	};
}