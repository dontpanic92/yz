#include <yz/klass.hpp>
#include <yz/object.hpp>

using namespace yz;

klass& yz::cls = klass::cls();

klass::klass(klass_builder_base& builder)
{
	_builder = builder.duplicate();
	_builder->set_klass(this);
	if (_builder->parent())
		_builder->parent()->type()->add(new klass(*this));
}

std::string klass::name() const
{
	return _builder->name();
}
