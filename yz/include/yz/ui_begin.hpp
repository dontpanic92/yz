#ifndef __YZ_UI_BEGIN_HPP__
#define __YZ_UI_BEGIN_HPP__

#include <yz/ui.hpp>
#include <yz/klass.hpp>

#define QUOTE_(x) #x
#define QUOTE(x) QUOTE_(x)

#define yz_object(base_class, class_name) \
	klass class_name##_obj = klass_builder<base_class, remove_reference_t<decltype(self)>>(QUOTE(base_class), QUOTE(class_name), &self) + [&](decltype(self) parent, base_class& self)

#define yz_define(base_class, class_name) \
	typedef base_class class_name; \
	klass class_name##_obj = klass_define_builder<base_class>(QUOTE(base_class), QUOTE(class_name)) + [&](object& parent, base_class& self)



#define yz_object_with_members(base_class, class_name) \
	klass class_name##_obj = klass_builder<class_name, remove_reference_t<decltype(self)>>(QUOTE(base_class), QUOTE(class_name), &self) + [&](decltype(self) parent, class_name& self)


#define yz_define_with_members(base_class, class_name) \
	klass class_name##_obj = klass_define_builder<class_name>(QUOTE(base_class), QUOTE(class_name)) + [&](object& parent, class_name& self)

#define yz_property static

namespace yz
{
	static object& self = object::ui();

#endif
