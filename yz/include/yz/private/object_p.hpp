#ifndef __YZ_OBJECT_P_HPP__
#define __YZ_OBJECT_P_HPP__

#include <string>
#include <yz/object.hpp>
#include <Windows.h>

namespace yz
{
	class object_p
	{
	public:
		object* _q;

		std::string name;
		object* parent;
		klass* cls;
		int id;


		map<string, object*> _childrenNameMap;
		map<int, object*> _childrenIdMap; 
		
		virtual LRESULT on_message(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

		object_p(object* q, const std::string& name, object* parent, klass* klass);
		void init();
		virtual ~object_p();

		template <class T>
		T* down_to() { return dynamic_cast<T*>(this); }
	};
}

#endif