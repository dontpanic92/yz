#ifndef __YZ_HAS_CHILD_HPP__
#define __YZ_HAS_CHILD_HPP__

#include <string>
#include <map>
#include <iostream>
#include <memory>

namespace yz
{
	using namespace std;

	template <class T>
	class has_child
	{
	public:

		struct type_proxy
		{
			T* p;
			T* operator->() { return p; }
			operator T&() { return *p; }

			template <class U>
			U* operator->() { return dynamic_cast<U*>(p); }

			template <class U>
			operator U&() { return *dynamic_cast<U*>(p); }

			template <class U>
			operator U*() { return dynamic_cast<U*>(p); }
		};

		struct const_type_proxy
		{
			const T* p;
			const T* operator->() { return p; }
			operator const T&() { return *p; }

			template <class U>
			const U* operator->() { return dynamic_cast<U*>(p); }

			template <class U>
			operator const U&() { return *dynamic_cast<U*>(p); }

			template <class U>
			operator const U*() { return dynamic_cast<U*>(p); }
		};

		virtual ~has_child()
		{
			for (auto p : _children) {
				delete p.second;
			}
		}

		void add(T* child)
		{
			if (_children.find(child->name()) == _children.end()) {
				_children.insert(make_pair(child->name(), child));
			} else {
				delete _children[child->name()];
				_children[child->name()] = child;
			}
		}

		bool find(const std::string& name) const
		{
			return _children.find(name) != _children.end();
		}

		type_proxy operator[](const std::string& name)
		{
			if (_children.find(name) == _children.end()) {
				throw std::out_of_range("Object key does not exist.");
			} else {
				return type_proxy{ _children[name] };
			}
		}

		type_proxy get(const std::string& name)
		{
			return type_proxy{ _children[name] };
		}

		const_type_proxy get(const std::string& name) const
		{
			return const_type_proxy{ _children.at(name) };
		}

		template <class U>
		U* get(const std::string& name)
		{
			return dynamic_cast<U*>(_children[name]);
		}

		typename std::map<std::string, T*>::const_iterator begin() const { return _children.cbegin(); }
		typename std::map<std::string, T*>::const_iterator end() const { return _children.cend(); }
		const std::map<std::string, T*>& children() const { return _children; }

	protected:
		std::map<std::string, T*> _children;
	};

}
#endif
