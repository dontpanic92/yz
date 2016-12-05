#include <iostream>
#include <memory>
#include <string>
#include <yz/klass.hpp>
#include <yz/object.hpp>
#include <yz/private/object_p.hpp>
#include <yz/util.hpp>

using namespace yz;

object& yz::ui = object::ui();

object_p::object_p(object* q, const std::string& name, object* parent, klass* klass) : _q(q), name(name), parent(parent), cls(klass)
{
}

object_p::~object_p()
{
	for (auto& it : _childrenNameMap) {
		delete it.second;
	}
}

void object_p::init()
{
	id = new_id();
}

LRESULT object_p::on_message(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return DefWindowProc(hWnd, message, wParam, lParam);
}

object::object(const string& name, klass* klass) : _d(new object_p(this, name, nullptr, klass))
{
	_d->init();
}

object::object(object_p* d) : _d(d)
{
	_d->init();
}

object::~object()
{
	delete _d;
}

object& object::ui()
{
	static object ui("global", &klass::cls());
	return ui;
}

string object::name() const 
{ 
	return _d->name; 
}

object* object::parent() const
{
	return _d->parent;
}

klass* object::type()
{
	return _d->cls;
}

const klass* object::type() const
{
	return _d->cls;
}

void object::add(object* child)
{
	if (_d->_childrenNameMap.find(child->name()) == _d->_childrenNameMap.end()) {
		_d->_childrenNameMap.insert(make_pair(child->name(), child));
		_d->_childrenIdMap.insert(make_pair(child->id(), child));
	} else {
		delete _d->_childrenNameMap[child->name()];
		_d->_childrenNameMap[child->name()] = child;
		_d->_childrenIdMap[child->id()] = child;
	}
}

bool object::find(const std::string& name) const
{
	return _d->_childrenNameMap.find(name) != _d->_childrenNameMap.end();
}

object::type_proxy object::operator[](const std::string& name)
{
	if (_d->_childrenNameMap.find(name) == _d->_childrenNameMap.end()) {
		throw std::out_of_range("Object key does not exist.");
	} else {
		return type_proxy{ _d->_childrenNameMap[name] };
	}
}

const map<string, object*>& object::children() const
{
	return _d->_childrenNameMap;
}

object::type_proxy object::get(const std::string& name)
{
	return type_proxy{ _d->_childrenNameMap[name] };
}

object::const_type_proxy object::get(const std::string& name) const
{
	return const_type_proxy{ _d->_childrenNameMap.at(name) };
}

int object::id()
{
	return _d->id;
}

object_p* object::d()
{
	return _d;
}