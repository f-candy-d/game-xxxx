#include "block.h"
#include "info_classes.h"

/**
 * lts_map::unit::Block class
 */
using namespace lts_map::unit;

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

Block* Block::Create(const DLib::size<size_t>& size, const AtlasInfo* atlas_info)
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

int Block::GetTypeAt(int x, int y)
const
{
	return this->GetTypeAt(size_.width * y + x);
}

int Block::GetTypeAt(int index)
const
{
	return tiles_[index];
}

void Block::InsertTypeAt(int x, int y, int type)
{
	this->InsertTypeAt(size_.width * y + x, type);
}

void Block::InsertTypeAt(int index, int type)
{
	tiles_[index] = type;
}

/**
 * protected
 */
bool Block::Init()
{
	return true;
}