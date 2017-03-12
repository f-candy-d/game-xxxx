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

	typedef enum
	{
		PORTRAIT,
		LANDSCAPE,
		NONE,
	}
	Type;

	/**
	 * Comvert an enum type to a string and return it.
	 * @method toString
	 * @param  type     [an enum type]
	 * @return          [a string made from an enum type]
	 */
	std::string toString(Type type);

	/**
	 * Comvert a string to an enum type and return it.
	 * @method toType
	 * @param  str    [a string that indicate a certain enum type]
	 * @return        [an enum type]
	 */
	Type toType(std::string str);

} /* namespace Orientation */

//The direction of loading chanks.
enum class LoadDirection
{
	DIRECTION_BEGIN,
	DIRECTION_END,
};

//Process state of Pane object
enum class State
{
	// Not a null object,but it has useless data
	ZOMBIE,
	// Staged in a layer object and drawn on the screen
	STAGED,
	// Staged in a layer object and wating to be drawn
	WATING_ALLOCATION,
};

// The direction of split of the map
enum class Split
{
	HORIZONTAL_SPLIT,
	VERTICAL_SPLIT,
};

} /* namespace TM2P5DComponent */

#endif
