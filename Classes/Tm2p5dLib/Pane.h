#ifndef PANE_H
#define PANE_H

#include "../../cocos2d/cocos/cocos2d.h"
#include "TM2P5DProperty.h"
/**
 * Pane class and SubPane class is a member of the namespace 'TM2P5DComponent'.
 */
namespace TM2P5DComponent {

/**
 * declaration of SubPane class
 */
class SubPane :public cocos2d::Ref
{
public:
	static SubPane* create(int index,size_t capacity);
	SubPane();
	// SubPane(int idx,size_t capa = 0);
	// SubPane(const SubPane& other);
	// SubPane& operator=(const SubPane& other) &;

	/**
	 * getter and setter functions for mOwnSprites.
	 * @return [description]
	 */
	bool ownSprites();
	void ownSprites(bool has);

	/**
	 * getter functions for vectors.
	 */
	std::vector<int>& getTiles() {return mTiles;}
	cocos2d::Vector<cocos2d::Sprite*>& getSprites() {return mSprites;}

private:
	CC_SYNTHESIZE(int,mIndex,Index);
	CC_SYNTHESIZE_READONLY(size_t,mSize,Size);
	bool mOwnSprites;
	std::vector<int> mTiles;
	cocos2d::Vector<cocos2d::Sprite*> mSprites;

	bool init(int index,size_t capacity);
};

/**
 * declaration of Pane class
 */
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
	static Pane* create(size_t width,size_t height,int index,int numSubPane,Split split);

	/**
	 * Insert a tile type into a array that contains tile types.
	 * @method insertTypeAt
	 * @param  type       [tile type]
	 * @param  x          [x-coordinate on the grid of a pane]
	 * @param  y          [y-coordinate on the grid of a pane]
	 */
	void insertTypeAt(int type,int x,int y);
	void insertTypeAt(int type,int index);

	/**
	 * get a tile type at (x,y),then return it.
	 * @method getTypeAt
	 * @param  x         [x-coordinate on the grid of a pane]
	 * @param  y         [y-coordinate on the grid of a pane]
	 * @return           [tile type]
	 */
	int getTypeAt(int x,int y);
	int getTypeAt(int index);

	/**
	 * Re-initialize a pane object.
	 * @method recycle
	 * @param  index   [new index]
	 * @return         [if index is < 0, return false]
	 */
	bool recycle(int index);

	// NOTE :: useless
	/**
	 * Add a sprite to the back of a vector.
	 * @method addSprite
	 * @param  sprite    [sprite object]
	 */
	void addSprite(cocos2d::Sprite* sprite);

	/**
	 * Return a pointer of a sub-pane object that has a certain index.
	 * @param  index [an index of a sub-pane]
	 * @return       [SubPane*]
	 */
	SubPane* getSubPaneAt(size_t index);

	/**
	 * pass the ownership of sprites
	 * @param from [an index of sub-pane that is a old owner of sprites]
	 * @param to   [an index of sub-pane that is a new owner pane of sprites]
	 */
	void passSpriteOwnership(size_t from,size_t to);

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
	bool initWithSize(size_t width,size_t height,int index,int numSubPane,Split split);

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
	 * The size of a sub-pane.
	 */
	CC_SYNTHESIZE_READONLY(size_t,mSubWidth,SubWidth);
	CC_SYNTHESIZE_READONLY(size_t,mSubHeight,SubHeight);
	CC_SYNTHESIZE_READONLY(size_t,mSubPaneSize,SubPaneSize);

	/**
	 * Is a terrain in a pane modified or not.
	 */
	CC_SYNTHESIZE_READONLY(bool,mIsModified,IsModified);

	// NOTE : #b : add on 12/3/2017
	/**
	 * A state of a pane object.
	 */
	CC_SYNTHESIZE(State,mState,State);

	/**
	 * An vector that contains sprites of tiles in a parent node.
	 */
	// cocos2d::Vector<cocos2d::Sprite*> mSprites;
	CC_SYNTHESIZE_READONLY_PASS_BY_REF(cocos2d::Vector<cocos2d::Sprite*>,mSprites,Sprites);

	/**
	 * A vector that contains sub-panes in this pane.
	 */
	cocos2d::Vector<SubPane*> mSubPanes;
};

} /* namespace TM2P5DComponent */

#endif
