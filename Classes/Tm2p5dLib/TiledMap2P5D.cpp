#include "TiledMap2P5D.h"
#include "TM2P5DJsonParser.h"
#include "InfoClasses.h"
#include "TiledLayer.h"

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
	parser->retain();
	parser->parseOriginJson("origin.json");
	if(parser->isError())
	{
		parser->outputErrors();
	}

	auto layer = TM2P5DComponent::TiledLayer::create(
		parser->getMapInfo(),
		parser->getLayerInfoByName("layer-A"),
		parser->getAtlasInfoByName("atlas-A"),
		3,
		0,
		Director::getInstance()->getWinSize() * 0.5);

	this->addChild(layer);

	parser->release();

	//Touch events
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [](Touch* touch,Event* event)
	{
		return true;
	};
	listener->onTouchMoved = [this,layer](Touch* touch,Event* event)
	{
		Vec2 delta = touch->getDelta();
		Vec2 now = this->getPosition();
		this->setPosition(now.x + delta.x,now.y + delta.y);

		layer->onOriginChanged(delta);
	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener,this);

	return true;
}
