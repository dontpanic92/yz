#ifndef __YZ_BUTTON_P_HPP__
#define __YZ_BUTTON_P_HPP__

#include <Windows.h>
#include <yz/window.hpp>
#include <yz/private/ui_object_p.hpp>

namespace yz
{
	class window_p: public ui_object_p
	{
	private:
		window* q;
	public:
		WNDCLASSEX wcx;
		HINSTANCE hInstance;

		window_p(window* q, const std::string& name, object* parent, klass* klass);
		ATOM register_window_class();
		void init();
		void init_property();

		LRESULT on_message(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
		static LRESULT CALLBACK static_wnd_proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	};
}

#endif