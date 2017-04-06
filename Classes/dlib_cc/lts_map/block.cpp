#include "block.h"
#include "info_classes.h"

/**
 * lts_map::unit::Block class
 */
using namespace cocos2d;
using namespace lts_map::unit;

/**
 * public
 */
Block* Block::Create(const dlib::size<size_t>& size, const AtlasInfo* atlas_info)
{
	auto ret = new Block(size, atlas_info->tile_type_no_tile);
	if(ret->Init())
	{
		ret->autorelease();
		return ret;
	}

	CC_SAFE_DELETE(ret);

	return nullptr;
}

void Block::Reset(const dlib::vec2<int>& position, bool do_cleanup)
{
	position_ = position;
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
 :tile_type_no_tile_(-1)
 ,size_(0, 0)
 ,is_modified_(false)
 ,position_(0, 0)
 {}

 Block::Block(const dlib::size<size_t>& size, int tile_type_no_tile)
 :tile_type_no_tile_(tile_type_no_tile)
 ,size_(size.width, size.height)
 ,tiles_(size.area(), tile_type_no_tile_)
 ,is_modified_(false)
 ,position_(0, 0)
 {}

bool Block::Init()
{
	sprites_.reserve(size_.area());

	return true;
}
