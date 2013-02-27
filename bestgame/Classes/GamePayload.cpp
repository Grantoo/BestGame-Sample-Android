//
//  GamePayload.m
//  bestgame
//
//  Created by Grantoo Developer on 12-11-01.
//  Copyright (c) 2012 Grantoo Developer. All rights reserved.
//

#include "GamePayload.h"
#include "support/CCUserDefault.h"

#pragma mark -
#pragma mark Public methods

// This is a GamePayload singleton class, see getInstance below
static GamePayload *s_instance = NULL;

GamePayload* GamePayload::instance(void)
{
    if (!s_instance)
    {
        s_instance = new GamePayload();
        s_instance->init();
    }
    
    return s_instance;
}

void GamePayload::clear(void)
{
    reset();
    storeGamePayload();
}

void GamePayload::store(void)
{
    storeGamePayload();
}

#pragma mark -
#pragma mark Private methods

// Made private to prevent accidental usage.
bool GamePayload::init(void) {
    reset();
    loadGamePayload();
	return true;
}

void GamePayload::reset(void) {
    tournID = "";
    matchID = "";
    seed = 0;
    round = 0;
    score = 0;
    gameType = 0;
    completeFlag = false;
    activeFlag = false;
}

void GamePayload::storeGamePayload(void)
{
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("gamePayloadVersion", 1);
    cocos2d::CCUserDefault::sharedUserDefault()->setBoolForKey("complete", completeFlag);
    cocos2d::CCUserDefault::sharedUserDefault()->setBoolForKey("active", activeFlag);
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("score", score);
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("seed", seed);
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("round", round);
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("gameType", gameType);
	cocos2d::CCUserDefault::sharedUserDefault()->setStringForKey("tournID", tournID);
	cocos2d::CCUserDefault::sharedUserDefault()->setStringForKey("matchID", matchID);
    cocos2d::CCUserDefault::sharedUserDefault()->flush();
    
}

void GamePayload::loadGamePayload(void)
{
    int gameVersion = cocos2d::CCUserDefault::sharedUserDefault()->getIntegerForKey("gamePayloadVersion");
    if (gameVersion == 1)
    {
        completeFlag = cocos2d::CCUserDefault::sharedUserDefault()->getBoolForKey("complete");
        activeFlag = cocos2d::CCUserDefault::sharedUserDefault()->getBoolForKey("active");
        score = cocos2d::CCUserDefault::sharedUserDefault()->getIntegerForKey("score");
        seed = cocos2d::CCUserDefault::sharedUserDefault()->getIntegerForKey("seed");
        round = cocos2d::CCUserDefault::sharedUserDefault()->getIntegerForKey("round");
        gameType = cocos2d::CCUserDefault::sharedUserDefault()->getIntegerForKey("gameType");
        
        setTournID(cocos2d::CCUserDefault::sharedUserDefault()->getStringForKey("tournID"));
        setMatchID(cocos2d::CCUserDefault::sharedUserDefault()->getStringForKey("matchID"));
    }
}

