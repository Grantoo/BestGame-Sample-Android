//
//  GameScene.cpp
//  HelloCpp
//
//  Created by Grantoo Developer on 12-12-17.
//
//

#include "GameScene.h"
#include "MainMenuScene.h"
#include "GameHistory.h"
#include "SimpleAudioEngine.h"
#include "GamePayload.h"

USING_NS_CC;
USING_NS_CC_EXT;

#define OPTIONS_GAMETYPE_RANDOM_VALUE 0
#define OPTIONS_GAMETYPE_SAMESY_VALUE 1

GameScene::GameScene()
: backButton(NULL)
, goButton(NULL)
, stopButton(NULL)
, soundOnButton(NULL)
, soundOffButton(NULL)
, scoreLabel(NULL)
, animationManager(NULL)
{}

GameScene::~GameScene()
{
    CC_SAFE_RELEASE(backButton);
    CC_SAFE_RELEASE(goButton);
    CC_SAFE_RELEASE(stopButton);
    CC_SAFE_RELEASE(soundOnButton);
    CC_SAFE_RELEASE(soundOffButton);
    CC_SAFE_RELEASE(scoreLabel);


    CC_SAFE_RELEASE_NULL(animationManager);
}

void GameScene::mysrandom(unsigned int seed)
{
    if (seed == 0) {
        seed = 7777;
    }
    randseed_w = seed;
    randseed_z = randseed_w/2;
}

unsigned int GameScene::myrandom(void)
{
    randseed_w = 36969 * (randseed_w & 65535) + (randseed_w >> 16);
    randseed_z = 18000 * (randseed_z & 65535) + (randseed_z >> 16);
    unsigned int value = (randseed_z << 16) + randseed_w;
    return value;
}

bool GameScene::init()
{
    if (!CCLayer::init())
    {
        return false;
    }
    
    bool seeded = false;
    goBackPressed = false;

    GamePayload *payLoad = GamePayload::instance();
    if (payLoad != NULL && payLoad->getActiveFlag()) {
        if (payLoad->getGameType() == OPTIONS_GAMETYPE_SAMESY_VALUE) {
            int seed = payLoad->getSeed();
            int round = payLoad->getRound();
            mysrandom(seed+round);
            seeded = true;
        }
    }
    
    if (!seeded) {
        mysrandom(time(0));
    }
    
    score = 0;
    presses = 0;
    scorePitch = 0.5f;

    CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("sound/scoreSFX.wav");
    CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("sound/failSFX.wav");
    CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("sound/lockSFX.wav");

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    this->setKeypadEnabled(true);
#endif
    
    return true;
}

void GameScene::onEnter()
{
    CCLayer::onEnter();
    CCLOG("GameSceneEntry");
    
    setSoundButtons();
    CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("sound/ingameBGM.mp3", true);
}

void GameScene::onExit()
{
    CCLayer::onExit();
    CCLOG("onExit");
    CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
}

SEL_MenuHandler GameScene::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName)
{
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "goBack:", GameScene::goBack);
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "goScore:", GameScene::goScore);
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "goStop:", GameScene::goStop);
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "goSoundOn:", GameScene::goSoundOn);
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "goSoundOff:", GameScene::goSoundOff);

    return NULL;
}

SEL_CCControlHandler GameScene::onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName)
{
    return NULL;
}

bool GameScene::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "backButton", CCMenuItemImage *, this->backButton);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "goButton", CCMenuItemImage *, this->goButton);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "stopButton", CCMenuItemImage *, this->stopButton);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "soundOnButton", CCMenuItemImage *, this->soundOnButton);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "soundOffButton", CCMenuItemImage *, this->soundOffButton);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "scoreLabel", CCLabelTTF *, this->scoreLabel);
    
    return false;
}

void GameScene::setAnimationManager(cocos2d::extension::CCBAnimationManager *pAnimationManager)
{
    CC_SAFE_RELEASE_NULL(animationManager);
    animationManager = pAnimationManager;
    CC_SAFE_RETAIN(animationManager);
}

void GameScene::delayedExit(float dt) {
    goBack(NULL);
}

void GameScene::goBack(CCObject *pSender) {
    CCLOG("goBack clicked");
    this->unschedule(schedule_selector(GameScene::delayedExit));
    if (!goBackPressed) {
        goBackPressed = true;
        cocos2d::extension::CCNodeLoaderLibrary * ccNodeLoaderLibrary = cocos2d::extension::CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
        ccNodeLoaderLibrary->registerCCNodeLoader("MainMenuScene", MainMenuSceneLoader::loader());
        cocos2d::extension::CCBReader * ccbReader = new cocos2d::extension::CCBReader(ccNodeLoaderLibrary);
        CCScene *scene = ccbReader->createSceneWithNodeGraphFromFile("ccb/MainMenuScene.ccbi");
        ccbReader->release();
        // run
        CCDirector* pDirector = CCDirector::sharedDirector();
        pDirector->replaceScene(scene);
    }
}

