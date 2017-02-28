#ifndef TM2P5D_JSON_PARSER_H
#define TM2P5D_JSON_PARSER_H

#include "../../cocos2d/cocos/cocos2d.h"

/**
 * TM2P5DJsonParser class is a member of the namespace 'TM2P5DComponent'.
 */
namespace TM2P5DComponent {

class TM2P5DJsonParser : public cocos2d::Ref
{
public:
	static TM2P5DJsonParser* create();

protected:
	TM2P5DJsonParser();
	~TM2P5DJsonParser();
	bool init();

private:
};

} /* namespace TM2P5DComponent */

#endif
