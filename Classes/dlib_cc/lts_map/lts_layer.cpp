#include "lts_layer.h"
#include "info_classes.h"
#include "config.h"
#include <iostream>
#include <fstream>

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
	static constexpr size_t kMinLoadingBlockAreaWidth = 3;
	static constexpr size_t kMinLoadingBlockAreaHeight = 3;
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
	center_block_position_.set(loading_block_area_size_.width / 2, loading_block_area_size_.height / 2);

	// make default blocks
	for(size_t y = 0; y < loading_block_area_size_.height; ++y)
	{
		for(size_t x = 0; x < loading_block_area_size_.width; ++x)
		{
			blocks_.pushBack(LoadTerrainIntoBlock(x, y, nullptr));
		}
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
	auto visible_size = Director::getInstance()->getWinSize();
	dlib::size<size_t> min_loading_area(
		actual_tile_size_.width == 0.0
		? 0
		: static_cast<size_t>(visible_size.width / actual_tile_size_.width + 1),
		actual_tile_size_.height == 0.0
		? 0
		: static_cast<size_t>(visible_size.height / actual_tile_size_.height + 1));

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
			for(size_t j = kMinLoadingBlockAreaWidth; j <= i; j += 2)
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
			for(size_t j = kMinLoadingBlockAreaHeight; j <= i; j += 2)
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
	for(loading_block_area_size_.width = kMinLoadingBlockAreaWidth;
		loading_block_area_size_.width * block_size_.width < min_loading_area.width
		&& loading_block_area_size_.width * block_size_.width < map_size_.width;
		loading_block_area_size_.width += 2);

	// loading_block_area_size_.height must be an odd number
	for(loading_block_area_size_.height = kMinLoadingBlockAreaHeight;
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

void LTSLayer::MoveTo(size_t new_center_x, size_t new_center_y)
{
	// shift all elements in blocks_ to the left by the same times as the width of loading-block-area-size
	std::vector<int> vec;
	for(int i = 0; i < 3 * 3; ++i)
		vec.push_back(i);

	std::vector<int> tmp;
	tmp.reserve(vec.size());

	std::cout << "before" << '\n';
	for(auto e : vec)
		std::cout << "vec => " << e << '\n';

	for(size_t i = vec.size() - 3, c = 0; c < vec.size(); ++i, ++c)
	{
		i = i < vec.size() ? i : 0;
		tmp.push_back(vec[i]);
	}
	vec = std::move(tmp);

	std::cout << "after" << '\n';
	for(auto e : vec)
		std::cout << "vec => " << e << '\n';

	for(size_t i = 0; i < 3; ++i)
	{
		std::cout << "load => " << vec[i] << '\n';
	}
}

void LTSLayer::MoveTo(dlib::vec2<size_t> new_center_position)
{
	MoveTo(new_center_position.x, new_center_position.y);
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
,loading_block_area_size_(kMinMapWidth, kMinMapHeight)
,block_size_(terrain_info->block_size)
,center_block_position_(0, 0)
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
Block* LTSLayer::LoadTerrainIntoBlock(size_t x, size_t y, Block* used)
{
	auto block_num_width = map_size_.width / block_size_.width;
	auto block_num_height = map_size_.height / block_size_.height;
	assert(0 <= x && x < block_num_width);
	assert(0 <= y && y < block_num_height);

	if(used == nullptr)
	{
		used = Block::Create(block_size_);
		used->set_tile_type_no_tile(tile_type_no_tile_);
		used->set_position(x, y);
	}
	else
	{
		used->Reset(x, y, false);
	}

	ReadTerrainDataBinary(used);
	return std::move(used);
}

bool LTSLayer::SaveTerrainInBlock(const Block* block)
{
	if(!block)
		return false;
}

bool LTSLayer::ReadTerrainDataBinary(Block* block)
{
	if(!block)
		return false;

	const std::string path = FileUtils::getInstance()->fullPathForFilename(kMapTerrainDirectory + terrain_src_name_);
	std::ifstream ifs(path.c_str(), std::ios::binary);

	assert(ifs);
	std::cout << "load terrain data from " << path << '\n';

	auto position = block->position();
	auto index = position.y * (map_size_.width / block_size_.width) + position.x;
	ifs.seekg(index * block->size().area() * sizeof(int), ifs.beg);

	for(size_t i = 0; i < block->size().area(); ++i)
	{
		int type = tile_type_no_tile_;
		ifs.read((char*)&type, sizeof(type));
		block->InsertTypeAt(i, type);
	}

	return true;
}

bool LTSLayer::WriteTerrainDataBinary(const Block* block)
{
	if(!block)
		return false;
}

void LTSLayer::AllocateSpritesToBlock(const Block* block)
{

}

void LTSLayer::MoveToRightNextColumn()
{
	assert(blocks_.size() == loading_block_area_size_.area());

	size_t new_x = blocks_.back()->position().x + 1;

	if(new_x < map_size_.width)
	{
		for(size_t y = 0; y < loading_block_area_size_.height; ++y)
		{
			for(size_t i = 0; i < loading_block_area_size_.width - 1; ++i)
			{
				blocks_.swap(
					y * loading_block_area_size_.width + i,
					y * loading_block_area_size_.width + i + 1);
			}

			LoadTerrainIntoBlock(
				new_x,
				blocks_.at((y + 1) * loading_block_area_size_.width - 1)->position().y,
				blocks_.at((y + 1) * loading_block_area_size_.width - 1));
		}
	}
}

void LTSLayer::MoveToLeftNextColumn()
{
	assert(blocks_.size() == loading_block_area_size_.area());

	size_t new_x = blocks_.front()->position().x - 1;

	if(0 <= new_x)
	{
		for(size_t y = 0; y < loading_block_area_size_.height; ++y)
		{
			for(size_t i = 0; i < loading_block_area_size_.width - 1; ++i)
			{
				blocks_.swap(
					(y + 1) * loading_block_area_size_.width - 1 - i,
					(y + 1) * loading_block_area_size_.width - 1 - i - 1);
			}

			LoadTerrainIntoBlock(
				new_x,
				blocks_.at(y * loading_block_area_size_.width)->position().y,
				blocks_.at(y * loading_block_area_size_.width));
		}
	}
}

void LTSLayer::MoveToRowBelow()
{
	assert(blocks_.size() == loading_block_area_size_.area());

	size_t new_y = blocks_.front()->position().y - 1;

	if(0 <= new_y)
	{
		// shift all elements in blocks_ to the right by the same times as the width of loading-block-area-size
		Vector<Block*> tmp;
		tmp.reserve(blocks_.size());
		for(size_t i = blocks_.size() - loading_block_area_size_.width, c = 0; c < blocks_.size(); ++i, ++c)
		{
			i = i < blocks_.size() ? i : 0;
			tmp.pushBack(blocks_.at(i));
		}
		blocks_ = std::move(tmp);

		for(size_t i = 0; i < loading_block_area_size_.width; ++i)
		{
			LoadTerrainIntoBlock(blocks_.at(i)->position().x, new_y, blocks_.at(i));
		}
	}
}

void LTSLayer::MoveToRowAbove()
{
	assert(blocks_.size() == loading_block_area_size_.area());

	size_t new_y = blocks_.back()->position().y + 1;

	if(new_y < map_size_.height)
	{
		// shift all elements in blocks_ to the left by the same times as the width of loading-block-area-size
		Vector<Block*> tmp;
		tmp.reserve(blocks_.size());
		for(size_t i = loading_block_area_size_.width, c = 0; c < blocks_.size(); ++i, ++c)
		{
			i = i < blocks_.size() ? i : 0;
			tmp.pushBack(blocks_.at(i));
		}
		blocks_ = std::move(tmp);

		for(size_t i = blocks_.size() - loading_block_area_size_.width; i < blocks_.size(); ++i)
		{
			LoadTerrainIntoBlock(blocks_.at(i)->position().x, new_y, blocks_.at(i));
		}
	}
}
