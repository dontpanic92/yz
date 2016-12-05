#ifndef __YZ_BUTTON_P_HPP__
#define __YZ_BUTTON_P_HPP__

#include <Windows.h>
#include <yz/button.hpp>
#include <yz/private/ui_object_p.hpp>

namespace yz
{
	class button_p : public ui_object_p
	{
	private:
		button* q;
	public:
		HWND hParentWnd;

		void init();
		void init_property();

		LRESULT on_message(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

		button_p(button* q, const std::string& name, object* parent, klass* klass);
		bool create_window();
	};
}

#endif