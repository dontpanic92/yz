#ifndef __YZ_PROPERTY_HPP__
#define __YZ_PROPERTY_HPP__

#include <string>
#include <functional>
#include <stdexcept>
#include <yz/has_child.hpp>
#include <yz/klass.hpp>

namespace yz
{
	using namespace std;
	template <class T>
	class property
	{
	public:
		property(const T& v) : _default(v) {}

		property<T>& operator=(const T& rhs)
		{
			if (_setter) {
				_setter(rhs);
			} else {
				throw runtime_error("property is read-only");
			}
			return *this;
		}

		T value()
		{
			if (!_getter) {
				throw runtime_error("property is write-only");
			}
			return _getter();
		}

		T default()
		{
			return _default;
		}

		operator T()
		{
			if (!_getter) {
				throw runtime_error("property is write-only");
			}
			return _getter();
		}

		property<T>& setter(function<void(const T& new_value)> set)
		{
			_setter = set;
			_setter(_default);
			return *this;
		}

		property<T>& getter(function<T()> get)
		{
			_getter = get;
			return *this;
		}

	protected:
		T _default;
		function<void(const T& new_value)> _setter;
		function<T()> _getter;
	};
}
#endif
