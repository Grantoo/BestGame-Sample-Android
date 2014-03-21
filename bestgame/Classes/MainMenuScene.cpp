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
#include "platform/android/jni/JniHelper.h"

#include "GamePayload.h"
#include "JNIBridge.h"

USING_NS_CC;
USING_NS_CC_EXT;

MainMenuScene::MainMenuScene()
: challengeLabel(NULL)
, soundOnButton(NULL)
, soundOffButton(NULL)
, grantooButton(NULL)
, grantooTournButton(NULL)
{
	challengeCount = 0;
	isTournamentRunning = false;
}

MainMenuScene::~MainMenuScene()
{
    CC_SAFE_RELEASE(challengeLabel);
    CC_SAFE_RELEASE(soundOnButton);
    CC_SAFE_RELEASE(soundOffButton);
    CC_SAFE_RELEASE(grantooButton);
    CC_SAFE_RELEASE(grantooTournButton);
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
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "grantooButton", CCMenuItemImage *, this->grantooButton);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "grantooTournButton", CCMenuItemImage *, this->grantooTournButton);
    return false;
}

void MainMenuScene::onEnter()
{
    CCLayer::onEnter();
    CCLOG("MainMenuSceneEntry");

    sendResult();

    setSoundButtons();

	CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("sound/menuBGM.mp3", false);

    displayChallengeCount();
    updateChallengeCount();
    schedule(schedule_selector(MainMenuScene::updateChallengeCount), 15);

    displayTournamentInfo();
    updateTournamentInfo();
    schedule(schedule_selector(MainMenuScene::updateTournamentInfo), 15);

    updateVirtualGoods();
}

void MainMenuScene::onExit()
{
    CCLayer::onExit();
    CCLOG("MainMenuSceneExit");
}

void MainMenuScene::keyBackClicked() {
    CCDirector::sharedDirector()->end();
}

void MainMenuScene::sdkCompletedWithExit(JNIEnv* env, jobject context) {
	CCLOG("sdkCompletedWithExit");
	startCocos();

    updateChallengeCount();
    updateTournamentInfo();
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

    updateChallengeCount();
    updateTournamentInfo();
}

bool MainMenuScene::sdkSocialLogin(JNIEnv* env, jobject context, jboolean allowCache) {
	const char* result = NULL;
	bool succeeded = false;

	// handle social login

	if (succeeded) {
		std::string provider = "facebook";
		std::string email = "";
		std::string id = "";
		std::string nickname = "";
		std::string token = "";

		// retrieve social login data

		std::string json = "";
		json += '{';
		json += "\"provider\":\"" + provider + "\"";
		json += ",\"email\":\"" + email + "\"";

		if (!id.empty()) {
			json += ",\"id\":\"" + id + "\"";
		}

		if (!nickname.empty()) {
			json += ",\"nickname\":\"" + nickname + "\"";
		}

		if (!token.empty()) {
			json += ",\"token\":\"" + token + "\"";
		}

		json += '}';

		result = json.c_str();
	}

	JNIBridge::instance()->sdkSocialLoginCompleted(result);

	return true;
}

bool MainMenuScene::sdkSocialInvite(JNIEnv* env, jobject context, jstring subject, jstring longMessage, jstring shortMessage, jstring linkUrl) {
	// handle social invite

    JNIBridge::instance()->sdkSocialInviteCompleted();

	return true;
}

bool MainMenuScene::sdkSocialShare(JNIEnv* env, jobject context, jstring subject, jstring longMessage, jstring shortMessage, jstring linkUrl) {
	// handle social share

    JNIBridge::instance()->sdkSocialShareCompleted();

	return true;
}

void MainMenuScene::updatedChallengeCount(JNIEnv* env, jobject context, jint count) {
	CCLOG("updatedChallengeCount");

	challengeCount = (int) count;

	scheduleOnce(schedule_selector(MainMenuScene::displayChallengeCount), 0);
}

