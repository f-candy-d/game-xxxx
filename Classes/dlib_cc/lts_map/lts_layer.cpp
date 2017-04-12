#include "lts_layer.h"
#include "info_classes.h"
#include "config.h"
#include "../utils/binary_file_stream.h"
#include <iostream>
#include <fstream>
#include <cstdio>

/**
 * dlib_cc::lts_map_unit::LTSLayer class
 */

using namespace cocos2d;
using namespace dlib_cc::lts_map_unit;

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
			auto block = LoadTerrainIntoBlock(x, y, nullptr);
			blocks_.pushBack(block);
			AllocateSpritesToBlock(block);
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
	AlignBlocksInStraightLineInFile();

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

	std::cout << "adjustment : loading block area size => " << loading_block_area_size_ << '\n';
}

void LTSLayer::ScaleTile(float scale)
{
	ScaleTile(scale, true);
}

void LTSLayer::ScaleTile(float scale, bool do_adjustment)
{
	actual_tile_size_.scale(scale / tile_scale_);
	tile_scale_ = scale;
	// this->setScale(scale);

	if(do_adjustment)
	{
		AdjustLoadingBlockArea();
	}
}

void LTSLayer::MoveTo(int new_center_x, int new_center_y)
{
	MoveTo(std::move(dlib::vec2<int>(new_center_x, new_center_y)));
}

void LTSLayer::MoveTo(dlib::vec2<int> new_center_block_position)
{
	// adjustment
	for(; new_center_block_position.x - (static_cast<int>(loading_block_area_size_.width) / 2) < 0; ++new_center_block_position.x);
	for(; map_size_.width / block_size_.width <= new_center_block_position.x + loading_block_area_size_.width / 2; --new_center_block_position.x);
	for(; new_center_block_position.y - (static_cast<int>(loading_block_area_size_.height) / 2) < 0; ++new_center_block_position.y);
	for(; map_size_.height / block_size_.height <= new_center_block_position.y + loading_block_area_size_.height / 2; --new_center_block_position.y);

	std::cout << "move to " << new_center_block_position << " from " << center_block_position_ << '\n';

	auto delta = new_center_block_position - center_block_position_;

	if(loading_block_area_size_.width <= delta.x
		|| loading_block_area_size_.width <= -delta.x
		|| loading_block_area_size_.height <= delta.y
		|| loading_block_area_size_.height <= -delta.y)
	{
		// reload all blocks
		auto itr_block = blocks_.begin();
		for(size_t y = new_center_block_position.y - loading_block_area_size_.height / 2;
			y <= new_center_block_position.y + loading_block_area_size_.height / 2;
			++y)
		{
			for(size_t x = new_center_block_position.x - loading_block_area_size_.width / 2;
				x <= new_center_block_position.x + loading_block_area_size_.width / 2;
				++x)
			{
				LoadTerrainIntoBlock(x, y, *itr_block);
				AllocateSpritesToBlock(*itr_block);
			}
		}

		// update
		center_block_position_ = new_center_block_position;

		return;
	}

	// x-coordinate
	if(0 < delta.x && delta.x < loading_block_area_size_.width)
	{
		for(int i = 0; i < delta.x; ++i)
			MoveToRightNextColumn();
	}
	else if(delta.x < 0 && -delta.x < loading_block_area_size_.width)
	{
		for(int i = 0; i < -delta.x; ++i)
			MoveToLeftNextColumn();
	}

	// y-coordinate
	if(0 < delta.y && delta.y < loading_block_area_size_.height)
	{
		for(int i = 0; i < delta.y; ++i)
			MoveToRowAbove();
	}
	else if(delta.y < 0 && -delta.y < loading_block_area_size_.height)
	{
		for(int i = 0; i < -delta.y; ++i)
			MoveToRowBelow();
	}

	// update
	center_block_position_ = new_center_block_position;
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
	if(!Layer::init())
		return false;

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
		WriteTerrainDataBinary(used);
		used->Reset(x, y, false);
	}

	ReadTerrainDataBinary(used);
	return std::move(used);
}

