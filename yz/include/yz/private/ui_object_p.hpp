#ifndef __YZ_UI_OBJECT_P_HPP__
#define __YZ_UI_OBJECT_P_HPP__

#include <Windows.h>
#include <yz/private/object_p.hpp>
#include <string>
#include <codecvt>

namespace yz
{
	class ui_object_p : public object_p
	{
	private:
		ui_object* q;
	public:
		std::wstring wname;
		HWND hWnd;
		wstring_convert<codecvt_utf8_utf16<wchar_t>, wchar_t> wstr_conv;

		ui_object_p(ui_object* q, const std::string& name, object* parent, klass* klass);
		void init();
		void init_property();

		LRESULT on_message(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	};
}
#endif
