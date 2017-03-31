#include "pane.h"
#include "info_classes.h"

/**
 * tm2d_w_d::tm2d_w_d_unit::Pane class
 */
using namespace tm2d_w_d::tm2d_w_d_unit;

/**
 * public
 */
Pane::Pane()
:kTileTypeNoTile(-1)
,grid_size_(0, 0)
,grid_point_(0, 0)
,is_modified_(false)
{}

Pane::Pane(DLib::size<size_t> _size, int _tile_type_no_tile)
:kTileTypeNoTile(_tile_type_no_tile)
,grid_size_(_size.width, _size.height)
,grid_point_(0, 0)
,tiles_(_size.area(), kTileTypeNoTile)
,is_modified_(false)
{}

Pane* Pane::create(DLib::size<size_t> _size, const AtlasInfo* _atlas_info)
{
	auto ret = new Pane(_size, _atlas_info->tile_type_no_tile_);
	if(ret->init())
	{
		ret->autorelease();
		return ret;
	}

	CC_SAFE_DELETE(ret);

	return nullptr;
}

int Pane::getTypeAt(int _x, int _y)
const
{
	return this->getTypeAt(grid_size_.width * _y + _x);
}

int Pane::getTypeAt(int _index)
const
{
	return tiles_[_index];
}

void Pane::insertTypeAt(int _x, int _y, int _type)
{
	this->insertTypeAt(grid_size_.width * _y + _x, _type);
}

// void Pane::insertTypeAt(int _index, int _type)
void Pane::insertTypeAt(int _index, int _type)
{
	tiles_[_index] = _type;
}

/**
 * protected
 */
bool Pane::init()
{
	return true;
}
