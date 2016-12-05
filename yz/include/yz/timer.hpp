#ifndef __YZ_TIMER_HPP__
#define __YZ_TIMER_HPP__

#include <yz/object.hpp>
#include <yz/property.hpp>
#include <yz/signal.hpp>

namespace yz
{
	class timer_p;
	class timer : public object
	{
	public:
		timer(const std::string& name, object* parent, klass* klass);

		property<int> interval = 1000;
		property<bool> triggered_on_start = true;
		property<bool> repeat = true;
		property<bool> running = false;

		signal<void()> on_timer;

		void start();
		void stop();

	protected:
		timer(timer_p* d);
	private:
		timer_p* d;
	};
}

#endif
