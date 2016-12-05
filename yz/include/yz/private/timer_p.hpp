#ifndef __YZ_TIMER_P_HPP__
#define __YZ_TIMER_P_HPP__

#include <yz/private/object_p.hpp>
#include <yz/timer.hpp>

namespace yz
{
	class timer_p : public object_p
	{
	private:
		timer* q;

	public:
		int interval;
		bool triggered_on_start;
		bool repeat;
		bool running;


		timer_p(timer* q, const std::string& name, object* parent, klass* klass);
		~timer_p();
		void init();
		void init_property();
		LRESULT on_message(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	};
}

#endif
