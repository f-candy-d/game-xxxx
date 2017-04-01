#ifndef TM2D_W_D_LIB_BLOCK_H
#define TM2D_W_D_LIB_BLOCK_H

#include "../../cocos2d/cocos/cocos2d.h"
#include "DLib/dlib_size.h"
#include "DLib/dlib_vec2.h"
#include <vector>

namespace tm2d_w_d
{
	namespace tm2d_w_d_unit
	{
		class Block;

		//forward declaration
		struct AtlasInfo;
	}
}

class tm2d_w_d::tm2d_w_d_unit::Block : public cocos2d::Ref
{
public:
	Block();
	Block(const DLib::size<size_t>& size, int tile_type_no_tile);
	static Block* create(const DLib::size<size_t>& size, const AtlasInfo* atlas_info);
	int getTypeAt(int x, int y) const;
	int getTypeAt(int index) const;
	void insertTypeAt(int x, int y, int type);
	void insertTypeAt(int index, int type);
	// accessors and mutators
	inline DLib::size<size_t> get_size() const { return size_; }
	inline bool is_modified() const { return is_modified_; }
	inline std::vector<int>& get_tiles() { return tiles_; }
	inline const std::vector<int>& get_tiles() const { return tiles_; }
	inline DLib::vec2<unsigned int> get_position() const { return position_; }
	inline void set_position(DLib::vec2<unsigned int> val) { position_ = val; }

protected:
	bool init();

private:
	// constant variable
	const int kTileTypeNoTile;
	// read-only variables
	const DLib::size<size_t> size_;
	bool is_modified_;
	std::vector<int> tiles_;
	// others
	DLib::vec2<unsigned int> position_;
};

#endif
