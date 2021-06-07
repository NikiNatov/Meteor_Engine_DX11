#pragma once

#include "event.h"
#include "meteor\core\mousecodes.h"
#include <sstream>

namespace meteor
{
	class MouseButtonEvent : public Event
	{
	public:
		inline MouseButton GetButton() const { return m_Button; }

		EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryMouse)
	protected:
		MouseButtonEvent(MouseButton button)
			: m_Button(button)
		{
		}

		MouseButton m_Button;
	};

	class MouseButtonPressedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonPressedEvent(MouseButton button)
			: MouseButtonEvent(button)
		{
		}

		mtrString ToString() const override
		{
			std::stringstream ss;

			ss << "MouseButtonPressed: " << (mtrInt)m_Button;

			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonPressed)
	};

	class MouseButtonReleasedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonReleasedEvent(MouseButton button)
			: MouseButtonEvent(button)
		{
		}

		mtrString ToString() const override
		{
			std::stringstream ss;

			ss << "MouseButtonReleased: " << (mtrInt)m_Button;

			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonReleased)
	};

	class MouseMovedEvent : public Event
	{
	public:
		MouseMovedEvent(mtrFloat xPos, mtrFloat yPos)
			: m_XPos(xPos), m_YPos(yPos)
		{
		}

		inline mtrFloat GetXPosition() const { return m_XPos; }
		inline mtrFloat GetYPosition() const { return m_YPos; }

		mtrString ToString() const override
		{
			std::stringstream ss;

			ss << "MouseMoved: " << m_XPos << ", " << m_YPos;

			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseMoved)
		EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryMouse)
	private:
		mtrFloat m_XPos;
		mtrFloat m_YPos;
	};

	class MouseScrolledEvent : public Event
	{
	public:
		MouseScrolledEvent(mtrFloat xOffset, mtrFloat yOffset)
			: m_XOffset(xOffset), m_YOffset(yOffset)
		{
		}

		inline mtrFloat GetXOffset() const { return m_XOffset; }
		inline mtrFloat GetYOffset() const { return m_YOffset; }

		mtrString ToString() const override
		{
			std::stringstream ss;

			ss << "MouseScrolled: " << m_XOffset << ", " << m_YOffset;

			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseScrolled)
		EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryMouse)
	private:
		mtrFloat m_XOffset;
		mtrFloat m_YOffset;
	};
}