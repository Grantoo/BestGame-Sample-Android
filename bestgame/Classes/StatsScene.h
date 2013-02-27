//
//  StatsScene.h
//  HelloCpp
//
//  Created by Grantoo Developer on 12-12-17.
//
//

#ifndef __HelloCpp__StatsScene__
#define __HelloCpp__StatsScene__

#include "cocos2d.h"
#include "cocos-ext.h"

class StatsScene :
public cocos2d::CCLayer,
public cocos2d::extension::CCBSelectorResolver,
public cocos2d::extension::CCBMemberVariableAssigner
{
public:
    StatsScene();
    virtual ~StatsScene();

    virtual bool init();

    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName);    
    virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName);
    virtual bool onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode);

    virtual void onEnter();
    virtual void onExit();

    virtual void keyBackClicked();
    
    void goBack(cocos2d::CCObject *pSender);
    void goGameCenter(cocos2d::CCObject *pSender);
    
    CREATE_FUNC(StatsScene);
    
private:
    cocos2d::CCLabelTTF      *statLabel1;
    cocos2d::CCLabelTTF      *statLabel2;
    cocos2d::CCLabelTTF      *statLabel3;
    cocos2d::CCLabelTTF      *statLabel4;
    cocos2d::CCLabelTTF      *scoreLabel1;
    cocos2d::CCLabelTTF      *scoreLabel2;
    cocos2d::CCLabelTTF      *scoreLabel3;
    cocos2d::CCLabelTTF      *scoreLabel4;
};

class StatsSceneLoader : public cocos2d::extension::CCLayerLoader {
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(StatsSceneLoader, loader);
    
protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(StatsScene);
};


#endif /* defined(__HelloCpp__StatsScene__) */
