#include "lts_layer.h"
#include "info_classes.h"

/**
 * lts_map::unit::LTSLayer class
 */

using namespace cocos2d;
using namespace lts_map::unit;

/**
 * public
 */
LTSLayer* LTSLayer::Create(
	const LayerInfo* layer_info, const AtlasInfo* atlas_info, const TerrainInfo* terrain_info)
{
	auto ret = new LTSLayer(layer_info, atlas_info, terrain_info);
	if(ret->InitWithInfo(layer_info, atlas_info, terrain_info))
	{
		ret->autorelease();
		return ret;
	}

	CC_SAFE_DELETE(ret);
	return nullptr;
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
,tile_scale_(layer_info->tile_scale)
,block_size_(terrain_info->block_size)
,location_pin_map_(terrain_info->location_pin_map)
{}

bool LTSLayer::InitWithInfo(
	const LayerInfo* layer_info, const AtlasInfo* atlas_info, const TerrainInfo* terrain_info)
{
	if(!(layer_info->Valied() && atlas_info->Valied() && terrain_info->Valied()))
		return false;

	auto block_num = OptimizeBlockSizeAndNum();
	blocks_.reserve(block_num);
	sprites_.reserve(block_num);

	// make SpriteContainer and default blocks
	for(int i = 0; i < block_num; ++i)
	{
		std::string container_id("id_" + std::to_string(i));
		Block::SpriteOwnership ownership;
		ownership.SetOwnership(container_id);
	}

	return true;
}

/**
 * private
 */
int LTSLayer::OptimizeBlockSizeAndNum()
{
	int block_num = 10;
	return block_num;
}
