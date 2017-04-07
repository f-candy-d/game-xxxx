#include "lts_layer.h"
#include "info_classes.h"
#include <iostream>

/**
 * lts_map::unit::LTSLayer class
 */

using namespace cocos2d;
using namespace lts_map::unit;

/**
 * compile-time constants
 */
namespace
{
	// default pitch of a loading block area size
	static constexpr float kDefPitchOfLoadingBlockArea = 0.5;
}

/**
 * public
 */
LTSLayer* LTSLayer::Create(
	const LayerInfo* layer_info, const AtlasInfo* atlas_info, const TerrainInfo* terrain_info)
{
	return Create(layer_info, atlas_info, terrain_info, true);
}

LTSLayer* LTSLayer::Create(
	const LayerInfo *layer_info, const AtlasInfo *atlas_info, const TerrainInfo *terrain_info, bool auto_initialization)
{
	auto ret = new LTSLayer(layer_info, atlas_info, terrain_info);
	if(ret->InitWithInfo(layer_info, atlas_info, terrain_info, auto_initialization))
	{
		ret->autorelease();
		return ret;
	}

	CC_SAFE_DELETE(ret);
	return nullptr;
}

void LTSLayer::InitLayer()
{
	/**
	 * this method should be called after calling OptimizeLoadingBlock()
	 */

	blocks_.reserve(num_width_block_ * num_height_block_);

	// make SpriteContainer and default blocks
	for(int i = 0; i < num_width_block_ * num_height_block_; ++i)
	{
	}

	is_initialized_ = true;
}

void LTSLayer::OptimizeLoadingBlock(float pitch = kDefPitchOfLoadingBlockArea)
{
	assert(0.0 <= pitch && pitch <= 1.0);
	/**
	 * if this layer is already initialized by InitLayer(), do not anything in this method
	 */
	if(is_initialized_)
		return;

	// the width of a block
	int map_width = map_size_.width;
	const int max_divisor_w = map_width;
	std::vector<int> divisors_w;

	for(int i = max_divisor_w; 0 < i; --i)
	{
		if(map_width % i == 0)
			divisors_w.push_back(i);
	}

	block_size_.width =
		map_width / divisors_w[static_cast<int>((divisors_w.size() - 1) * pitch)];

	// the height of a block
	int map_height = map_size_.height;
	const int max_divisor_h = map_height;
	std::vector<int> divisors_h;

	for(int i = max_divisor_h; 0 < i; --i)
	{
		if(map_height % i == 0)
			divisors_h.push_back(i);
	}

	block_size_.height =
		map_height / divisors_h[static_cast<int>((divisors_h.size() - 1) * pitch)];
}

void LTSLayer::AdjustLoadingBlockArea()
{
	//
	// auto visible_size = Director::getInstance()->getWinSize();
	//
	// // the horizontal number of blocks
	// const int max_block_w_num = map_size_.width / block_size_.width;
	// int min_block_w_num(1);
	//
	// for(;min_block_w_num * actual_tile_size_.width * block_size_.width < visible_size.width;
	// 	&& min_block_w_num < max_block_w_num;
	// 	++min_block_w_num);
	//
	// num_width_block_ = (max_block_w_num - min_block_w_num + 1) * pitch;
	//
	// // the horizontal number of blocks
	// const int max_block_h_num = map_size_.height / block_size_.height;
	// int min_block_h_num(1);
	//
	// for(;min_block_h_num * actual_tile_size_.height * block_size_.height < visible_size.height;
	// 	&& min_block_h_num < max_block_h_num;
	// 	++min_block_h_num) {}
	//
	// num_height_block_ = (max_block_h_num - min_block_h_num + 1) * pitch;
	//
	// std::cout << "num width blocks =>" << num_width_block_ << '\n';
	// std::cout << "num height blocks =>" << num_height_block_ << '\n';
}

void LTSLayer::ScaleTile(float scale, bool do_adjustment)
{
	tile_scale_ = scale;

	if(do_adjustment)
	{
		AdjustLoadingBlockArea();
	}
}

/**
 * protected
 */
LTSLayer::LTSLayer(
	const LayerInfo* layer_info, const AtlasInfo* atlas_info, const TerrainInfo* terrain_info)
:tile_type_no_tile_(atlas_info->tile_type_no_tile)
,texture_size_(atlas_info->texture_size)
,layer_name_(layer_info->layer_name)
,atlas_name_(atlas_info->atlas_name)
,terrain_name_(terrain_info->terrain_name)
,atlas_src_name_(atlas_info->atlas_src_name)
,terrain_src_name_(terrain_info->terrain_src_name)
,num_tile_type_(atlas_info->num_tile_type)
,map_size_(terrain_info->map_size)
,texture_positions_(atlas_info->texture_positions)
,is_visible_(layer_info->is_visible)
,is_editable_(layer_info->is_editable)
,is_initialized_(false)
,tile_scale_(layer_info->tile_scale)
,num_width_block_(1)
,num_height_block_(1)
,loading_block_area_size_(atlas_info->texture_size)
,actual_tile_size_(atlas_info->texture_size)
,block_size_(terrain_info->block_size)
,location_pin_map_(terrain_info->location_pin_map)
{}

bool LTSLayer::InitWithInfo(
	const LayerInfo* layer_info, const AtlasInfo* atlas_info, const TerrainInfo* terrain_info, bool auto_initialization)
{
	if(!(layer_info->Valied() && atlas_info->Valied() && terrain_info->Valied()))
		return false;

	/**
	 * if auto_initialization is false,
	 * be sure of calling OptimizeLoadingBlock() and InitLayer() methods manually!
	 */
	if(auto_initialization)
	{
		OptimizeLoadingBlock(kDefPitchOfLoadingBlockArea);
		InitLayer();
	}

	return true;
}

/**
 * private
 */
