#include "AppDelegate.h"
#include "MainMenuScene.h"
#include "AppMacros.h"
#include "SimpleAudioEngine.h"

#include "platform/android/jni/JniHelper.h"
#include "JNIBridge.h"

USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    CCDirector* pDirector = CCDirector::sharedDirector();
    CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();

    pDirector->setOpenGLView(pEGLView);

    // Set the design resolution
    pEGLView->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, kResolutionShowAll);

	CCSize frameSize = pEGLView->getFrameSize();

    // In this demo, we select resource according to the frame's height.
    // If the resource size is different from design resolution size, you need to set contentScaleFactor.
    // We use the ratio of resource's height to the height of design resolution,
    // this can make sure that the resource's height could fit for the height of design resolution.

	std::vector<std::string> resDirOrders;

    // if the frame's height is larger than the height of small resource size, select large resource.
	if (frameSize.height > smallResource.size.height)
	{ 
        resDirOrders.push_back(largeResource.directory);
        CCFileUtils::sharedFileUtils()->setSearchResolutionsOrder(resDirOrders);
        pDirector->setContentScaleFactor(MAX(largeResource.size.height/designResolutionSize.height, largeResource.size.width/designResolutionSize.width));
	}
    // if the frame's height is smaller than the height of large resource size, select small resource.
	else
    { 
        resDirOrders.push_back(smallResource.directory);
        CCFileUtils::sharedFileUtils()->setSearchResolutionsOrder(resDirOrders);
        pDirector->setContentScaleFactor(MAX(smallResource.size.height/designResolutionSize.height, smallResource.size.width/designResolutionSize.width));
    }
	
    // turn on display FPS
    // pDirector->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);

#ifdef DEBUG
    // this is for test startup
    JNIBridge::useSandbox();
    // use the sandbox game key/secret pair
    JNIBridge::initialize("50ac1a38f6aae30200000001", "c38b6697-b453-99c6-bc59-b50f0eca347f");
#else
    // use the production game key/secret pair
    JNIBridge::initialize("50b665d167379a020000000b", "a918a013-842e-ceb9-19ec-c0f981894d85");
#endif
    JNIBridge::instance()->setOrientation("landscape");

    // load first scene
    cocos2d::extension::CCNodeLoaderLibrary * ccNodeLoaderLibrary = cocos2d::extension::CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
    ccNodeLoaderLibrary->registerCCNodeLoader("MainMenuScene", MainMenuSceneLoader::loader());
    cocos2d::extension::CCBReader * ccbReader = new cocos2d::extension::CCBReader(ccNodeLoaderLibrary);
    CCScene *scene = ccbReader->createSceneWithNodeGraphFromFile("ccb/MainMenuScene.ccbi");
    ccbReader->release();
    // run
    pDirector->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    cocos2d::CCLog("applicationDidEnterBackground");
    CCDirector::sharedDirector()->stopAnimation();
    CCDirector::sharedDirector()->pause();
    
    // if you use SimpleAudioEngine, it must be pause
    CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    cocos2d::CCLog("applicationWillEnterForeground");
    CCDirector::sharedDirector()->resume();
    CCDirector::sharedDirector()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    CocosDenshion::SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}