bool LTSLayer::ReadTerrainDataBinary(Block* block)
{
	if(!block)
		return false;

	auto path = FileUtils::getInstance()->fullPathForFilename(kMapTerrainDirectory + terrain_src_name_);
	auto position = block->position();
	auto index = position.y * (map_size_.width / block_size_.width) + position.x;
	auto bfs = dlib_cc::utils::BinaryFileStream::Create();
	std::vector<int> buff;
	buff.reserve(block->tiles().size());

	assert(bfs->Read(path, buff, block->size().area(), index * block->size().area()));
	block->CopyTiles(std::move(buff));

	std::cout << "read terrain from " << path << '\n';

	return true;
}

bool LTSLayer::WriteTerrainDataBinary(const Block* block)
{
	if(!block || !block->is_modified())
		return false;

	const std::string path = FileUtils::getInstance()->fullPathForFilename(kMapTerrainDirectory + terrain_src_name_);
	// std::ofstream ofs(path.c_str(), std::ios::in|std::ios::out|std::ios::binary|std::ios::app);
	//
	// assert(ofs);
	// std::cout << "write terrain data to " << path << '\n';
	//
	// auto position = block->position();
	// auto index = position.y * (map_size_.width / block_size_.width) + position.x;
	// ofs.seekp(index * block->size().area() * sizeof(int), ofs.beg);
	//
	// for(auto type : block->tiles())
	// {
	// 	ofs.write((char*)&type, sizeof(int));
	// }

	auto position = block->position();
	auto index = position.y * (map_size_.width / block_size_.width) + position.x;
	auto bfs = dlib_cc::utils::BinaryFileStream::Create();
	assert(bfs->Write<int>(path, block->tiles(), true, index * block->size().area()));
	std::cout << "write terrain data to " << path << '\n';

	return true;
}

void LTSLayer::AlignBlocksInStraightLineInFile()
{
	const std::string path =
		FileUtils::getInstance()->fullPathForFilename(kMapTerrainDirectory + terrain_src_name_);
	auto bfs = dlib_cc::utils::BinaryFileStream::Create();
}

void LTSLayer::AllocateSpritesToBlock(Block* block)
{
	assert(block);

	for(; block->sprites().size() < block->size().area();)
	{
		auto sprite = Sprite::create(kMapAtlasDirectory + atlas_src_name_);
		this->addChild(sprite);
		block->PushBackSprite(sprite);
		sprite->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	}

	Vec2 block_origin(
		block->size().width * actual_tile_size_.width * block->position().x,
		block->size().height * actual_tile_size_.height * block->position().y);

	int count = 0;

	for(int y = 0; y < block->size().height; ++y)
	{
		for(int x = 0; x < block->size().width; ++x)
		{
			auto type = block->GetTypeAt(y * block->size().width + x);
			if(tile_type_no_tile_ != type)
			{
				auto sprite = block->GetSpriteAt(y * block->size().width + x);
				auto texture_pos = texture_positions_[type];
				Rect texture_rect(texture_pos.x, texture_pos.y, texture_size_.width, texture_size_.height);
				sprite->setTextureRect(std::move(texture_rect));
				sprite->setPosition(
					block_origin.x + actual_tile_size_.width * x,
					block_origin.y + actual_tile_size_.height * y);
				sprite->setScale(tile_scale_);

				count++;
			}
		}
	}

	std::cout << "allocated " << count << " sprites" << '\n';
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
			AllocateSpritesToBlock(blocks_.at((y + 1) * loading_block_area_size_.width - 1));
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
			AllocateSpritesToBlock(blocks_.at(y * loading_block_area_size_.width));
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
			AllocateSpritesToBlock(blocks_.at(i));
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
			AllocateSpritesToBlock(blocks_.at(i));
		}
	}
}
