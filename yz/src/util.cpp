#include <yz/util.hpp>
#include <string>
using namespace yz;
namespace yz
{
	static int id = 0x1000;

	int new_id()
	{
		id++;
		return id;
	}
}