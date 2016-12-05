#include <yz/ui_object.hpp>
#include <yz/private/ui_object_p.hpp>

using namespace yz;


ui_object_p::ui_object_p(ui_object* q, const std::string& name, object* parent, klass* klass) : object_p(q, name, parent, klass)
{
	this->q = q;
}

void ui_object_p::init()
{
	init_property();
}

void ui_object_p::init_property()
{
	q->x.setter([&](int value){
		RECT rc;
		GetWindowRect(hWnd, &rc);
		POINT p = { rc.left, rc.top };
		if (ui_object_p* ui_parent = parent->d()->down_to<ui_object_p>())
			ScreenToClient(ui_parent->hWnd, &p);
		SetWindowPos(hWnd, 0, value, p.y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
	}).getter([&](){
		RECT rc;
		GetWindowRect(hWnd, &rc);
		POINT p = { rc.left, rc.top };
		if (ui_object_p* ui_parent = parent->d()->down_to<ui_object_p>())
			ScreenToClient(ui_parent->hWnd, &p);
		return p.x;
	});

	q->y.setter([&](int value){
		RECT rc;
		GetWindowRect(hWnd, &rc);
		POINT p = { rc.left, rc.top };
		if (ui_object_p* ui_parent = parent->d()->down_to<ui_object_p>())
			ScreenToClient(ui_parent->hWnd, &p);
		SetWindowPos(hWnd, 0, p.x, value, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
	}).getter([&](){
		RECT rc;
		GetWindowRect(hWnd, &rc);
		POINT p = { rc.left, rc.top };
		if (ui_object_p* ui_parent = parent->d()->down_to<ui_object_p>())
			ScreenToClient(ui_parent->hWnd, &p);
		return p.y;
	});
}

LRESULT ui_object_p::on_message(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return DefWindowProc(hWnd, message, wParam, lParam);
}

ui_object::ui_object(const std::string& name, object* parent, klass* klass) : object(new ui_object_p(this, name, parent, klass))
{
	d = reinterpret_cast<ui_object_p*>(_d);
	d->init();
}

ui_object::ui_object(ui_object_p* d) : object(d)
{
	this->d = d;
	d->init();
}