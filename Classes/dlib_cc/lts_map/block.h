#ifndef DLIB_CC_LTS_MAP_BLOCK_H
#define DLIB_CC_LTS_MAP_BLOCK_H

#include "../tools/include_lib_index.h"
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
	static Block* Create(const dlib::size<size_t>& size);
	void Reset(const dlib::vec2<int>& position, bool do_cleanup) {
		Reset(position.x, position.y, do_cleanup); }
	void Reset(size_t x, size_t y, bool do_cleanup);
	int GetTypeAt(int index) const { return tiles_[index]; }
	void InsertTypeAt(int index, int type) { tiles_[index] = type; }
	void PushBackType(int type) { tiles_.push_back(type); }
	cocos2d::Sprite* GetSpriteAt(int index) const { return sprites_.at(index); }
	void InsertSpriteAt(int index, cocos2d::Sprite* sprite) { sprites_.insert(index, sprite); }
	void PushBackSprite(cocos2d::Sprite* sprite) { sprites_.pushBack(sprite); }
	// accessors and mutators
	dlib::size<size_t> size() const { return size_; }
	bool is_modified() const { return is_modified_; }
	const std::vector<int>& tiles() const { return tiles_; }
	const cocos2d::Vector<cocos2d::Sprite*>& sprites() const { return sprites_; }
	int tile_type_no_tile() const { return tile_type_no_tile_; }
	dlib::vec2<int> position() const { return position_; }
	void set_position(const dlib::vec2<int>& pos) { position_ = pos; }
	void set_position(const int x, const int y) { position_.x = x; position_.y = y; }
	void set_tile_type_no_tile(const int type) { tile_type_no_tile_ = type; }

protected:
	Block();
	Block(const dlib::size<size_t>& size);
	bool Init();

private:
	// constant variable
	const dlib::size<size_t> size_;
	// read-only variables
	bool is_modified_;
	std::vector<int> tiles_;
	cocos2d::Vector<cocos2d::Sprite*> sprites_;
	// others
	int tile_type_no_tile_;
	dlib::vec2<int> position_;
};

#endif
