#include "TM2P5DProperty.h"

using namespace TM2P5DComponent;

/**
 * Functions of namespace Orientation
 */

std::string Orientation::toString(Orientation::Enum type)
{
	switch (type)
	{
		case Enum::PORTRAIT:
			return std::string("portrait");

		case Enum::LANDSCAPE:
			return std::string("landscape");

		default :
			return std::string("");
	}
}

Orientation::Enum Orientation::toEnum(std::string str)
{
	if(str == "portrait")
		return Orientation::Enum::PORTRAIT;
	else if(str == "landscape")
		return Orientation::Enum::LANDSCAPE;
	else
		return Orientation::Enum::NONE;
}
