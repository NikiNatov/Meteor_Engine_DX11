#pragma once

#include "meteor\core\base.h"
#include "meteor\core\layer.h"

namespace meteor
{
	class ImGuiLayer : public Layer
	{
	public:
		virtual void	OnAttach() override;
		virtual void	OnDetach() override;
		virtual void	OnEvent(Event& event) override;

		void			SetDarkTheme();
		void			Begin();
		void			End();

		inline void		BlockEvents(mtrBool block) { m_BlockEvents = block; }
	private:
		mtrBool			m_BlockEvents = false;
    };
}