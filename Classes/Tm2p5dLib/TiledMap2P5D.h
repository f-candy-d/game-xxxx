#ifndef TILED_MAP_2P5D_H
#define TILED_MAP_2P5D_H

#include "../../cocos2d/cocos/cocos2d.h"
#include <string>

class TiledMap2P5D : public cocos2d::Node
{
public:
	/**
	 * Create TiledMap2P5D object
	 * @method create
	 * @param  origin [The name of origin information file]
	 * @return        [TiledMap2P5D*]
	 */
	static TiledMap2P5D* create(std::string origin);

protected:
	TiledMap2P5D();
	~TiledMap2P5D();

	/**
	 * Initialize TiledMap2P5D object
	 * @method initWithOrigin
	 * @param  origin         [The name of origin information file]
	 * @return                [true/false]
	 */
	bool initWithOrigin(std::string origin);

private:

	// TODO : for test, remove the following lines later
	cocos2d::Rect rectPool;
	size_t paneWidth;
	size_t paneHeight;
	cocos2d::Size tileSize;
};

#endif
