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

    DLib::vec<10, float> vecA, vecB;
    int i = 0;
    for(auto itr = vecA.begin(); itr != vecA.end(); ++itr, ++i)
        *itr  = 10.0 * i;

    auto vecAB = vecB - vecA;

    for(auto ba = vecAB.begin(), a = vecA.begin(), b = vecB.begin();
    ba != vecAB.end(); ++ba, ++a, ++b)
        log("%lf[AB] = %lf[B] - %lf[A]",*ba,*b,*a);

    -vecAB;
    for(auto itr = vecAB.begin(); itr != vecAB.end(); ++itr)
        std::cout << "vecAB=>" << *itr << '\n';

    std::cout << "vecAB[0] = " << vecAB[0] << '\n';
    float zero = 0;
    zero *= -1;
    std::cout << "-1 * 0 = " << zero << '\n';
    if(-0 != 0) std::cout << "-1 * 0 is not equal to 0" << '\n';

    DLib::vec<10, float> v1,v2;
    v1 = v2 = vecAB;

    DLib::vec<10, float> v(777);
    for(auto itr = v.begin(); itr != v.end(); ++itr)
        std::cout << "v=>" << *itr << '\n';

        DLib::vec2<int> v123(19);
        for(auto elm : v123)
            std::cout << "elm=" << elm << '\n';

    vecA.set(1,2,3,4,5,6,7,8,9,10);
    for(auto elm : vecA)
        std::cout << "vecA=" << elm << '\n';

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
