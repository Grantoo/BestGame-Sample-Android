//
//  GameScene.h
//  HelloCpp
//
//  Created by Grantoo Developer on 12-12-17.
//
//

#ifndef __HelloCpp__GameScene__
#define __HelloCpp__GameScene__

#include "cocos2d.h"
#include "cocos-ext.h"

class GameScene :
public cocos2d::CCLayer,
public cocos2d::extension::CCBSelectorResolver,
public cocos2d::extension::CCBMemberVariableAssigner
{
public:
    GameScene();
    virtual ~GameScene();

    virtual bool init();

    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName);    
    virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName);
    virtual bool onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode);

    virtual void onEnter();
    virtual void onExit();
    
    virtual void keyBackClicked();


    void goBack(cocos2d::CCObject *pSender);
    void goScore(cocos2d::CCObject *pSender);
    void goSoundOff(cocos2d::CCObject *pSender);
    void goSoundOn(cocos2d::CCObject *pSender);
    void goStop(cocos2d::CCObject *pSender);
    
    void setAnimationManager(cocos2d::extension::CCBAnimationManager *pAnimationManager);

    CREATE_FUNC(GameScene);
    
private:
    void finalize(void);
    
    void floaterFinished(CCNode* node);
    void createFloater(const char *text, const cocos2d::ccColor3B& color, const cocos2d::CCPoint& location, const cocos2d::CCPoint& destination);
    
    void setSoundButtons(void);
    
    void delayedExit(float dt);
    
    void mysrandom(unsigned int seed);
	unsigned int myrandom(void);

    cocos2d::CCMenuItemImage    *backButton;
    cocos2d::CCMenuItemImage    *goButton;
    cocos2d::CCMenuItemImage    *stopButton;
    
    cocos2d::CCMenuItemImage    *soundOnButton;
    cocos2d::CCMenuItemImage    *soundOffButton;
    
    cocos2d::CCLabelTTF         *scoreLabel;
    
    float           scorePitch;
    int             score;
    int             presses;
    bool            goBackPressed;
    
    unsigned int        randseed_w;
    unsigned int        randseed_z;
    
    cocos2d::extension::CCBAnimationManager *animationManager;
};

class GameSceneLoader : public cocos2d::extension::CCLayerLoader {
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(GameSceneLoader, loader);
    
protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(GameScene);
};


#endif /* defined(__HelloCpp__GameScene__) */
