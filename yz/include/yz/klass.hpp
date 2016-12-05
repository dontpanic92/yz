#ifndef __YZ_KLASS_HPP__
#define __YZ_KLASS_HPP__

#include <functional>
#include <vector>
#include <string>
#include <memory>
#include <yz/has_child.hpp>

namespace yz
{
	using namespace std;

	class object;
	class klass;
	class klass_builder_base
	{
	public:

		// A new class
		klass_builder_base(const string& name, object* parent) 
			: _name(name),  _parent(parent)
		{
		}

		// Copy from a base class
		klass_builder_base(const string& basename, const string& name, object* parent)
			: _basename(basename), _name(name), _parent(parent)
		{
		}

		void set_klass(klass* kls) { _kls = kls; }

		virtual klass_builder_base* duplicate() const = 0;

		string name() const { return _name; }
		string basename() const { return _basename; }
		object* parent() { return _parent; }

	protected:

		string _name;
		string _basename;
		object* _parent;
		klass* _kls;
	};


	class klass_define_builder_base : public klass_builder_base
	{
	public:
		klass_define_builder_base(const string& name) : klass_builder_base(name, nullptr)
		{
		}

		klass_define_builder_base(const string& basename, const string& name)
			: klass_builder_base(basename, name, nullptr)
		{
		}
		virtual void construct_base(object*, object*) const = 0;
	};


	class klass_object_builder_base : public klass_builder_base
	{
	public:
		klass_object_builder_base(const string& name, object* parent) : klass_builder_base(name, parent)
		{
		}

		klass_object_builder_base(const string& basename, const string& name, object* parent)
			: klass_builder_base(basename, name, parent)
		{
		}

		virtual object* create(const string& name) const = 0;
		virtual void construct_object(object*) const = 0;

	};

	class klass :public has_child<klass>
	{
	public:
		klass(klass_builder_base& builder);

		~klass()
		{
			delete _builder;
		}

		string name() const;

		object* create(const string& name)
		{
			object* p = dynamic_cast<klass_object_builder_base*>(_builder)->create(name);
			return p;
		}

		const klass_builder_base* builder() const { return _builder; }

		//klass* outer_klass() { return _outer_klass; }
		//const klass* outer_klass() const { return _outer_klass; }

		template <class U>
		operator U&() { return *dynamic_cast<U*>(this); }

		template <class U>
		operator U*() { return dynamic_cast<U*>(this); }

		static klass& cls()
		{
			static klass c;
			return c;
		}

	protected:
		klass() 
		{
			_builder = nullptr; 
		}

		klass(const klass& k)
		{
			_builder = k._builder->duplicate();
			_builder->set_klass(this);
			//_outer_klass = k._outer_klass;
		}

		klass_builder_base* _builder;
	};

	extern klass& cls;

	template <class T>
	class klass_define_builder : public klass_define_builder_base
	{
	public:
		typedef void(constructor_func)(object& parent, T& self);
		klass_define_builder(const string& name) : klass_define_builder_base(name)
		{
		}

		klass_define_builder(const string& basename, const string& name)
			: klass_define_builder_base(basename, name)
		{
		}

		klass_builder_base* duplicate() const
		{
			return new klass_define_builder<T>(*this);
		}

		klass_define_builder<T> operator+(function<constructor_func> f)
		{
			_constructor = f;
			return *this;
		}

		void construct_base(object* p, object* pr) const override
		{
			if (!_basename.empty() && cls.find(_basename)) {
				const klass_define_builder_base* k = dynamic_cast<const klass_define_builder_base*>(cls.get(_basename)->builder());
				k->construct_base(p, pr);
			}
			if (_constructor) {
				T* t = dynamic_cast<T*>(p);
				_constructor(*pr, *t);
			}
		}
		function<constructor_func> _constructor;
	};


	template <class T, class ParentT>
	class klass_builder : public klass_object_builder_base
	{
	public:
		typedef void(constructor_func)(ParentT& parent, T& self);
	
		klass_builder(const string& name, ParentT* parent) 
			: klass_object_builder_base(name, parent)
		{
		}
		klass_builder(const string& basename, const string& name, ParentT* parent)
			: klass_object_builder_base(basename, name, parent)
		{
		}

		klass_builder<T, ParentT> operator+(function<constructor_func> f)
		{
			_constructor = f;
			return *this;
		}

		object* create(const string& name) const override
		{
			ParentT* parent = dynamic_cast<ParentT*>(_parent);
			T* p = new T(name, parent, _kls);
			parent->add(p);
			construct_object(p);
			p->on_created.emit();
			return p;
		}

		klass_builder_base* duplicate() const override
		{
			return new klass_builder<T, ParentT>(*this);
		}
		void construct_object(object* p) const override
		{
			if (!_basename.empty()) {
				const klass* kls = _parent->type();
				object* pr = _parent;
				while (true) {
					if (kls->find(_basename)) {
						const klass_define_builder_base* k = dynamic_cast<const klass_define_builder_base*>(kls->get(_basename)->builder());
						k->construct_base(p, pr);
						break;
					} else if (!pr->parent()) {
						break;
					} else {
						kls = pr->parent()->type();
						pr = pr->parent();
					}
				}
			}
			if (_constructor) {
				T* t = dynamic_cast<T*>(p);
				ParentT* parent = dynamic_cast<ParentT*>(_parent);
				_constructor(*parent, *t);
			}
		}
	protected:
		function<constructor_func> _constructor;
	};
	
}
#endif