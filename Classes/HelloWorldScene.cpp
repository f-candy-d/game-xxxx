#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "Tm2p5dLib/TiledMap2P5D.h"
#include "tm2DwdLib/CocosLockerArray.h"
// #include "DLib/dlib.h"
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

    DLib::locker_array<int> locker(10);
    locker.set_dummy_baggage(0);
    auto key1 = locker.rent_locker(4);
    std::cout << "rented 4 lockers! (key1)" << '\n';
    for(size_t i = 0; i < key1.size; ++i)
        locker.leave_baggage(key1, i, i * 10);

    for(auto elm : locker)
        std::cout << "range_based loop (all) => " << elm << '\n';

    auto key2 = locker.rent_locker(3);
    std::cout << "rented 3 lockers! (key2)" << '\n';
    for(size_t i = 0; i < key2.size; ++i)
        locker.leave_baggage(key2, i, i * key2.size + key2.size);
    for(auto elm : locker)
        std::cout << "range_based loop (all) => " << elm << '\n';

    for(auto itr = locker.begin_of(key1); itr != locker.end_of(key1); ++itr)
        std::cout << "loop using iterator (key1) => " << *itr << '\n';

    int removed = locker.remove_baggage(key1, 2);
    std::cout << "removed baggage at index 2!(key1) removed => " << removed << '\n';

    std::cout << "get baggaged at index of 1 (key2) => " << locker.get_baggage(key2, 1) << '\n';

    for(auto elm : locker)
        std::cout << "range_based loop (all) => " << elm << '\n';

    std::cout << "replace with 999 (key2) at index 2" << '\n';
    removed = locker.replace_baggage(key2, 2, 999);
    std::cout << "removed => " << removed << '\n';

    for(auto elm : locker)
        std::cout << "range_based loop (all) => " << elm << '\n';

    auto key3 = locker.rent_locker(10);
    std::cout << "rented 10 lockers! (key3)" << '\n';
    for(size_t i = 0; i < key3.size; ++i)
        locker.leave_baggage(key3, i, i * key3.size + key3.size);

    for(auto elm : locker)
        std::cout << "range_based loop (all) => " << elm << '\n';

    std::cout << "get baggaged at index of 1 (key2) => " << locker.get_baggage(key2, 1) << '\n';

    removed = locker.remove_baggage(key1, 3);
    std::cout << "removed baggage at index 3!(key1) removed => " << removed << '\n';
    removed = locker.remove_baggage(key2, 2);
    std::cout << "removed baggage at index 2!(key2) removed => " << removed << '\n';

    std::cout << "replace with 777 (key3) at index 5" << '\n';
    removed = locker.replace_baggage(key3, 5, 777);
    std::cout << "removed => " << removed << '\n';

    for(auto itr = locker.begin_of(key3); itr != locker.end_of(key3); ++itr)
        std::cout << "loop using iterator (key3) => " << *itr << '\n';

    log("key1.rent=%zd key1.in_use=%zd",key1.size,key1.in_use);
    log("key2.rent=%zd key2.in_use=%zd",key2.size,key2.in_use);
    log("key3.rent=%zd key3.in_use=%zd",key3.size,key3.in_use);

    for(auto elm : locker)
        std::cout << "range_based loop (all) => " << elm << '\n';

    std::cout << "locker.return_locker(key2)=>" << locker.return_locker(key2) << '\n';
    std::cout << "returned 3 lockers! (key2)" << '\n';

    for(auto itr = locker.begin_of(key1); itr != locker.end_of(key1); ++itr)
        std::cout << "loop using iterator (key1) => " << *itr << '\n';

    for(auto itr = locker.begin_of(key3); itr != locker.end_of(key3); ++itr)
        std::cout << "loop using iterator (key3) => " << *itr << '\n';

    for(auto elm : locker)
        std::cout << "range_based loop (all) => " << elm << '\n';

    std::cout << "loop(key2)" << '\n';
    std::cout << "key2.isbroken=" << key2.is_broken << '\n';
    for(auto itr = locker.begin_of(key2); itr != locker.end_of(key2); ++itr)
        std::cout << "loop using iterator (key2) => " << *itr << '\n';

    removed = locker.get_baggage(key2, 3);
    std::cout << "get baggage at index 3!(key2) removed => " << removed << '\n';
    removed = locker.remove_baggage(key2, 2);
    std::cout << "removed baggage at index 2!(key2) removed => " << removed << '\n';

    std::cout << "replace with 777 (key2) at index 5" << '\n';
    removed = locker.replace_baggage(key2, 5, 777);
    std::cout << "removed => " << removed << '\n';

    // making spare-key part1
    // key2 = key3;
    key2 = key3.spare();

    for(auto itr = locker.begin_of(key2); itr != locker.end_of(key2); ++itr)
        std::cout << "loop using iterator (key2 = key3) => " << *itr << '\n';

    // making spare-key part2
    DLib::locker_array<int>::key key4(key1);
    for(auto itr = locker.begin_of(key4); itr != locker.end_of(key4); ++itr)
        std::cout << "loop using iterator (key4 = key1) => " << *itr << '\n';

        for(auto itr = locker.begin_of(key3); itr != locker.end_of(key3); ++itr)
            std::cout << "loop using iterator (key3) => " << *itr << '\n';

        auto key5 = locker.rent_locker(5);
        DLib::locker_array<int> locker2;
        auto key6 = locker2.rent_locker(5);
        if(locker.leave_baggage(key5, 0, 777))
            std::cout << "leave baggage '777' in locker at index 0 with key5" << '\n';
        else
            std::cout << "could't leave baggage '777' in locker at index 0 with key5" << '\n';

        if(locker.leave_baggage(key6, 1, 666))
            std::cout << "leave baggage '666' in locker at index 1 with key6" << '\n';
        else
            std::cout << "could't leave baggage '666' in locker at index 1 with key6" << '\n';

        if(locker2.leave_baggage(key6, 1, 666))
            std::cout << "leave baggage '666' in locker2 at index 1 with key6" << '\n';
        else
            std::cout << "could't leave baggage '666' in locker2 at index 1 with key6" << '\n';

        DLib::locker_array<int>::key key7(key5);
        if(locker.leave_baggage(key7, 3, 111))
            std::cout << "leave baggage '111' in locker at index 3 with key7(= key5)" << '\n';
        else
            std::cout << "could't leave baggage '111' in locker at index 3 with key7(= key5)" << '\n';

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
