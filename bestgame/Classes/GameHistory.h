//
//  GameHistory.h
//  bestgame
//
//  Created by Grantoo Developer on 12-11-13.
//  Copyright (c) 2012 Grantoo Developer. All rights reserved.
//

#ifndef __HelloCpp__GameHistory__
#define __HelloCpp__GameHistory__

#include "platform/CCPlatformMacros.h"

class GameHistory
{
public:
    CC_SYNTHESIZE_READONLY(int, maxScore, MaxScore)
    CC_SYNTHESIZE_READONLY(int, maxTapsWin, MaxTapsWin)
    CC_SYNTHESIZE_READONLY(int, maxTapsLoss, MaxTapsLoss)
    CC_SYNTHESIZE_READONLY(int, maxWinStreak, MaxWinStreak)
    CC_SYNTHESIZE_READONLY(int, maxLossStreak, MaxLossStreak)
    CC_SYNTHESIZE_READONLY(int, totalScore, TotalScore)
    CC_SYNTHESIZE_READONLY(int, totalGames, TotalGames)
    CC_SYNTHESIZE_READONLY(int, totalWins, TotalWins)
    CC_SYNTHESIZE_READONLY(int, totalTaps, TotalTaps)
    CC_SYNTHESIZE_READONLY(int, currentWinStreak, CurrentWinStreak)
    CC_SYNTHESIZE_READONLY(int, currentLossStreak, CurrentLossStreak)
    
    float averageScore(void);
    float winPercent(void);
    float averageTaps(void);
    
    void updateHistoryWithScore(int score, int taps);
    
    // Returns an instance of this class as a Singleton
    static GameHistory *instance(void);
    
private:
    bool init(void);

    void reset(void);
    void storeGameHistory(void);
    void loadGameHistory(void);
    
};

#endif // __HelloCpp__GameHistory__
