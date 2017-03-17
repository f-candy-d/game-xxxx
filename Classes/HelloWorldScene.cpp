#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "Tm2p5dLib/TiledMap2P5D.h"
#include "tm2DwdLib/CocosLockerArray.h"
#include "DLib/dlib.h"
#include <iostream>

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
    // auto tm = TiledMap2P5D::create("originfile");
    // this->addChild(tm);

    DLib::categorized_map<int, std::string> cmap;
    int cat1 = 1;
    int cat2 = 2;
    int cat3 = 3;
    int cat4 = 4;

    std::vector<std::string> source1{"a","b","c"};
    cmap().emplace(cat3, std::move(source1));

    std::cout << "cmap.get_of(cat3, 0) = " << cmap[cat3][0] << '\n';
    std::cout << "cmap.get_of(cat3, 1) = " << cmap[cat3][1] << '\n';
    std::cout << "cmap.get_of(cat3, 2) = " << cmap[cat3][2] << '\n';

    cmap[cat1].push_back("bokeboke");
    cmap[cat1].push_back("bokebokehogehoge");
    for(auto itr = cmap[cat1].begin(); itr != cmap[cat1].end(); ++itr)
        std::cout << "cat1 => " << *itr << '\n';

    cmap[cat2].push_back("2fdfd");
    cmap[cat2].push_back("2fdfdefdf");
    cmap[cat2].push_back("2fdfdfdfdfdffff");

    for(auto itr = cmap.begin(); itr != cmap.end(); ++itr)
    {
        for(auto str : itr->second)
            std::cout << "category => " << itr->first << " : value => " << str << '\n';
    }

    cmap.go_round(
        [](int category, std::string& val) {
            val += "------> write";
            category += 10;
    });

    cmap.go_round(
        [](int category, std::string val) {
        std::cout << "go-round() :: category => " << category << " : value => " << val << '\n';
    });

    std::cout << "cmap.size_of(cat1) = " << cmap.size_of(cat1) << '\n';
    std::cout << "cmap.size_of(cat4) = " << cmap.size_of(cat4) << '\n';

    const DLib::categorized_map<int, std::string> cmap2(cmap);
    auto& vec = cmap2[cat3];

    for(auto str : vec)
    {
        std::cout << "vec::" << str << '\n';
    }

    cmap2.go_round(
        [](int category, std::string val) {
        std::cout << "cmap2 :: category => " << category << " : value => " << val << '\n';
    });

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
