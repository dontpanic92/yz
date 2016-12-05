#ifndef __YZ_BUTTON_HPP__
#define __YZ_BUTTON_HPP__

#include <string>
#include <memory>
#include <yz/has_child.hpp>
#include <yz/klass.hpp>
#include <yz/ui_object.hpp>
#include <yz/property.hpp>
#include <yz/signal.hpp>

namespace yz
{
	using namespace std;

	class button_p;
	class button : public ui_object
	{
	public:
		button(const std::string& name, object* parent, klass* klass);

		long HWND() override;
		
		property<string> text = "OK";

		signal<void()> on_click;

	protected:
		button(button_p* d);
	private:
		button_p* d;
	};
}

#endif