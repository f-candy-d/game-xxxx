#ifndef CHANK_H
#define CHANK_H

#include "../../cocos2d/cocos/cocos2d.h"
#include "TM2P5DProperty.h"

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
	 * @param  width       [The width of a chank]
	 * @param  height      [The height of a chank]
	 * @param  index       [An intager number that indicate a certain chank on the layer]
	 * @return             [Chank*]
	 */
	static Chank* create(size_t width,size_t height,int index);

	/**
	 * Insert a tile type into a array that contains tile types.
	 * @method insertType
	 * @param  type       [tile type]
	 * @param  x          [x-coordinate on the grid of a chank]
	 * @param  y          [y-coordinate on the grid of a chank]
	 */
	void insertType(int type,unsigned int x,unsigned int y);

	/**
	 * get a tile type at (x,y),then return it.
	 * @method getTypeAt
	 * @param  x         [x-coordinate on the grid of a chank]
	 * @param  y         [y-coordinate on the grid of a chank]
	 * @return           [tile type]
	 */
	int getTypeAt(unsigned int x,unsigned int y);

	/**
	 * Re-initialize a chank object.
	 * @method recycle
	 * @param  index   [new index]
	 * @return         [if index is < 0, return false]
	 */
	bool recycle(int index);

protected:
	Chank();
	~Chank();

	/**
	 * Initialize a Chank object.
	 * @method initWithSize
	 * @param  width       [The width of a chank]
	 * @param  height      [The height of a chank]
	 * @param  index       [An intager number that indicate a certain chank on the layer]
	 * @return             [true/false]
	 */
	bool initWithSize(size_t width,size_t height,int index);

private:
	/**
	 * An integer number that indecate a certaion chank on the layer.
	 */
	CC_SYNTHESIZE_READONLY(int,mIndex,Index);

	/**
	 * The width and height of a chank.
	 */
	CC_SYNTHESIZE_READONLY(size_t,mWidth,Width);
	CC_SYNTHESIZE_READONLY(size_t,mHeight,Height);

	/**
	 * Is a terrain in a chank modified or not.
	 */
	CC_SYNTHESIZE_READONLY(bool,mIsModified,IsModified);

	/**
	 * An array that contains types of tiles.
	 */
	// int* mTiles;
	CC_SYNTHESIZE_READONLY(int*,mTiles,Tiles);

	/**
	 * An vector that contains sprites of tiles in a parent node.
	 */
	// cocos2d::Vector<cocos2d::Sprite*> mSprites;
	CC_SYNTHESIZE_READONLY_PASS_BY_REF(cocos2d::Vector<cocos2d::Sprite*>,mSprites,Sprites);
};

} /* namespace TM2P5DComponent */

#endif
