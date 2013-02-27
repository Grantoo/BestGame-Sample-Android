//
//  GamePayload.h
//  bestgame
//
//  Created by Grantoo Developer on 12-11-01.
//  Copyright (c) 2012 Grantoo Developer. All rights reserved.
//

#ifndef __GamePayload_H__
#define __GamePayload_H__

#include "platform/CCPlatformMacros.h"
#include "cocoa/CCString.h"

class GamePayload
{
public:
    CC_SYNTHESIZE(std::string, tournID, TournID)
    CC_SYNTHESIZE(std::string, matchID, MatchID)
    CC_SYNTHESIZE(int, seed, Seed)
    CC_SYNTHESIZE(int, round, Round)
    CC_SYNTHESIZE(int, score, Score)
    CC_SYNTHESIZE(int, gameType, GameType)
    CC_SYNTHESIZE(bool, completeFlag, CompleteFlag)
    CC_SYNTHESIZE(bool, activeFlag, ActiveFlag)
    
    // Returns an instance of this class as a Singleton
    static GamePayload *instance(void);
    
    void clear(void);
    void store(void);
    
private:
    bool init(void);
    void reset(void);
    void storeGamePayload(void);
    void loadGamePayload(void);
    
};

#endif // __GamePayload_H__
