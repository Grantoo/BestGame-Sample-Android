//
//  RulesScene.cpp
//  HelloCpp
//
//  Created by Grantoo Developer on 12-12-17.
//
//

#include "RulesScene.h"

USING_NS_CC;
USING_NS_CC_EXT;

bool RulesScene::init()
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

void RulesScene::onEnter()
{
    CCLayer::onEnter();
    CCLOG("RulesSceneEntry");
}

SEL_MenuHandler RulesScene::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName)
{
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "goBack:", RulesScene::goBack);
    return NULL;
}

SEL_CCControlHandler RulesScene::onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName)
{
    return NULL;
}

void RulesScene::goBack(CCObject *pSender) {
    CCLOG("back clicked");
    CCDirector* pDirector = CCDirector::sharedDirector();
    pDirector->popScene();
}

void RulesScene::keyBackClicked() {
    goBack(NULL);
}
