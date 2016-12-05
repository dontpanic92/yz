#ifndef __YZ_OBJECT_HPP__
#define __YZ_OBJECT_HPP__

#include <string>
#include <memory>
#include <yz/has_child.hpp>
#include <yz/klass.hpp>
#include <yz/signal.hpp>

namespace yz
{
	using namespace std;

	class object_p;
	class object
	{
	public:

		struct type_proxy
		{
			object* p;
			object* operator->() { return p; }
			operator object&() { return *p; }
			type_proxy operator [](const std::string& name)
			{
				return p->operator[](name);
			}


			template <class U>
			U* operator->() { return dynamic_cast<U*>(p); }

			template <class U>
			operator U&() { return *dynamic_cast<U*>(p); }

			template <class U>
			operator U*() { return dynamic_cast<U*>(p); }
		};

		struct const_type_proxy
		{
			const object* p;
			const object* operator->() { return p; }
			operator const object&() { return *p; }

			template <class U>
			const U* operator->() { return dynamic_cast<U*>(p); }

			template <class U>
			operator const U&() { return *dynamic_cast<U*>(p); }

			template <class U>
			operator const U*() { return dynamic_cast<U*>(p); }
		};

		void add(object* child);

		bool find(const std::string& name) const;

		type_proxy operator[](const std::string& name);

		type_proxy get(const std::string& name);

		const_type_proxy get(const std::string& name) const;

		const map<string, object*>& children() const;

		object(const std::string& name, klass* klass);

		virtual ~object();

		std::string name() const;
		object* parent() const;
		klass* type();
		const klass* type() const;

		int id();

		template <class T>
		T* down_to() { return dynamic_cast<T*>(this); }

		static object& ui();

		object_p* d();
		signal<void()> on_created;
	protected:
		object_p* _d;
		object(object_p* d);


		friend class klass_builder_base;
	};

	extern object& ui;
}

#endif