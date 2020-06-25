#pragma once

//
//  SimpleAI.hpp
//  makhos
//
//  Created by Narongdej Sarnsuwan on 15/6/20.
//  Copyright © 2020 Narongdej Sarnsuwan. All rights reserved.
//

#ifndef SimpleAI_hpp
#define SimpleAI_hpp

#include <stdio.h>
#include <vector>
#include <queue>
#include <iostream>
#include <memory>

#include "Checker.hpp"

namespace makhos {
//
// https://www.geeksforgeeks.org/generic-tree-level-order-traversal/
//

struct PawnLeft
{
    int redLeft;
    int blueLeft;
};

struct EatLoc {
    int from;
    int to;
    int ate;
};

class SimpleAI {
public:
    SimpleAI();
    std::vector<BoardGrid> findBestPossibleMove(std::vector<BoardGrid> gameState, bool isRed, int depth);
    int minimax(std::vector<BoardGrid> gameState, bool isRed, int depth, int alpha, int beta, bool maximizingPlayer);

    PawnLeft evalPosition(std::vector<BoardGrid> &gameState);

    std::vector<std::vector<BoardGrid>> findActionableChild(std::vector<BoardGrid> gameState, bool isRed);

    std::vector<BoardGrid> generateGameState(std::vector<BoardGrid> gameState, int position, BoardPosition moveTo);

private:
    std::shared_ptr<Checker> checker;

};


}

#endif /* SimpleAI_hpp */
