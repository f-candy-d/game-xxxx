#ifndef TM2P5D_PROPERTY_H
#define TM2P5D_PROPERTY_H

#include <string>

namespace TM2P5DComponent {

//Directory names
const std::string DIR_NAME_TM2P5D("tm2p5d/");
const std::string DIR_NAME_ATLAS("atlas/");
const std::string DIR_NAME_TERRAIN("terrain/");

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
