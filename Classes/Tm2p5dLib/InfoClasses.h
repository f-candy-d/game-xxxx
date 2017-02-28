#ifndef INFO_CLASSES_H
#define INFO_CLASSES_H

#include "../../cocos2d/cocos/cocos2d.h"

/**
 * MapInfo, LayerInfo, LayerBundlerInfo, AtlasInfo class are members of the namespace 'TM2P5DComponent'.
 */
namespace TM2P5DComponent {

/**
 * Map information class
 */
class MapInfo : public cocos2d::Ref
{
public:
	static MapInfo* create();

protected:
	MapInfo();
	~MapInfo();
	bool init();
};

/**
 * Tiled Layer information class
 */
class LayerInfo : public cocos2d::Ref
{
public:
	static LayerInfo* create();

protected:
	LayerInfo();
	~LayerInfo();
	bool init();
};

/**
 * Tiled Layer Bundler information class
 */
class LayerBundlerInfo : public cocos2d::Ref
{
public:
	static LayerBundlerInfo* create();

protected:
	LayerBundlerInfo();
	~LayerBundlerInfo();
	bool init();
};

/**
 * Tile Atlas information class
 */
class AtlasInfo : public cocos2d::Ref
{
public:
	static AtlasInfo* create();

protected:
	AtlasInfo();
	~AtlasInfo();
	bool init();
};

} /* namespace TM2P5DComponent */

#endif
