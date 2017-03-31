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

Pane::Pane(DLib::size<size_t> v_size, int v_tile_type_no_tile)
:kTileTypeNoTile(v_tile_type_no_tile)
,grid_size_(v_size.width, v_size.height)
,grid_point_(0, 0)
,tiles_(v_size.area(), kTileTypeNoTile)
,is_modified_(false)
{}

Pane* Pane::create(DLib::size<size_t> v_size, const AtlasInfo* v_atlas_info)
{
	auto ret = new Pane(v_size, v_atlas_info->tile_type_no_tile_);
	if(ret->init())
	{
		ret->autorelease();
		return ret;
	}

	CC_SAFE_DELETE(ret);

	return nullptr;
}

int Pane::getTypeAt(int v_x, int v_y)
const
{
	return this->getTypeAt(grid_size_.width * v_y + v_x);
}

int Pane::getTypeAt(int v_index)
const
{
	return tiles_[v_index];
}

void Pane::insertTypeAt(int v_x, int v_y, int v_type)
{
	this->insertTypeAt(grid_size_.width * v_y + v_x, v_type);
}

// void Pane::insertTypeAt(int v_index, int v_type)
void Pane::insertTypeAt(int v_index, int v_type)
{
	tiles_[v_index] = v_type;
}

/**
 * protected
 */
bool Pane::init()
{
	return true;
}
