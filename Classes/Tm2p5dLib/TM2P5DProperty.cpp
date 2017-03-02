#include "TM2P5DProperty.h"

using namespace TM2P5DComponent;

/**
 * Functions of namespace Orientation
 */

std::string Orientation::toString(Orientation::Type type)
{
	switch (type)
	{
		case PORTRAIT:
			return std::string("portrait");

		case LANDSCAPE:
			return std::string("landscape");

		default :
			return std::string("none");
	}
}

Orientation::Type Orientation::toType(std::string str)
{
	if(str == "portrait")
		return Orientation::PORTRAIT;
	else if(str == "landscape")
		return Orientation::LANDSCAPE;
	else
		return Orientation::NONE;
}
