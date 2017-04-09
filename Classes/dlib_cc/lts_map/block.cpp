#include "block.h"
#include "info_classes.h"
#include "config.h"

/**
 * lts_map::unit::Block class
 */
using namespace cocos2d;
using namespace lts_map::unit;

/**
 * public
 */
Block* Block::Create(const dlib::size<size_t>& size)
{
	auto ret = new Block(size);
	if(ret->Init())
	{
		ret->autorelease();
		return ret;
	}

	CC_SAFE_DELETE(ret);

	return nullptr;
}

void Block::Reset(size_t x, size_t y, bool do_cleanup)
{
	position_.x = x;
	position_.y = y;
	is_modified_ = false;

	if(do_cleanup)
	{
		for(auto& tile : tiles_)
			tile = tile_type_no_tile_;
	}
}

/**
 * protected
 */
 Block::Block()
 :size_(0, 0)
 ,is_modified_(false)
 ,tile_type_no_tile_(kDefaultTileTypeNoTile)
 ,position_(0, 0)
 {}

 Block::Block(const dlib::size<size_t>& size)
 :size_(size.width, size.height)
 ,is_modified_(false)
 ,tiles_(size.area(), tile_type_no_tile_)
 ,tile_type_no_tile_(kDefaultTileTypeNoTile)
 ,position_(0, 0)
 {}

bool Block::Init()
{
	sprites_.reserve(size_.area());

	return true;
}
