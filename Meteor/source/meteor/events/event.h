#pragma once

#include "meteor/core/base.h"

#include <functional>

namespace meteor
{
	enum class EventType
	{
		None = 0,
		WindowResized, WindowClosed, WindowMoved,
		KeyPressed, KeyReleased, KeyTyped,
		MouseButtonPressed, MouseButtonReleased, MouseScrolled, MouseMoved
	};

	enum EventCategory
	{
		None = 0,
		EventCategoryKeyboard		= BIT(0),
		EventCategoryApplication	= BIT(1),
		EventCategoryMouse			= BIT(2),
		EventCategoryInput			= BIT(3),
		EventCategoryMouseButton	= BIT(4)
	};

	#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::##type; }\
									virtual EventType GetEventType() const override { return GetStaticType(); }\
									virtual const mtrChar* GetEventName() const override { return #type;}

	#define EVENT_CLASS_CATEGORY(category) virtual mtrInt GetEventCategoryFlags() const override { return category; }

	class Event
	{
	public:
		Event() = default;
		virtual ~Event() = default;

		virtual EventType GetEventType() const = 0;
		virtual const mtrChar* GetEventName() const = 0;
		virtual mtrInt GetEventCategoryFlags() const = 0;

		virtual mtrString ToString() const { return GetEventName(); }

		virtual mtrBool IsInCategory(EventCategory category)
		{
			return GetEventCategoryFlags() & category;
		}

		mtrBool Handled = false;
	};

	class EventDispatcher
	{
	public:
		EventDispatcher(Event& event)
			: m_Event(event)
		{

		}

		template<typename T, typename F>
		mtrBool Dispatch(const F& func)
		{
			if (m_Event.GetEventType() == T::GetStaticType())
			{
				m_Event.Handled = func(static_cast<T&>(m_Event));
				return true;
			}

			return false;
		}

	private:
		Event& m_Event;
	};

	inline std::ostream& operator<<(std::ostream& stream, const Event& event)
	{
		stream << event.ToString();
		return stream;
	}
}