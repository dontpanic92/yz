#ifndef __YZ_UI_OBJECT_HPP__
#define __YZ_UI_OBJECT_HPP__

#include <yz/object.hpp>
#include <yz/property.hpp>

namespace yz
{
	class ui_object_p;
	class ui_object : public object
	{
	public:
		ui_object(const std::string& name, object* parent, klass* klass);

		property<int> x = 10;
		property<int> y = 10;

		virtual long HWND() = 0;

	protected:
		ui_object(ui_object_p* d);

	private:
		ui_object_p* d;
		ui_object(const ui_object&) = delete;
	};
}

#endif
