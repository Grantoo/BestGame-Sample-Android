//
//  StatsScene.cpp
//  HelloCpp
//
//  Created by Grantoo Developer on 12-12-17.
//
//

#include "GameHistory.h"
#include "StatsScene.h"

USING_NS_CC;
USING_NS_CC_EXT;

StatsScene::StatsScene()
: statLabel1(NULL)
, statLabel2(NULL)
, statLabel3(NULL)
, statLabel4(NULL)
, scoreLabel1(NULL)
, scoreLabel2(NULL)
, scoreLabel3(NULL)
, scoreLabel4(NULL)
{}

StatsScene::~StatsScene()
{
    CC_SAFE_RELEASE(statLabel1);
    CC_SAFE_RELEASE(statLabel2);
    CC_SAFE_RELEASE(statLabel3);
    CC_SAFE_RELEASE(statLabel4);
    CC_SAFE_RELEASE(scoreLabel1);
    CC_SAFE_RELEASE(scoreLabel2);
    CC_SAFE_RELEASE(scoreLabel3);
    CC_SAFE_RELEASE(scoreLabel4);
}

bool StatsScene::init()
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

void StatsScene::onEnter()
{
    CCLayer::onEnter();

    CCLOG("StatsSceneEntry");
    GameHistory *gHistory = GameHistory::instance();
    char string[12] = {0};
    
    // Create a label to show challenge count.
    this->statLabel1->setString("Highest Score");
    this->statLabel2->setString("Average Score");
    this->statLabel3->setString("Scoring %");
    this->statLabel4->setString("Scoring Streak");
    
    sprintf(string, "%d", gHistory->getMaxScore());
    this->scoreLabel1->setString(string);
    sprintf(string, "%.1f", gHistory->averageScore());
    this->scoreLabel2->setString(string);
    sprintf(string, "%.1f", gHistory->winPercent());
    this->scoreLabel3->setString(string);
    sprintf(string, "%d", gHistory->getCurrentWinStreak());
    this->scoreLabel4->setString(string);    
}

void StatsScene::onExit()
{
    CCLayer::onExit();

    CCLOG("onExit");
}

SEL_MenuHandler StatsScene::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName)
{
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "goBack:", StatsScene::goBack);
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "goGameCenter:", StatsScene::goGameCenter);
    return NULL;
}

SEL_CCControlHandler StatsScene::onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName)
{
    return NULL;
}

bool StatsScene::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "statLabel1", CCLabelTTF *, this->statLabel1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "statLabel2", CCLabelTTF *, this->statLabel2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "statLabel3", CCLabelTTF *, this->statLabel3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "statLabel4", CCLabelTTF *, this->statLabel4);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "scoreLabel1", CCLabelTTF *, this->scoreLabel1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "scoreLabel2", CCLabelTTF *, this->scoreLabel2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "scoreLabel3", CCLabelTTF *, this->scoreLabel3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "scoreLabel4", CCLabelTTF *, this->scoreLabel4);
    
    return false;
}

void StatsScene::goBack(CCObject *pSender) {
    CCLOG("back clicked");
    CCDirector* pDirector = CCDirector::sharedDirector();
    pDirector->popScene();
}

void StatsScene::goGameCenter(CCObject *pSender) {
    CCLOG("center clicked");
}

void StatsScene::keyBackClicked() {
    goBack(NULL);
}
