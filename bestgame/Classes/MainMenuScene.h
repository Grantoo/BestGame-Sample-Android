//
//  MainMenuScene.h
//  HelloCpp
//
//  Created by Grantoo Developer on 12-12-17.
//
//

#ifndef __HelloCpp__MainMenuScene__
#define __HelloCpp__MainMenuScene__

#include "cocos2d.h"
#include "cocos-ext.h"

#include "JNIBridgeCallback.h"
#include "JNIBridgeBroadcastReceiver.h"

class MainMenuScene :
public cocos2d::CCLayer,
public cocos2d::extension::CCBSelectorResolver,
public cocos2d::extension::CCBMemberVariableAssigner,
public JNIBridgeCallback,
public JNIBridgeBroadcastReceiver
{
public:

    MainMenuScene();
    virtual ~MainMenuScene();
    
    virtual bool init();

    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName);    
    virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName);
    virtual bool onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode);

    virtual void onEnter();
    virtual void onExit();
    
    virtual void keyBackClicked();

    virtual void sdkCompletedWithExit(JNIEnv*, jobject);
    virtual void sdkCompletedWithMatch(JNIEnv*, jobject, jstring, jstring, jlong, jint, jint);
    virtual void sdkFailed(JNIEnv*, jobject, jstring, jobject);

    virtual void updatedChallengeCount(JNIEnv*, jobject, jint);
    virtual void updatedTournamentInfo(JNIEnv*, jobject, jstring, jstring, jstring, jlong, jlong, jstring);

    void goPlay(cocos2d::CCObject *pSender);
    void goLaunch(cocos2d::CCObject *pSender);
    void goStats(cocos2d::CCObject *pSender);
    void goRules(cocos2d::CCObject *pSender);
    void goFeedback(cocos2d::CCObject *pSender);
    void goSoundOff(cocos2d::CCObject *pSender);
    void goSoundOn(cocos2d::CCObject *pSender);
    
    CREATE_FUNC(MainMenuScene);
    
private:
    void startCocos();
    void stopCocos();

    void setSoundButtons();

    void delayedPlay();

    bool sendResult();
    bool sendFeeback();
    
    void updateChallengeCount();
    void displayChallengeCount();

    void updateTournamentInfo();
    void displayTournamentInfo();

    cocos2d::CCMenuItemImage    *soundOnButton;
    cocos2d::CCMenuItemImage    *soundOffButton;
    cocos2d::CCLabelTTF         *challengeLabel;
    cocos2d::CCMenuItemImage    *grantooButton;
    cocos2d::CCMenuItemImage    *grantooTournButton;

    int challengeCount;
    bool isTournamentRunning;
};

class MainMenuSceneLoader : public cocos2d::extension::CCLayerLoader {
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(MainMenuSceneLoader, loader);
    
protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(MainMenuScene);
};


#endif /* defined(__HelloCpp__MainMenuScene__) */
