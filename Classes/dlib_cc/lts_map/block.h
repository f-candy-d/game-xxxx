#ifndef TM2D_W_D_LIB_BLOCK_H
#define TM2D_W_D_LIB_BLOCK_H

#include "../../../cocos2d/cocos/cocos2d.h"
#include "../../dlib/size.h"
#include "../../dlib/vec2.h"
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
	Block(const DLib::size<size_t>& size, int tile_type_no_tile);
	static Block* Create(const DLib::size<size_t>& size, const AtlasInfo* atlas_info);
	int GetTypeAt(int x, int y) const;
	int GetTypeAt(int index) const;
	void InsertTypeAt(int x, int y, int type);
	void InsertTypeAt(int index, int type);
	// accessors and mutators
	inline DLib::size<size_t> size() const { return size_; }
	inline bool is_modified() const { return is_modified_; }
	inline const std::vector<int>& tiles() const { return tiles_; }
	inline DLib::vec2<int> position() const { return position_; }
	inline void set_position(DLib::vec2<int> val) { position_ = val; }

protected:
	bool Init();

private:
	// constant variable
	const int kTileTypeNoTile;
	// read-only variables
	const DLib::size<size_t> size_;
	bool is_modified_;
	std::vector<int> tiles_;
	// others
	DLib::vec2<int> position_;
};

#endif
