#ifndef __YZ_DECL_BEGIN_HPP__
#define __YZ_DECL_BEGIN_HPP__

#define yz_declare(base_class, class_name) \
	typedef base_class class_name;

#define yz_declare_with_members_begin(base_class, class_name) \
	struct class_name: public base_class { \
		class_name(const std::string& name, object * parent, klass* kls) : \
			base_class(name, parent, kls) {}

#define yz_declare_with_members_end \
	}

#endif