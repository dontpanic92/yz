#include <codecvt>
#include <Windows.h>
#include <yz/app.hpp>
#include <yz/util.hpp>
#include <yz/button.hpp>
#include <yz/private/button_p.hpp>

using namespace yz;
using namespace std;

klass button_base = klass_builder<button, object>("button", &object::ui(), false);

button_p::button_p(button* q, const std::string& name, object* parent, klass* klass) : ui_object_p(q, name, parent, klass)
{
	this->q = q; 
}

void button_p::init()
{
	wname = wstr_conv.from_bytes(q->name());
	if (ui_object* p = q->parent()->down_to<ui_object>()) {
		hParentWnd = (HWND)p->HWND();
	} else {
		hParentWnd = 0;
	}
	create_window();
	init_property();
}

bool button_p::create_window()
{
	hWnd = CreateWindow(TEXT("BUTTON"), L"OK", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
		q->x.default(), q->y.default(), 100, 100, hParentWnd, (HMENU)id, app::get()->hInstance(), NULL);
	if (!hWnd) {
		return false;
	}
	return true;
}

void button_p::init_property()
{
	q->text.setter([&](const string& new_value){
		SetWindowText(hWnd, wstr_conv.from_bytes(new_value).c_str());
	}).getter([&](){
		int len = GetWindowTextLength(hWnd);
		wchar_t* buffer = new wchar_t[len + 1];
		GetWindowText(hWnd, buffer, len);
		string ret = wstr_conv.to_bytes(buffer);
		delete[] buffer;
		return ret;
	});
}

LRESULT button_p::on_message(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_COMMAND:
		q->on_click.emit();
		break;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

button::button(const std::string& name, object* parent, klass* klass) : ui_object(new button_p(this, name, parent, klass))
{
	this->d = reinterpret_cast<button_p*>(_d);
	d->init();
}

button::button(button_p* d) : ui_object(d)
{
	this->d = reinterpret_cast<button_p*>(_d);
	d->init();
}

long button::HWND()
{
	return (long)d->hWnd;
}
