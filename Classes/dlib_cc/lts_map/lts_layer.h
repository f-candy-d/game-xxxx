#ifndef DLIB_CC_LTS_MAP_TLS_LAYER_H
#define DLIB_CC_LTS_MAP_TLS_LAYER_H

#include "../tools/include_lib_index.h"
#include "block.h"
#include <string>
#include <vector>
#include <unordered_map>

namespace lts_map
{
	namespace unit
	{
		class LTSLayer;

		// forward declaration
		struct LayerInfo;
		struct AtlasInfo;
		struct TerrainInfo;
	}
}

class lts_map::unit::LTSLayer : public cocos2d::Ref
{
	using SpriteContainer = cocos2d::Vector<cocos2d::Sprite*>;

public:
	static LTSLayer* Create(
		const LayerInfo* layer_info, const AtlasInfo* atlas_info, const TerrainInfo* terrain_info);

	static LTSLayer* Create(
		const LayerInfo* layer_info, const AtlasInfo* atlas_info, const TerrainInfo* terrain_info, bool auto_initialization);

	void InitLayer();
	void OptimizeBlockSize(float pitch);
	void OptimizeBlockNum(float pitch);
	void ScaleTile(float scale, bool do_optimization);

	// accessors and mutators
	inline const std::string& layer_name() const { return layer_name_; }
	inline const std::string& atlas_name() const { return atlas_name_; }
	inline const std::string& terrain_name() const { return terrain_name_; }
	inline bool is_visible() const { return is_visible_; }
	inline bool is_editable() const { return is_editable_; }
	inline bool is_initialized() const { return is_initialized_; }
	inline float tile_scale() const { return tile_scale_; }
	inline void set_is_visible(bool is_visible) { is_visible_ = is_visible; }
	inline void set_is_editable(bool is_editable) { is_editable_ = is_editable; }

protected:
	LTSLayer(
		const LayerInfo* layer_info, const AtlasInfo* atlas_info, const TerrainInfo* terrain_info);

	bool InitWithInfo(
		const LayerInfo* layer_info, const AtlasInfo* atlas_info, const TerrainInfo* terrain_info, bool auto_initialization);

private:
	// constant variables
	const int tile_type_no_tile_;
	const dlib::size<float> texture_size_;
	const std::string layer_name_;
	const std::string atlas_name_;
	const std::string terrain_name_;
	const std::string atlas_src_name_;
	const std::string terrain_src_name_;
	const int num_tile_type_;
	const dlib::size<size_t> map_size_;
	const std::vector<dlib::vec2<float>> texture_positions_;
	// others
	bool is_visible_;
	bool is_editable_;
	bool is_initialized_;
	float tile_scale_;
	int num_width_block_;
	int num_height_block_;
	dlib::size<float> actual_tile_size_;
	dlib::size<size_t> block_size_;
	std::unordered_map<std::string, dlib::vec2<int>> location_pin_map_;
	cocos2d::Vector<Block*> blocks_;

	void LoadTerrainIntoBlock(int x, int y);
	void SaveTerrainInBlock(int x, int y);
	bool ReadTerrainDataBinary(int x, int y);
	bool WriteTerrainDataBinary(int x, int y);
	void AllocateSpritesToBlock(const Block* block);
};

#endif
