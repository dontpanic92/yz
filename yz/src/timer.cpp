#include <Windows.h>
#include <yz/app.hpp>
#include <yz/util.hpp>
#include <yz/window.hpp>
#include <yz/private/timer_p.hpp>
#include <yz/private/ui_object_p.hpp>

using namespace yz;
using namespace std;

klass timer_base = klass_define_builder<window>("timer");


timer_p::timer_p(timer* q, const std::string& name, object* parent, klass* klass) : object_p(q, name, parent, klass)
{
	this->q = q;
}

timer_p::~timer_p()
{
	q->stop();
}

void timer_p::init()
{
	init_property();
	q->on_created += [&](){
		if (triggered_on_start) {
			q->start();
		}
	};
}

void timer_p::init_property()
{
	q->interval.getter([&](){
		return interval;
	}).setter([&](const int& value){
		interval = value;
	});

	q->triggered_on_start.getter([&](){
		return triggered_on_start;
	}).setter([&](const bool & value){
		triggered_on_start = value;
	});

	q->repeat.getter([&](){
		return repeat;
	}).setter([&](const bool& value) {
		repeat = value;
	});

	q->running.getter([&](){
		return running;
	});
}

LRESULT timer_p::on_message(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	q->on_timer.emit();
	if (!repeat) {
		q->stop();
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

timer::timer(const std::string& name, object* parent, klass* klass) :object(new timer_p(this, name, parent, klass))
{
	d = reinterpret_cast<timer_p*>(_d);
	d->init();
}

timer::timer(timer_p* d) :object(d)
{
	this->d = d;
	d->init();
}

void timer::start()
{
	if (ui_object_p* p = dynamic_cast<ui_object_p*>(d->parent->d())) {
		SetTimer(p->hWnd, d->id, interval.value(), nullptr);
		d->running = true;
	}
}

void timer::stop()
{
	if (ui_object_p* p = dynamic_cast<ui_object_p*>(d->parent->d())) {
		KillTimer(p->hWnd, d->id);
		d->running = false;
	}
}