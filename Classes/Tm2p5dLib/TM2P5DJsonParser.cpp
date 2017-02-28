#include "TM2P5DJsonParser.h"

USING_NS_CC;
using namespace TM2P5DComponent;

/**
 * public
 */
TM2P5DJsonParser* TM2P5DJsonParser::create()
{
	auto ret = new TM2P5DJsonParser();
	if(ret->init())
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
TM2P5DJsonParser::TM2P5DJsonParser()
{

}

TM2P5DJsonParser::~TM2P5DJsonParser()
{

}

bool TM2P5DJsonParser::init()
{
	return true;
}
