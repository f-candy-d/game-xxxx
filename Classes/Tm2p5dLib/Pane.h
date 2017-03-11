#ifndef CHANK_H
#define CHANK_H

#include "../../cocos2d/cocos/cocos2d.h"
#include "TM2P5DProperty.h"

/**
 * Pane class is a member of the namespace 'TM2P5DComponent'.
 */
namespace TM2P5DComponent {

class Pane : public cocos2d::Ref
{
public:
	/**
	 * Create Pane object.
	 * @method create
	 * @param  width       [The width of a pane]
	 * @param  height      [The height of a pane]
	 * @param  index       [An intager number that indicate a certain pane on the layer]
	 * @return             [Pane*]
	 */
	static Pane* create(size_t width,size_t height,int index);

	/**
	 * Insert a tile type into a array that contains tile types.
	 * @method insertType
	 * @param  type       [tile type]
	 * @param  x          [x-coordinate on the grid of a pane]
	 * @param  y          [y-coordinate on the grid of a pane]
	 */
	void insertType(int type,unsigned int x,unsigned int y);

	/**
	 * get a tile type at (x,y),then return it.
	 * @method getTypeAt
	 * @param  x         [x-coordinate on the grid of a pane]
	 * @param  y         [y-coordinate on the grid of a pane]
	 * @return           [tile type]
	 */
	int getTypeAt(unsigned int x,unsigned int y);

	/**
	 * Re-initialize a pane object.
	 * @method recycle
	 * @param  index   [new index]
	 * @return         [if index is < 0, return false]
	 */
	bool recycle(int index);

	/**
	 * Add a sprite to the back of a vector.
	 * @method addSprite
	 * @param  sprite    [sprite object]
	 */
	void addSprite(cocos2d::Sprite* sprite);

protected:
	Pane();
	~Pane();

	/**
	 * Initialize a Pane object.
	 * @method initWithSize
	 * @param  width       [The width of a pane]
	 * @param  height      [The height of a pane]
	 * @param  index       [An intager number that indicate a certain pane on the layer]
	 * @return             [true/false]
	 */
	bool initWithSize(size_t width,size_t height,int index);

private:
	/**
	 * An integer number that indecate a certaion pane on the layer.
	 */
	CC_SYNTHESIZE_READONLY(int,mIndex,Index);

	/**
	 * The width and height of a pane.
	 * These are must be 2^n.
	 * If the orientation of a map is portrait,the width must be larger than the height,if it is landscape,the height must be larger than the width.
	 */
	CC_SYNTHESIZE_READONLY(size_t,mWidth,Width);
	CC_SYNTHESIZE_READONLY(size_t,mHeight,Height);

	/**
	 * Is a terrain in a pane modified or not.
	 */
	CC_SYNTHESIZE_READONLY(bool,mIsModified,IsModified);

	/**
	 * Is a pane object is a null state or not.
	 */
	CC_SYNTHESIZE(bool,mIsNullState,IsNullState);

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
