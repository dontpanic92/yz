#ifndef __YZ_INITIALIZER_HPP__
#define __YZ_INITIALIZER_HPP__

#include <functional>
#include <vector>
#include <yz/object.hpp>

using namespace std;

namespace yz
{
	class initializers
	{
	public:
		static initializers& get()
		{
			static initializers instance;
			return instance;
		}

		void add_initializer(function<void()>& i)
		{
			_initializers.push_back(i);
		}

		void initialize()
		{
			for (auto& init : _initializers) 
			{
				init();
			}
		}
	private:
		initializers() {}
		initializers(const initializers&) = delete;
		initializers& operator=(const initializers&) = delete;

		vector<function<void()>> _initializers;
	};

	class initializer_register
	{
	public:
		initializer_register(function<void()> i)
		{
			initializers::get().add_initializer(i);
		}
	};
}

#endif
