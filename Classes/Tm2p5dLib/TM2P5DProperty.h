#ifndef TM2P5D_PROPERTY_H
#define TM2P5D_PROPERTY_H

#include <string>

namespace TM2P5DComponent {

//Directory names
const std::string DIR_NAME_TM2P5D("tm2p5d/");
const std::string DIR_NAME_ATLAS("atlas/");
const std::string DIR_NAME_TERRAIN("terrain/");

//The orientation of a map
namespace Orientation {

	enum class Enum
	{
		PORTRAIT,
		LANDSCAPE,
		NONE,
	};

	/**
	 * Comvert an enum type to a string and return it.
	 * @method toString
	 * @param  type     [an enum type]
	 * @return          [a string made from an enum type]
	 */
	std::string toString(Enum type);

	/**
	 * Comvert a string to an enum type and return it.
	 * @method toEnum
	 * @param  str    [a string that indicate a certain enum type]
	 * @return        [an enum type]
	 */
	Enum toEnum(std::string str);

} /* namespace Orientation */

} /* namespace TM2P5DComponent */

#endif
