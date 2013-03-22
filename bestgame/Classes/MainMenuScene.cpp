//  MainMenuScene.cpp
//  HelloCpp
//
//  Created by Grantoo Developer on 12-12-17.
//
//

#include "MainMenuScene.h"
#include "StatsScene.h"
#include "RulesScene.h"
#include "GameScene.h"
#include "SimpleAudioEngine.h"
#include "GamePayload.h"
#include "platform/android/jni/JniHelper.h"
#include "JNIBridge.h"

USING_NS_CC;
USING_NS_CC_EXT;

MainMenuScene::MainMenuScene()
: challengeLabel(NULL)
, soundOnButton(NULL)
, soundOffButton(NULL)
{}

MainMenuScene::~MainMenuScene()
{
    CC_SAFE_RELEASE(challengeLabel);
    CC_SAFE_RELEASE(soundOnButton);
    CC_SAFE_RELEASE(soundOffButton);
}

bool MainMenuScene::init()
{
    if (!CCLayer::init())
    {
        return false;
    }
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    this->setKeypadEnabled(true);
#endif
    
    return true;
}

void MainMenuScene::onEnter()
{
    CCLayer::onEnter();
    CCLOG("MainMenuSceneEntry");
    
    setSoundButtons();

    if (!sendResult()) {
        CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("sound/menuBGM.mp3", false);
    }

    updateChallengeCount(0);
    schedule(schedule_selector(MainMenuScene::updateChallengeCount),15);
}

void MainMenuScene::onExit()
{
    CCLayer::onExit();
    CCLOG("MainMenuSceneExit");
}

SEL_MenuHandler MainMenuScene::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName)
{
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "goPlay:", MainMenuScene::goPlay);
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "goLaunch:", MainMenuScene::goLaunch);
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "goStats:", MainMenuScene::goStats);
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "goRules:", MainMenuScene::goRules);
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "goFeedback:", MainMenuScene::goFeedback);
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "goSoundOff:", MainMenuScene::goSoundOff);
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "goSoundOn:", MainMenuScene::goSoundOn);
    return NULL;
}

SEL_CCControlHandler MainMenuScene::onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName)
{
    return NULL;
}

bool MainMenuScene::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "challengeLabel", CCLabelTTF *, this->challengeLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "soundOnButton", CCMenuItemImage *, this->soundOnButton);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "soundOffButton", CCMenuItemImage *, this->soundOffButton);    
    return false;
}

void MainMenuScene::updateChallengeCount(float dt) {
    JNIBridge::instance()->syncChallengeCounts();
    scheduleOnce(schedule_selector(MainMenuScene::displayChallengeCount), 3);
}

void MainMenuScene::displayChallengeCount(float dt) {
    int challengeCount = JNIBridge::instance()->getChallengeCounts();
    if (challengeCount > 0) {
        char string[12] = {0};
        sprintf(string, "%d", challengeCount);
        challengeLabel->setString(string);
        challengeLabel->getParent()->setVisible(true);
    } else {
        challengeLabel->getParent()->setVisible(false);
    }
}

bool MainMenuScene::sendResult(void) {
    bool sentResult = false;
    GamePayload *payLoad = GamePayload::instance();
    if (payLoad && payLoad->getActiveFlag() && payLoad->getCompleteFlag()) {
        CCLOG("sendResult");
        //stopCocos();
        CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("sound/outgameBGM.mp3", false);

        JNIBridge::instance()->launchWithMatchResult(
        		payLoad->getTournID().c_str(),
        		payLoad->getMatchID().c_str(),
        		payLoad->getScore(),
        		this);

        payLoad->clear();
        sentResult = true;
    }
    return sentResult;
    
}

void MainMenuScene::delayedPlay(float dt) {
	goPlay(NULL);
}

void MainMenuScene::goPlay(CCObject *pSender) {
    CCLOG("goPlay clicked");
    cocos2d::extension::CCNodeLoaderLibrary * ccNodeLoaderLibrary = cocos2d::extension::CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
    ccNodeLoaderLibrary->registerCCNodeLoader("GameScene", GameSceneLoader::loader());
    cocos2d::extension::CCBReader * ccbReader = new cocos2d::extension::CCBReader(ccNodeLoaderLibrary);
    
    //slightly different here as we need to let the new layer know what the animationManager is

    CCNode *node = ccbReader->readNodeGraphFromFile("ccb/GameScene.ccbi");
    ((GameScene*)node)->setAnimationManager(ccbReader->getAnimationManager());
    CCScene *scene = CCScene::create();
    scene->addChild(node);
    
    ccbReader->release();
    // run
    CCDirector* pDirector = CCDirector::sharedDirector();
    pDirector->replaceScene(scene);
}

void MainMenuScene::goLaunch(CCObject *pSender) {
    CCLOG("goLaunch clicked");
    stopCocos();
    JNIBridge::instance()->launch(this);
}

