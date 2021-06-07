#pragma once

#include "event.h"
#include "meteor/core/keycodes.h"
#include <sstream>

namespace meteor
{
	class KeyEvent : public Event
	{
	public:
		inline Key GetKeyCode() const { return m_KeyCode; }

		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
	protected:
		KeyEvent(Key keycode)
			: m_KeyCode(keycode)
		{
		}

		Key m_KeyCode;
	};

	class KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(Key keycode, mtrInt repeatCount)
			: KeyEvent(keycode), m_RepeatCount(repeatCount)
		{
		}

		virtual mtrString ToString() const override
		{
			std::stringstream ss;

			ss << "KeyPressed: " << (mtrInt)m_KeyCode << "(" << m_RepeatCount << ")";

			return ss.str();
		}

		inline mtrInt GetRepeatCount() const { return m_RepeatCount; }

		EVENT_CLASS_TYPE(KeyPressed)

	private:
		mtrInt m_RepeatCount;
	};


	class KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(Key keycode)
			: KeyEvent(keycode)
		{
		}

		virtual mtrString ToString() const override
		{
			std::stringstream ss;

			ss << "KeyReleased: " << (mtrInt)m_KeyCode;

			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyReleased)

	};

	class KeyTypedEvent : public KeyEvent
	{
	public:
		KeyTypedEvent(Key keycode)
			: KeyEvent(keycode)
		{
		}

		mtrString ToString() const override
		{
			std::stringstream ss;

			ss << "KeyTyped: " << (mtrInt)m_KeyCode;

			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyTyped)
	};
}