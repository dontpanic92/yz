#include <iostream>
#include <set>
#include <string>
#include <stdexcept>

#include <Windows.h>
#include <yz/app.hpp>
#include <yz/ui.hpp>
#include <yz/klass.hpp>
#include <yz/initializer.hpp>

using namespace std;
using namespace yz;

app* app::_app = nullptr;

app::app()
{
	if (_app) {
		throw runtime_error("More than one yz::app initialized!");
	}
	_app = this;
	initializers::get().initialize();
	init_all_object();
}

app::~app()
{
	_app = nullptr;
}

app* app::get()
{
	return _app;
}

HINSTANCE app::hInstance()
{
	return  GetModuleHandle(NULL);
}

void init_object(klass* kls)
{
	if (!kls->builder()->create_object())
		return;

	yz::object* obj = kls->create(kls->name());
	for (auto child : *kls) {
		init_object(kls->get(child.second->name()));
	}
}

void app::init_all_object()
{
	for (auto& k : cls) {
		init_object(k.second);
	}
}

int app::exec()
{
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0) > 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}