void MainMenuScene::goStats(CCObject *pSender) {
    CCLOG("goStats clicked");
    cocos2d::extension::CCNodeLoaderLibrary * ccNodeLoaderLibrary = cocos2d::extension::CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
    ccNodeLoaderLibrary->registerCCNodeLoader("StatsScene", StatsSceneLoader::loader());
    cocos2d::extension::CCBReader * ccbReader = new cocos2d::extension::CCBReader(ccNodeLoaderLibrary);
    CCScene *scene = ccbReader->createSceneWithNodeGraphFromFile("ccb/StatsScene.ccbi");
    ccbReader->release();
    // run
    CCDirector* pDirector = CCDirector::sharedDirector();
    pDirector->pushScene(scene);
}

void MainMenuScene::goRules(CCObject *pSender) {
    CCLOG("goRules clicked");
    cocos2d::extension::CCNodeLoaderLibrary * ccNodeLoaderLibrary = cocos2d::extension::CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
    ccNodeLoaderLibrary->registerCCNodeLoader("RulesScene", RulesSceneLoader::loader());
    cocos2d::extension::CCBReader * ccbReader = new cocos2d::extension::CCBReader(ccNodeLoaderLibrary);
    CCScene *scene = ccbReader->createSceneWithNodeGraphFromFile("ccb/HowToPlayScene.ccbi");
    ccbReader->release();
    // run
    CCDirector* pDirector = CCDirector::sharedDirector();
    pDirector->pushScene(scene);
}

void MainMenuScene::goFeedback(CCObject *pSender) {
    CCLOG("goFeedback clicked");
    sendFeeback();
}

#pragma mark -
#pragma mark sound button functions

void MainMenuScene::setSoundButtons(void) {
    float musicLevel = CocosDenshion::SimpleAudioEngine::sharedEngine()->getBackgroundMusicVolume();
    float sfxLevel = CocosDenshion::SimpleAudioEngine::sharedEngine()->getEffectsVolume();
    
    bool currentlyOn = true;
    
    if (musicLevel == 0.0f && sfxLevel == 0.0f) {
        currentlyOn = false;
    }
    
    if (currentlyOn) {
        soundOnButton->setVisible(true);
        soundOffButton->setVisible(false);
    } else {
        soundOnButton->setVisible(false);
        soundOffButton->setVisible(true);
    }
    
}

void MainMenuScene::goSoundOff(CCObject *pSender) {
    CCLOG("goSoundOff clicked");
    
    CocosDenshion::SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(1.0f);
    CocosDenshion::SimpleAudioEngine::sharedEngine()->setEffectsVolume(1.0f);
    
    setSoundButtons();
}

void MainMenuScene::goSoundOn(CCObject *pSender) {
    CCLOG("goSoundOn clicked");

    CocosDenshion::SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(0.0f);
    CocosDenshion::SimpleAudioEngine::sharedEngine()->setEffectsVolume(0.0f);
    
    setSoundButtons();
}

void MainMenuScene::keyBackClicked() {
    CCDirector::sharedDirector()->end();
}

void MainMenuScene::sdkCompletedWithExit(JNIEnv* env, jobject context) {
	CCLOG("sdkCompletedWithExit");
	startCocos();
}

void MainMenuScene::sdkCompletedWithMatch(JNIEnv* env, jobject context, jstring tournId, jstring matchId, jlong seed, jint round, jint gameType) {
	CCLOG("sdkCompletedWithMatch");
	startCocos();

	GamePayload* payload = GamePayload::instance();

	if (payload) {
		payload->setTournID(JniHelper::jstring2string(tournId));
		payload->setMatchID(JniHelper::jstring2string(matchId));
		payload->setSeed((int) seed);
		payload->setRound((int) round);
		payload->setGameType((int) gameType);
		payload->setActiveFlag(true);
		payload->setCompleteFlag(false);
	}

    this->scheduleOnce(schedule_selector(MainMenuScene::delayedPlay), 0);
}

void MainMenuScene::sdkFailed(JNIEnv* env, jobject context, jstring message, jobject result) {
	CCLOG("sdkFailed");
	startCocos();
}

void MainMenuScene::startCocos() {
    CCDirector::sharedDirector()->startAnimation();
    CCDirector::sharedDirector()->resume();
}

void MainMenuScene::stopCocos() {
    CCDirector::sharedDirector()->pause();
    CCDirector::sharedDirector()->stopAnimation();
}

bool MainMenuScene::sendFeeback()
{
    JniMethodInfo jniMethodInfo;
    if (!JniHelper::getStaticMethodInfo(
                                        jniMethodInfo,
                                        "org/grantoo/bestgame/bestgame",
                                        "sendFeedback",
                                        "()V")) {
        // method info could not be obtained
        return false;
    }
    
    jniMethodInfo.env->CallStaticVoidMethod(
                                            jniMethodInfo.classID,
                                            jniMethodInfo.methodID
                                            );
    
    if (jniMethodInfo.env->ExceptionCheck()) {
        // exception thrown in the called method
        return false;
    }
    return true;
}

