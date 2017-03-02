#ifndef CHANK_H
#define CHANK_H

#include "../../cocos2d/cocos/cocos2d.h"

/**
 * Chank class is a member of the namespace 'TM2P5DComponent'.
 */
namespace TM2P5DComponent {

class Chank : public cocos2d::Ref
{
public:
	/**
	 * Create Chank object.
	 * @method create
	 * @param  width    [The width of a chank]
	 * @param  height   [The height of a chank]
	 * @param  index    [An intager number that indicate a certain chank on the layer]
	 * @param  tileSize [The size of a tile texture]
	 * @return          [Chank*]
	 */
	static Chank* create(size_t width,size_t height,int index,cocos2d::Size tileSize);

protected:
	Chank();
	~Chank();

	/**
	 * Initialize a Chank object.
	 * @method initWithParam
	 * @param  width    [The width of a chank]
	 * @param  height   [The height of a chank]
	 * @param  index    [An intager number that indicate a certain chank on the layer]
	 * @param  tileSize [The size of a tile texture]
	 * @return          [true/false]
	 */
	bool initWithParam(size_t width,size_t height,int index,cocos2d::Size tileSize);
};

} /* namespace TM2P5DComponent */

#endif
