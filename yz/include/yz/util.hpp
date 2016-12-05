#ifndef __YZ_UTIL_HPP__
#define __YZ_UTIL_HPP__

namespace yz
{
	int new_id();

#define yz_properties \
	template <int N, class Type> \
	struct user_property {}; \
	struct user_properties


#define yz_property(type, x) \
	template <__LINE__, class Type> \
	struct user_property<__LINE__, Type> \
	{ \
		Type self; \
	};

}

#endif