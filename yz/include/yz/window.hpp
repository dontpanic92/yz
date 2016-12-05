#ifndef __YZ_WINODW_HPP__
#define __YZ_WINODW_HPP__

#include <string>
#include <memory>
#include <yz/klass.hpp>
#include <yz/ui_object.hpp>
#include <yz/property.hpp>

namespace yz
{
	using namespace std;

	class window_p;
	class window : public ui_object
	{
	public:
		window(const std::string& name, object* parent, klass* klass);

		void show();
		long HWND() override;

		property<string> title = "Hello world";

	protected:
		window(window_p* d);
	private:
		window_p* d;
	};
}

#endif