#include "pch.h"
#include "meteor\core\base.h"
#include "meteor\core\input.h"

namespace meteor
{
#if defined(METEOR_PLATFORM_WINDOWS)
	void*	Input::ms_WindowHandle = nullptr;
	mtrBool Input::ms_CursorEnabled = true;

	// --------------------------------------------------------------------------------------------------------------------------------------
	void Input::Initialize(void* windowHandle)
	{
		ms_WindowHandle = windowHandle;
	}

	// --------------------------------------------------------------------------------------------------------------------------------------
	bool Input::IsKeyPressed(Key key)
	{
		if (GetAsyncKeyState((mtrInt)key) & (1 << 15))
			return true;
		
		return false;
	}

	// --------------------------------------------------------------------------------------------------------------------------------------
	bool Input::IsMouseButtonPressed(MouseButton button)
	{
		if (GetAsyncKeyState((mtrInt)button) & (1 << 15))
			return true;

		return false;
	}

	// --------------------------------------------------------------------------------------------------------------------------------------
	glm::vec2 Input::GetMousePosition()
	{
		POINT point;

		mtrBool result = GetCursorPos(&point);
		METEOR_ENGINE_ASSERT(result, "Failed to get the cursor position!");

		result = ScreenToClient(static_cast<HWND>(ms_WindowHandle), &point);
		METEOR_ENGINE_ASSERT(result, "Could not convert from screen coordinates to client coordinates!");
		return { point.x, point.y };
	}

	// --------------------------------------------------------------------------------------------------------------------------------------
	void Input::SetMouseCursor(mtrBool enabled)
	{
		ms_CursorEnabled = enabled;

		if (enabled)
		{
			while (ShowCursor(true) < 0);
		}
		else
		{
			while (ShowCursor(false) >= 0);
		}
	}

	// --------------------------------------------------------------------------------------------------------------------------------------
	void Input::SetMousePosition(const glm::vec2& position)
	{
		POINT point = { (LONG)position.x, (LONG)position.y };
		ClientToScreen(static_cast<HWND>(ms_WindowHandle), &point);
		SetCursorPos(point.x, point.y);
	}

	// --------------------------------------------------------------------------------------------------------------------------------------
	bool Input::IsCursorEnabled()
	{
		return ms_CursorEnabled;
	}
#endif // METEOR_PLATFORM_WINDOWS
}