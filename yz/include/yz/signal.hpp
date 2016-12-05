#ifndef __YZ_SIGNAL_HPP__
#define __YZ_SIGNAL_HPP__

#include <vector>
#include <functional>
using namespace std;

namespace yz
{
	template <class FunctionT>
	class signal
	{
	public:
		signal<FunctionT>& operator +=(function<FunctionT> slot)
		{
			connect(slot);
			return *this;
		}

		void connect(function<FunctionT> slot)
		{
			_slots.push_back(slot);
		}

		template <class... T>
		void emit(T&& ...args)
		{
			for (auto& slot : _slots) {
				slot(forward<T>(args)...);
			}
		}

	private:
		vector<function<FunctionT>> _slots;
	};
}

#endif
