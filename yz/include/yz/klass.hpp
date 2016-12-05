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
		klass_builder_base(const string& name, object* parent, bool create_object) 
			: _name(name),  _parent(parent), _create_object(create_object)
		{
		}

		// Copy from a base class
		klass_builder_base(const string& basename, const string& name, object* parent, bool create_object)
			: _basename(basename), _name(name), _parent(parent), _create_object(create_object)
		{
		}

		void set_klass(klass* kls) { _kls = kls; }

		virtual object* create(const string& name) const = 0;
		virtual void construct_object(object*) const {};

		virtual klass_builder_base* duplicate() const = 0;

		string name() const { return _name; }
		string basename() const { return _basename; }
		object* parent() { return _parent; }
		bool create_object() const { return _create_object; }

	protected:

		string _name;
		string _basename;
		bool _create_object;
		object* _parent;
		klass* _kls;
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
			object* p = _builder->create(name);
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

	template <class T, class ParentT>
	class klass_builder : public klass_builder_base
	{
	public:
		typedef void(constructor_func)(ParentT& parent, T& self);
	
		klass_builder(const string& name, ParentT* parent, bool create_object) 
			: klass_builder_base(name, parent, create_object) {}
		klass_builder(const string& basename, const string& name, ParentT* parent, bool create_object)
			: klass_builder_base(basename, name, parent, create_object) {}

		klass_builder<T, ParentT> operator+(function<constructor_func> f)
		{
			_constructor = f;
			return *this;
		}

		object* create(const string& name) const override
		{
			ParentT* parent = dynamic_cast<ParentT*>(_parent);
			T* p = new T(name, parent, _kls);
			construct_object(p);
			parent->add(p);
			p->on_created.emit();
			return p;
		}

		klass_builder_base* duplicate() const override
		{
			return new klass_builder<T, ParentT>(*this);
		}
	protected:
		void construct_object(object* p) const
		{
			if (!_basename.empty()) {
				//const klass* kls = _kls->outer_klass();
				const klass* kls = _parent->type();
				while (true) {
					if (kls->find(_basename)) {
						kls->get(_basename)->builder()->construct_object(p);
						break;
					} else if (!_parent->parent()){
						break;
					} else {
						kls = _parent->parent()->type();
					}
				}
			}
			if (_constructor) {
				T* t = dynamic_cast<T*>(p);
				ParentT* parent = dynamic_cast<ParentT*>(_parent);
				_constructor(*parent, *t);
			}
		}

		function<constructor_func> _constructor;
	};
	
}
#endif