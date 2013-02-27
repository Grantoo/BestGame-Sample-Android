//
//  GameHistory.m
//  bestgame
//
//  Created by Grantoo Developer on 12-11-13.
//  Copyright (c) 2012 Grantoo Developer. All rights reserved.
//

#include "support/CCUserDefault.h"
#include "GameHistory.h"

#pragma mark -
#pragma mark Public methods

// This is a GrantooLib singleton class, see getInstance below
static GameHistory *s_instance = NULL;

GameHistory* GameHistory::instance(void)
{
    if (!s_instance)
    {
        s_instance = new GameHistory();
        s_instance->init();
    }
    
    return s_instance;
}

float GameHistory::averageScore(void) {
    if (totalGames > 0) {
        return (float)totalScore / (float) totalGames;
    }
    return 0.0f;
}

float GameHistory::averageTaps(void)  {
    if (totalGames > 0) {
        return (float)totalTaps / (float) totalGames;
    }
    return 0.0f;
}

float GameHistory::winPercent(void)  {
    if (totalGames > 0) {
        return (float)totalWins * 100.0f / (float) totalGames;
    }
    return 0.0f;
}

void GameHistory::updateHistoryWithScore(int score, int taps) {
    totalGames++;
    totalScore += score;
    totalTaps += taps;
    maxScore = score > maxScore ? score : maxScore;
    
    if (score != 0) {
        totalWins++;
        maxTapsWin =  taps > maxTapsWin ? taps : maxTapsWin;
        currentWinStreak++;
        currentLossStreak = 0;
        maxWinStreak = currentWinStreak > maxWinStreak ? currentWinStreak : maxWinStreak;
    } else {
        maxTapsLoss =  taps > maxTapsLoss ? taps : maxTapsLoss;
        currentLossStreak++;
        currentWinStreak = 0;
        maxLossStreak = currentLossStreak > maxLossStreak ? currentLossStreak : currentLossStreak;
    }
    
    this->storeGameHistory();
}

#pragma mark -
#pragma mark Private methods

// Made private to prevent accidental usage.
bool GameHistory::init(void)
{
    this->reset();
    this->loadGameHistory();
	return true;
}

void GameHistory::reset(void) {
    maxScore = 0;
    maxTapsWin = 0;
    maxTapsLoss = 0;
    maxWinStreak = 0;
    maxLossStreak = 0;
    totalScore = 0;
    totalGames = 0;
    totalWins = 0;
    totalTaps = 0;
    currentWinStreak = 0;
    currentLossStreak = 0;    
}

void GameHistory::storeGameHistory(void) {
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("gameHistoryVersion", 1);
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("maxScore", this->maxScore);
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("maxTapsWin", this->maxTapsWin);
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("maxTapsLoss", this->maxTapsLoss);
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("maxWinStreak", this->maxWinStreak);
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("maxLossStreak", this->maxLossStreak);
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("totalScore", this->totalScore);
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("totalGames", this->totalGames);
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("totalWins", this->totalWins);
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("totalTaps", this->totalTaps);
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("currentWinStreak", this->currentWinStreak);
    cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("currentLossStreak", this->currentLossStreak);    
    cocos2d::CCUserDefault::sharedUserDefault()->flush();
    
}

void GameHistory::loadGameHistory(void) {
    int gameVersion = cocos2d::CCUserDefault::sharedUserDefault()->getIntegerForKey("gameHistoryVersion");
    if (gameVersion == 1) {
        maxScore = cocos2d::CCUserDefault::sharedUserDefault()->getIntegerForKey("maxScore");
        maxTapsWin = cocos2d::CCUserDefault::sharedUserDefault()->getIntegerForKey("maxTapsWin");
        maxTapsLoss = cocos2d::CCUserDefault::sharedUserDefault()->getIntegerForKey("maxTapsLoss");
        maxWinStreak = cocos2d::CCUserDefault::sharedUserDefault()->getIntegerForKey("maxWinStreak");
        maxLossStreak = cocos2d::CCUserDefault::sharedUserDefault()->getIntegerForKey("maxLossStreak");
        totalScore = cocos2d::CCUserDefault::sharedUserDefault()->getIntegerForKey("totalScore");
        totalGames = cocos2d::CCUserDefault::sharedUserDefault()->getIntegerForKey("totalGames");
        totalWins = cocos2d::CCUserDefault::sharedUserDefault()->getIntegerForKey("totalWins");
        totalTaps = cocos2d::CCUserDefault::sharedUserDefault()->getIntegerForKey("totalTaps");
        currentWinStreak = cocos2d::CCUserDefault::sharedUserDefault()->getIntegerForKey("currentWinStreak");
        currentLossStreak = cocos2d::CCUserDefault::sharedUserDefault()->getIntegerForKey("currentLossStreak");
    }
}