void GameScene::goScore(CCObject *pSender) {
    CCLOG("goScore clicked");
    int whammy = myrandom() % 100;
    if (whammy > 6 || score == 0 || presses < 3) {
        // we are safe
        int newup = (myrandom() % 950) + 50;
        score += newup;
        presses ++;
        
        scorePitch += 0.1;
        if (scorePitch > 2.0) {
            scorePitch = 2.0;
        }

        char string[12] = {0};
        sprintf(string, "+%d", newup);
        createFloater(string, ccc3(0,142,41), goButton->getPosition(), scoreLabel->getPosition());

        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/scoreSFX.wav", false, scorePitch, 1.0, 0.9);
        
    } else {
        
        // we are done
        score = 0;
 
        this->finalize();
 
        createFloater("FAIL", ccc3(0,142,41), goButton->getPosition(), scoreLabel->getPosition());
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/failSFX.wav", false);
        
        this->backButton->setVisible(true);
        this->stopButton->setEnabled(false);
        this->goButton->setEnabled(false);

        animationManager->runAnimationsForSequenceNamed("GameOver");
    }
}

void GameScene::goStop(CCObject *pSender) {
    CCLOG("goStop clicked");
    this->finalize();

    createFloater("LOCK", ccRED, stopButton->getPosition(), scoreLabel->getPosition());
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound/lockSFX.wav", false);
    
    char string[12] = {0};
    sprintf(string, "%d", score);
    this->scoreLabel->setString(string);

    this->backButton->setVisible(true);
    this->stopButton->setEnabled(false);
    this->goButton->setEnabled(false);

    animationManager->runAnimationsForSequenceNamed("GameOver");
}

void GameScene::finalize(void) 
{
    CCLOG("finalize");
    // load up the payload if needed
    GamePayload *payLoad = GamePayload::instance();
    if (payLoad && payLoad->getActiveFlag())
    {
        CCLOG("setting Payload");

        payLoad->setScore(score);
        payLoad->setCompleteFlag(true);
        payLoad->store();
    }
    
    // add to History
    GameHistory::instance()->updateHistoryWithScore(score, presses);
    
    // cue up the kick out code
    this->scheduleOnce(schedule_selector(GameScene::delayedExit), 3);
}

void GameScene::floaterFinished(CCNode* node)
{
    CCLabelTTF *floater = (CCLabelTTF *)node;
    
    char string[12] = {0};
    sprintf(string, "%d", score);
    this->scoreLabel->setString(string);
    if (score == 0) {
        this->scoreLabel->setColor(ccRED);
    }
    
    this->removeChild(floater, true);
}

void GameScene::createFloater(const char *text, const cocos2d::ccColor3B& color, const cocos2d::CCPoint& location, const cocos2d::CCPoint& destination)
{
    int fontSize = 28;
    // this needs to get multiplied by something I think
    
    CCLabelTTF *floater = CCLabelTTF::create(text, "Arial", fontSize);
    floater->setColor(color);
    floater->setPosition(location);
    floater->setScale(0.5);
    this->addChild(floater);
    
    CCActionInterval* moveaction = CCMoveTo::create(1, destination);
    CCFiniteTimeAction* easeaction = CCEaseOut::create(moveaction, 2);
    CCFiniteTimeAction* fadeaction = CCFadeOut::create(1);
    CCFiniteTimeAction* scaleaction = CCScaleTo::create(1,1);
    CCFiniteTimeAction* spawnaction = CCSpawn::create(easeaction, fadeaction, scaleaction, NULL);
    CCCallFuncN* mycallfunc = CCCallFuncN::create(this, callfuncN_selector(GameScene::floaterFinished));
    CCAction* sequence = CCSequence::create(spawnaction, mycallfunc, NULL);
    floater->runAction(sequence);
}

#pragma mark -
#pragma mark sound button functions

void GameScene::setSoundButtons(void) {
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

void GameScene::goSoundOff(CCObject *pSender) {
    CCLOG("goSoundOff clicked");
    
    CocosDenshion::SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(1.0f);
    CocosDenshion::SimpleAudioEngine::sharedEngine()->setEffectsVolume(1.0f);
    
    setSoundButtons();
}

void GameScene::goSoundOn(CCObject *pSender) {
    CCLOG("goSoundOn clicked");
    
    CocosDenshion::SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(0.0f);
    CocosDenshion::SimpleAudioEngine::sharedEngine()->setEffectsVolume(0.0f);
    
    setSoundButtons();
}

void GameScene::keyBackClicked() {
    if (backButton->isVisible()) {
        goBack(NULL);
    }
}