void MainMenuScene::updatedTournamentInfo(JNIEnv* env, jobject context, jstring name, jstring campaignName, jstring sponsorName, jlong startDate, jlong endDate, jstring logo) {
	CCLOG("updatedTournamentInfo");

	const char* tournamentName = env->GetStringUTFChars(name, NULL);
	const char* tournamentCampaignName = env->GetStringUTFChars(campaignName, NULL);
	const char* tournamentSponsorName = env->GetStringUTFChars(sponsorName, NULL);
	long tournamentStartDate = (long) startDate;
	long tournamentEndDate = (long) endDate;
	const char* tournamentLogo = env->GetStringUTFChars(logo, NULL);

	isTournamentRunning = (tournamentStartDate != 0) && (tournamentEndDate != 0) && (tournamentStartDate < tournamentEndDate);

	env->ReleaseStringUTFChars(name, tournamentName);
	env->ReleaseStringUTFChars(campaignName, tournamentCampaignName);
	env->ReleaseStringUTFChars(sponsorName, tournamentSponsorName);
	env->ReleaseStringUTFChars(logo, tournamentLogo);

	scheduleOnce(schedule_selector(MainMenuScene::displayTournamentInfo), 0);
}

void MainMenuScene::updatedVirtualGoods(JNIEnv* env, jobject context, jstring transactionId, jobjectArray virtualGoods) {
    int length = env->GetArrayLength(virtualGoods);

	for (int index = 0; index < length; index++) {
		jstring goodString = (jstring) env->GetObjectArrayElement(virtualGoods, index);
		const char* goodId = env->GetStringUTFChars(goodString, NULL);

		// process each virtual good

		env->ReleaseStringUTFChars(goodString, goodId);
	}

	const char* transId = env->GetStringUTFChars(transactionId, NULL);

	JNIBridge::instance()->acknowledgeVirtualGoods(this, transId, true);

	env->ReleaseStringUTFChars(transactionId, transId);
}

void MainMenuScene::rollbackVirtualGoods(JNIEnv* env, jobject context, jstring transactionId) {
    const char* transId = env->GetStringUTFChars(transactionId, NULL);

    // rollback the virtual goods for the given transaction ID

    env->ReleaseStringUTFChars(transactionId, transId);
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

void MainMenuScene::startCocos() {
    CCDirector::sharedDirector()->startAnimation();
    CCDirector::sharedDirector()->resume();
}

void MainMenuScene::stopCocos() {
    CCDirector::sharedDirector()->pause();
    CCDirector::sharedDirector()->stopAnimation();
}

void MainMenuScene::setSoundButtons() {
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

void MainMenuScene::delayedPlay() {
	goPlay(NULL);
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
    		jniMethodInfo.methodID);
    
    if (jniMethodInfo.env->ExceptionCheck()) {
        // exception thrown in the called method
        return false;
    }
    return true;
}

bool MainMenuScene::sendResult() {
	bool sentResult = false;

	GamePayload *payLoad = GamePayload::instance();

	if (payLoad && payLoad->getActiveFlag() && payLoad->getCompleteFlag()) {
		CCLOG("sendResult");

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

void MainMenuScene::updateChallengeCount() {
	JNIBridge::instance()->syncChallengeCounts(this);
    scheduleOnce(schedule_selector(MainMenuScene::displayChallengeCount), 3);
}

void MainMenuScene::displayChallengeCount() {
	if (challengeCount > 0) {
		char challengeCountString[12] = {0};

		sprintf(challengeCountString, "%d", challengeCount);

		challengeLabel->setString(challengeCountString);
		challengeLabel->getParent()->setVisible(true);
	} else {
		challengeLabel->getParent()->setVisible(false);
	}
}

void MainMenuScene::updateTournamentInfo() {
	JNIBridge::instance()->syncTournamentInfo(this);
	scheduleOnce(schedule_selector(MainMenuScene::displayTournamentInfo), 3);
}

void MainMenuScene::displayTournamentInfo() {
	grantooButton->setVisible(!isTournamentRunning);
	grantooTournButton->setVisible(isTournamentRunning);
}

void MainMenuScene::updateVirtualGoods() {
	JNIBridge::instance()->syncVirtualGoods(this);
}
