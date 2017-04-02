#ifndef DLIB_CC_LTS_MAP_BLOCK_H
#define DLIB_CC_LTS_MAP_BLOCK_H

#include "../include_lib_index.h"
#include <vector>

namespace lts_map
{
	namespace unit
	{
		class Block;

		//forward declaration
		struct AtlasInfo;
	}
}

class lts_map::unit::Block : public cocos2d::Ref
{
public:
	Block();
	Block(const dlib::size<size_t>& size, int tile_type_no_tile);
	static Block* Create(const dlib::size<size_t>& size, const AtlasInfo* atlas_info);
	int GetTypeAt(int x, int y) const;
	int GetTypeAt(int index) const;
	void InsertTypeAt(int x, int y, int type);
	void InsertTypeAt(int index, int type);
	// accessors and mutators
	inline dlib::size<size_t> size() const { return size_; }
	inline bool is_modified() const { return is_modified_; }
	inline const std::vector<int>& tiles() const { return tiles_; }
	inline dlib::vec2<int> position() const { return position_; }
	inline void set_position(dlib::vec2<int> val) { position_ = val; }

protected:
	bool Init();

private:
	// constant variable
	const int kTileTypeNoTile;
	// read-only variables
	const dlib::size<size_t> size_;
	bool is_modified_;
	std::vector<int> tiles_;
	// others
	dlib::vec2<int> position_;
};

#endif
