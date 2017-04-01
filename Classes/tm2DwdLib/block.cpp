#include "block.h"
#include "info_classes.h"

/**
 * tm2d_w_d::tm2d_w_d_unit::Block class
 */
using namespace tm2d_w_d::tm2d_w_d_unit;

/**
 * public
 */
Block::Block()
:kTileTypeNoTile(-1)
,size_(0, 0)
,position_(0, 0)
,is_modified_(false)
{}

Block::Block(const DLib::size<size_t>& size, int tile_type_no_tile)
:kTileTypeNoTile(tile_type_no_tile)
,size_(size.width, size.height)
,position_(0, 0)
,tiles_(size.area(), kTileTypeNoTile)
,is_modified_(false)
{}

Block* Block::create(const DLib::size<size_t>& size, const AtlasInfo* atlas_info)
{
	auto ret = new Block(size, atlas_info->tile_type_no_tile_);
	if(ret->init())
	{
		ret->autorelease();
		return ret;
	}

	CC_SAFE_DELETE(ret);

	return nullptr;
}

int Block::getTypeAt(int x, int y)
const
{
	return this->getTypeAt(size_.width * y + x);
}

int Block::getTypeAt(int index)
const
{
	return tiles_[index];
}

void Block::insertTypeAt(int x, int y, int type)
{
	this->insertTypeAt(size_.width * y + x, type);
}

void Block::insertTypeAt(int index, int type)
{
	tiles_[index] = type;
}

/**
 * protected
 */
bool Block::init()
{
	return true;
}
