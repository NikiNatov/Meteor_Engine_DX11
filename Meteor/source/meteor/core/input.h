#pragma once

#include "base.h"
#include "keycodes.h"
#include "mousecodes.h"

#include <glm\glm.hpp>

namespace meteor
{
	class Input
	{
	public:
		static void			 Initialize(void* windowHandle);
		static mtrBool		 IsKeyPressed(Key keycode);
		static mtrBool		 IsMouseButtonPressed(MouseButton button);

		static glm::vec2	 GetMousePosition();
		static void			 SetMouseCursor(mtrBool enabled);
		static void			 SetMousePosition(const glm::vec2& position);
		static bool			 IsCursorEnabled();
	private:
		static void*		 ms_WindowHandle;
		static mtrBool		 ms_CursorEnabled;
	};
}