#include "TiledMap2P5D.h"
#include "TM2P5DJsonParser.h"
#include "InfoClasses.h"

USING_NS_CC;

/**
 * public
 */
TiledMap2P5D* TiledMap2P5D::create(std::string origin)
{
	auto ret = new TiledMap2P5D();
	if(ret->initWithOrigin(origin))
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
TiledMap2P5D::TiledMap2P5D()
{

}

TiledMap2P5D::~TiledMap2P5D()
{

}

/**
 * private
 */
bool TiledMap2P5D::initWithOrigin(std::string origin)
{
	if(!Node::init())
		return false;

	auto parser = TM2P5DComponent::TM2P5DJsonParser::create();
	parser->parseOriginJson("origin.json");
	return true;
}
