#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include <iostream>
#include "dlib_cc/lts_map/block.h"
#include "dlib_cc/lts_map/lts_layer.h"
#include "dlib_cc/lts_map/info_classes.h"
#include "dlib_cc/lts_map/info_json_parser.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();

    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));

    closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    //
    // auto label = Label::createWithTTF("Hello World", "fonts/Marker Felt.ttf", 24);
    //
    // // position the label on the center of the screen
    // label->setPosition(Vec2(origin.x + visibleSize.width/2,
    //                         origin.y + visibleSize.height - label->getContentSize().height));
    //
    // // add the label as a child to this layer
    // this->addChild(label, 1);
    //
    // // add "HelloWorld" splash screen"
    // auto sprite = Sprite::create("HelloWorld.png");
    //
    // // position the sprite on the center of the screen
    // sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    //
    // // add the sprite as a child to this layer
    // this->addChild(sprite, 0);

    //NOTE : TEST

    lts_map::unit::AtlasInfo mAtlas;
    auto fBlock = lts_map::unit::Block::Create(dlib::size<size_t>(10, 10));
    std::cout << "fBlock::GridSize = " << fBlock->size() << '\n';
    std::cout << "fBlock::GridPoint = " << fBlock->position() << '\n';

    auto parser = lts_map::unit::InfoJsonParser::Create();
    parser->ParseJson("index.json");
    // // log map-info
    // lts_map::unit::DebugLog(parser->map_info());
    // for(auto nameb : parser->map_info()->architecture)
    // {
    //     std::cout << "IN " << nameb << '\n';
    //     // log bundler-info
    //     lts_map::unit::DebugLog(parser->bundler_info(nameb));
    //     for(auto namel : parser->bundler_info(nameb)->architecture)
    //     {
    //         std::cout << "IN " << namel << '\n';
    //         // log layer-info
    //         auto layer = parser->layer_info(namel);
    //         lts_map::unit::DebugLog(layer);
    //         // log atlas-info
    //         auto namea = layer->atlas_name;
    //         lts_map::unit::DebugLog(parser->atlas_info(namea));
    //         auto namet = layer->terrain_name;
    //         // log terrain-info
    //         lts_map::unit::DebugLog(parser->terrain_info(namet));
    //
    //         std::cout << "END " << namel << '\n';
    //     }
    //     std::cout << "END " << nameb << '\n';
    // }
    std::string layername("layer-A");
    std::string atlasname("atlas-A");
    std::string terrainname("terrain-A");
    auto layer = lts_map::unit::LTSLayer::Create(
        parser->layer_info(layername),
        parser->atlas_info(atlasname),
        parser->terrain_info(terrainname), false);
    // layer->ScaleTile(0.5, false);
    layer->OptimizeBlockSize();
    layer->InitLayer();
    layer->MoveTo(10,15);
    // layer->MoveTo(0, 0);

    this->addChild(layer);

    //Touch events
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [](Touch* touch,Event* event)
	{
		return true;
	};
	listener->onTouchMoved = [this,layer](Touch* touch,Event* event)
	{
		Vec2 delta = touch->getDelta();
		Vec2 now = layer->getPosition();
		layer->setPosition(now.x + delta.x,now.y + delta.y);
	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener,this);

    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() and exit(0) as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}
