#pragma once

#include "event.h"

#include <sstream>

namespace meteor
{
	class WindowResizedEvent : public Event
	{
	public:
		WindowResizedEvent(mtrUInt width, mtrUInt height)
			: m_Width(width), m_Height(height)
		{
		}

		inline mtrUInt GetWidth() const { return m_Width; }
		inline mtrUInt GetHeight() const { return m_Height; }

		mtrString ToString() const override
		{
			std::stringstream ss;

			ss << "WindowResized: " << m_Width << ", " << m_Height;

			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowResized)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	private:
		mtrUInt m_Width;
		mtrUInt m_Height;
	};

	class WindowClosedEvent : public Event
	{
	public:
		WindowClosedEvent()
		{
		}

		EVENT_CLASS_TYPE(WindowClosed)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class WindowMovedEvent : public Event
	{
	public:

	private:
	};
}