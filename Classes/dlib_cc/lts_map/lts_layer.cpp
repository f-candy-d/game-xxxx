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
	static constexpr float kDefaultPitch = 0.0;
	// the number of blocks of one side of loading block area (an odd number)
	static constexpr size_t kMinBlockNum = 3;
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
	 * this method should be called after calling OptimizeBlockSize()
	 */

	blocks_.reserve(loading_block_area_size_.area());

	// make SpriteContainer and default blocks
	for(size_t i = 0; i < loading_block_area_size_.area(); ++i)
	{
	}

	is_initialized_ = true;
}

void LTSLayer::OptimizeBlockSize()
{
	OptimizeBlockSize(kDefaultPitch);
}

/**
 * the higher the pitch is, the bigger is the size of loading blocks area
 */
void LTSLayer::OptimizeBlockSize(float pitch)
{
	assert(0.0 <= pitch && pitch <= 1.0);
	/**
	 * if this layer is already initialized by InitLayer(), do not anything in this method
	 */
	if(is_initialized_)
		return;

	std::vector<size_t> options;
	auto visible_size = Director::getInstance()->getWinSize() * 200;
	dlib::size<size_t> min_loading_area(
		actual_tile_size_.width == 0.0
		? 0
		: static_cast<size_t>(visible_size.width / actual_tile_size_.width + 1),
		actual_tile_size_.height == 0.0
		? 0
		: static_cast<size_t>(visible_size.height / actual_tile_size_.height + 1));

	std::cout << "visible size width => " << visible_size.width << '\n';
	std::cout << "actual_tile_size_ = " << actual_tile_size_ << '\n';
	std::cout << "visible_size.width / actual_tile_size_.width = " << visible_size.width / actual_tile_size_.width << '\n';
	std::cout << "min loading area => " << min_loading_area << '\n';

	/**
	 * when a visible size is too large or an actual tile size is too small
	 */
	if(min_loading_area > map_size_)
	{
		std::cout << "LTSLayer::OptimizeBlockSize() >> A visible size is too large or an actual tile size is too small !!" << '\n';
		// make a visible size equal to the map size
		min_loading_area = map_size_;
	}

	// optimize the width of a size of a loading block area
	for(size_t i = map_size_.width; 0 < i; --i)
	{
		// i is an divisor of the map_size_.width
		if(map_size_.width % i == 0)
		{
			size_t block_width = map_size_.width / i;
			// j is an odd number (>1)
			for(size_t j = kMinBlockNum; j <= i; j += 2)
			{
				if(min_loading_area.width <= block_width * j
					&& block_width * j <= map_size_.width)
				{
					if(min_loading_area.width <= block_width * (j - 1))
					{
						options.push_back(block_width);
					}

					break;
				}
			}
		}
	}

	if(options.size() != 0)
	{
		block_size_.width = options[static_cast<size_t>((options.size() - 1) * pitch)];
	}
	options.clear();

	// optimize the height of a size of a loading block area
	for(size_t i = map_size_.height; 0 < i; --i)
	{
		if(map_size_.height % i == 0)
		{
			size_t block_height = map_size_.height / i;
			for(size_t j = kMinBlockNum; j <= i; j += 2)
			{
				if(min_loading_area.height <= block_height * j
					&& block_height * j <= map_size_.height)
				{
					if(min_loading_area.height <= block_height * (j - 1))
					{
						options.push_back(block_height);
					}
					break;
				}
			}
		}
	}

	if(options.size() != 0)
	{
		block_size_.height = options[static_cast<size_t>((options.size() - 1) * pitch)];
	}
	AdjustLoadingBlockArea();

	std::cout << "block size => " << block_size_ << '\n';
	std::cout << "loading block area => " << loading_block_area_size_ << '\n';
}

void LTSLayer::AdjustLoadingBlockArea()
{
	auto visible_size = Director::getInstance()->getWinSize();
	dlib::size<size_t> min_loading_area(
		static_cast<size_t>(visible_size.width / actual_tile_size_.width + 1),
		static_cast<size_t>(visible_size.height / actual_tile_size_.height + 1));

	assert(min_loading_area <= map_size_);

	// loading_block_area_size_.width must be an odd number
	for(loading_block_area_size_.width = kMinBlockNum;
		loading_block_area_size_.width * block_size_.width < min_loading_area.width
		&& loading_block_area_size_.width * block_size_.width < map_size_.width;
		loading_block_area_size_.width += 2);

	// loading_block_area_size_.height must be an odd number
	for(loading_block_area_size_.height = kMinBlockNum;
		loading_block_area_size_.height * block_size_.height < min_loading_area.height
		&& loading_block_area_size_.height * block_size_.height < map_size_.height;
		loading_block_area_size_.height += 2);
}

void LTSLayer::ScaleTile(float scale)
{
	ScaleTile(scale, true);
}

void LTSLayer::ScaleTile(float scale, bool do_adjustment)
{
	actual_tile_size_.scale(scale / tile_scale_);
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
,actual_tile_size_(atlas_info->texture_size)
,loading_block_area_size_(0, 0)
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
	 * be sure of calling OptimizeBlockSize() and InitLayer() methods manually!
	 */
	if(auto_initialization)
	{
		OptimizeBlockSize(kDefaultPitch);
		InitLayer();
	}

	return true;
}

/**
 * private
 */
