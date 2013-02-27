//
//  RulesScene.h
//  HelloCpp
//
//  Created by Grantoo Developer on 12-12-17.
//
//

#ifndef __HelloCpp__RulesScene__
#define __HelloCpp__RulesScene__

#include "cocos2d.h"
#include "cocos-ext.h"

class RulesScene :
public cocos2d::CCLayer,
public cocos2d::extension::CCBSelectorResolver
{
public:
    
    virtual bool init();
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName);    
    virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName);

    virtual void onEnter();

    void goBack(cocos2d::CCObject *pSender);
    
    virtual void keyBackClicked();

    
    CREATE_FUNC(RulesScene);
};

class RulesSceneLoader : public cocos2d::extension::CCLayerLoader {
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(RulesSceneLoader, loader);
    
protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(RulesScene);
};


#endif /* defined(__HelloCpp__RulesScene__) */
