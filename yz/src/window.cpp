#include <Windows.h>
#include <codecvt>
#include <stdexcept>
#include <yz/app.hpp>
#include <yz/util.hpp>
#include <yz/window.hpp>
#include <yz/private/window_p.hpp>

using namespace yz;
using namespace std;

klass window_base = klass_define_builder<window>("window");


window_p::window_p(window* q, const std::string& name, object* parent, klass* klass) : ui_object_p(q, name, parent, klass)
{
	this->q = reinterpret_cast<window*>(_q);
}

void window_p::init()
{
	id = new_id();
	wname = wstr_conv.from_bytes(q->name());
	hInstance = app::get()->hInstance();
	if (!register_window_class()) {
		throw runtime_error("Failed to register window class");
	}
	hWnd = CreateWindow(wcx.lpszClassName, TEXT("Hello world!"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 640, 480, 0, 0, hInstance, this);
	if (!hWnd) {
		throw runtime_error("Failed to create window");
	}
	init_property();
}

ATOM window_p::register_window_class()
{
	memset(&wcx, 0, sizeof(wcx));
	wcx.cbSize = sizeof(wcx);
	wcx.style = CS_HREDRAW | CS_VREDRAW;
	wcx.lpfnWndProc = static_wnd_proc;
	wcx.hInstance = hInstance;
	wcx.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcx.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcx.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wcx.lpszClassName = (wname + L"_yzwindow").c_str();
	return RegisterClassEx(&wcx);
}

void window_p::init_property()
{
	q->title.setter([&](const string& new_value){
		wstring wstr = wstr_conv.from_bytes(new_value);
		SetWindowText(hWnd, wstr.c_str());
	}).getter([&](){
		int len = GetWindowTextLength(hWnd);
		wchar_t* buffer = new wchar_t[len + 1];
		GetWindowText(hWnd, buffer, len);
		string ret = wstr_conv.to_bytes(buffer);
		delete[] buffer;
		return ret;
	});
}

LRESULT window_p::on_message(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_COMMAND:
		{
			int id = LOWORD(wParam);
			if (_childrenIdMap.find(id) != _childrenIdMap.end()) {
				return dynamic_cast<ui_object_p*>(_childrenIdMap[id]->d())->on_message(hWnd, message, wParam, lParam);
			}
		}
		break;
	case WM_TIMER:
		{
			int id = wParam;
			if (_childrenIdMap.find(id) != _childrenIdMap.end()) {
				return _childrenIdMap[id]->d()->on_message(hWnd, message, wParam, lParam);
			}
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

LRESULT CALLBACK window_p::static_wnd_proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// https://blogs.msdn.microsoft.com/oldnewthing/20140203-00/?p=1893/

	window_p* p;
	if (message == WM_NCCREATE) {
		LPCREATESTRUCT lpcs = reinterpret_cast<LPCREATESTRUCT>(lParam);
		p = static_cast<window_p*>(lpcs->lpCreateParams);
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(p));
	} else {
		p = reinterpret_cast<window_p*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	}
	if (p) {
		return p->on_message(hWnd, message, wParam, lParam);
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}


window::window(const std::string& name, object* parent, klass* klass) : ui_object(new window_p(this, name, parent, klass))
{
	d = reinterpret_cast<window_p*>(_d);
	d->init();
}

window::window(window_p* d) : ui_object(d)
{
	this->d = d;
	d->init();
}

void window::show()
{
	ShowWindow(d->hWnd, SW_SHOW);
}

long window::HWND()
{
	return (long)d->hWnd;
}
