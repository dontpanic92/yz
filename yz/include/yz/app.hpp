#ifndef __YZ_APP_HPP__
#define __YZ_APP_HPP__
#include <Windows.h>

namespace yz {
	class app
	{
	public:
		app();
		~app();

		int exec();

		HINSTANCE hInstance();

		static app* get(); 

	protected:
		//void init_object(const klass_base* kls, object* obj, std::set<std::string>& instantiatedObjects);
		void init_all_object();
		static app* _app;
	};
}

#endif